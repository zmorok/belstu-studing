#include "pch.h"
#include "OS11_HTAPI.h"

#include <iostream>
#include <cstring>

using namespace HT;
using std::cout;
using std::endl;

// ----------------- Разметка файла -----------------
#pragma pack(push, 1)
struct FileHeader
{
    char signature[9];      // "OS11HTv1" + '\0'
    int  Capacity;
    int  SecSnapshotInterval;
    int  MaxKeyLength;
    int  MaxPayloadLength;
    int  count;             // физическое количество записанных узлов
	int isActive;           // флаг активности хранилища
};
#pragma pack(pop)

static const char FILE_SIGNATURE[] = "OS11HTv1";

// сколько intов нужно для хранения N байт
static inline int IntsForBytes(int bytes)
{
    return (bytes + sizeof(int) - 1) / sizeof(int);
}

// максимальный размер узла (в int'ах) для заданных ограничений
static inline int MaxNodeInts(int maxKeyLen, int maxPayloadLen)
{
    return 1   // next
        + 1   // key_len
        + IntsForBytes(maxKeyLen)
        + 1   // data_len
        + IntsForBytes(maxPayloadLen);
}

// ---------- Вспомогательные утилиты ------------
static DWORD WINAPI SnapShotCycle(LPVOID param);
static void BuildObjectName(const char* fileName, char* out, size_t outSize);

static void SetErrorMessage(HTHANDLE* ht, const char* message)
{
    if (!ht) return;
    strncpy_s(ht->LastErrorMessage, message, _TRUNCATE);
}

static unsigned int HashKey(const void* key, int len)
{
    unsigned int h = 5381;
    const unsigned char* p = (const unsigned char*)key;
    for (int i = 0; i < len; ++i)
        h = ((h << 5) + h) + p[i]; // h * 33 + p[i]
    return h;
}

// ----------------- Реализация HTHANDLE -----------------
HTHANDLE::HTHANDLE()
{
    Capacity = 0;
    SecSnapshotInterval = 0;
    MaxKeyLength = 0;
    MaxPayloadLength = 0;
    File = nullptr;
    FileMapping = nullptr;
    Addr = nullptr;
    lastsnaptime = 0;
    mutex = nullptr;
    SnapshotThread = nullptr;
    N = 0;
    LastErrorMessage[0] = 0;
    isActive = false;
    FileName[0] = 0;
}

HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval,
    int MaxKeyLength, int MaxPayloadLength,
    const char FileNameParam[512])
{
    this->Capacity = Capacity;
    this->SecSnapshotInterval = SecSnapshotInterval;
    this->MaxKeyLength = MaxKeyLength;
    this->MaxPayloadLength = MaxPayloadLength;
    this->File = nullptr;
    this->FileMapping = nullptr;
    this->Addr = nullptr;
    this->lastsnaptime = 0;
    this->mutex = nullptr;
    this->SnapshotThread = nullptr;
    this->N = 0;
    this->LastErrorMessage[0] = 0;
    this->isActive = false;
    strncpy_s(this->FileName, FileNameParam, _TRUNCATE);
}

// ----------------- Реализация Element -----------------
Element::Element()
{
    key = nullptr;
    keylength = 0;
    payload = nullptr;
    payloadlength = 0;
}

Element::Element(const void* key, int keylength)
{
    this->key = key;
    this->keylength = keylength;
    this->payload = nullptr;
    this->payloadlength = 0;
}

Element::Element(const void* key, int keylength,
    const void* payload, int payloadlength)
{
    this->key = key;
    this->keylength = keylength;
    this->payload = payload;
    this->payloadlength = payloadlength;
}

Element::Element(Element* oldelement,
    const void* newpayload, int newpayloadlength)
{
    key = oldelement->key;
    keylength = oldelement->keylength;
    payload = newpayload;
    payloadlength = newpayloadlength;
}

// ----------------- Create / Open / Close / OpenExistHt / CloseExist -----------------
HTHANDLE* HT::Create(int Capacity, int SecSnapshotInterval,
    int MaxKeyLength, int MaxPayloadLength,
    const char FileName[512])
{
    HANDLE hf = CreateFileA(
        FileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hf == INVALID_HANDLE_VALUE) {
        throw "Invalid HANDLE hf (CreateFileA)";
        return nullptr;
    }

    int nodeInts = MaxNodeInts(MaxKeyLength, MaxPayloadLength);

    DWORD fileSize = sizeof(FileHeader)
        + (DWORD)Capacity * sizeof(int)
        + (DWORD)Capacity * nodeInts * sizeof(int);

    char objName[512];
    BuildObjectName(FileName, objName, sizeof(objName));

    cout << "[HT::Create] Creating HT-Storage: '" << FileName << "'\n";
	cout << "objName: " << objName << "\n";


    HANDLE hm = CreateFileMappingA(
        hf,
        NULL,
        PAGE_READWRITE,
        0,
        fileSize,
        "filemap"
    );
    if (!hm) {
        CloseHandle(hf);
        cout << "Invalid HANDLE hm (CreateFileMappingA)" << GetLastError();
        return nullptr;
    }

    void* addr = MapViewOfFile(
        hm,
        FILE_MAP_ALL_ACCESS | FILE_MAP_READ | FILE_MAP_WRITE,
        0, 0,
        0
    );
    if (!addr) {
        CloseHandle(hm);
        CloseHandle(hf);
        throw "Invalid HANDLE addr (MapViewOfFile)";
        return nullptr;
    }

    ZeroMemory(addr, fileSize);

    FileHeader* hdr = (FileHeader*)addr;
    memcpy(hdr->signature, FILE_SIGNATURE, sizeof(FILE_SIGNATURE));
    hdr->Capacity = Capacity;
    hdr->SecSnapshotInterval = SecSnapshotInterval;
    hdr->MaxKeyLength = MaxKeyLength;
    hdr->MaxPayloadLength = MaxPayloadLength;
    hdr->count = 0;
	hdr->isActive = 1;

    HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval,
        MaxKeyLength, MaxPayloadLength, FileName);
    ht->File = hf;
    ht->FileMapping = hm;
    ht->Addr = addr;
    ht->lastsnaptime = time(NULL);
    ht->mutex = CreateMutexA(NULL, FALSE, "mutex");
    ht->isActive = true;

    DWORD threadId = 0;
    ht->SnapshotThread = CreateThread(NULL, 0, SnapShotCycle, ht, 0, &threadId);
    if (!ht->SnapshotThread) {
        ht->isActive = false;
        UnmapViewOfFile(addr);
        CloseHandle(hm);
        CloseHandle(hf);
        delete ht;
        throw "CreateThread failed (SnapShotCycle)";
        return nullptr;
    }

    return ht;
}

HTHANDLE* HT::Open(const char FileName[512])
{
    HANDLE hf = CreateFileA(
        FileName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hf == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    char objName[512];
    BuildObjectName(FileName, objName, sizeof(objName));

    HANDLE hm = CreateFileMappingA(
        hf,
        NULL,
        PAGE_READWRITE,
        0,
        0,
        "filemap"
    );
    if (!hm) {
        CloseHandle(hf);
        return nullptr;
    }

    void* addr = MapViewOfFile(
        hm,
        FILE_MAP_ALL_ACCESS | FILE_MAP_READ | FILE_MAP_WRITE,
        0, 0,
        0
    );
    if (!addr) {
        CloseHandle(hm);
        CloseHandle(hf);
        return nullptr;
    }

    FileHeader* hdr = (FileHeader*)addr;
	hdr->isActive = 1;
    if (memcmp(hdr->signature, FILE_SIGNATURE, sizeof(FILE_SIGNATURE)) != 0 ||
        hdr->Capacity <= 0 ||
        hdr->MaxKeyLength <= 0 ||
        hdr->MaxPayloadLength <= 0)
    {
        UnmapViewOfFile(addr);
        CloseHandle(hm);
        CloseHandle(hf);
        return nullptr;
    }

    HTHANDLE* ht = new HTHANDLE(
        hdr->Capacity,
        hdr->SecSnapshotInterval,
        hdr->MaxKeyLength,
        hdr->MaxPayloadLength,
        FileName
    );

    ht->File = hf;
    ht->FileMapping = hm;
    ht->Addr = addr;
    ht->lastsnaptime = time(NULL);
    ht->mutex = CreateMutexA(NULL, FALSE, "mutex");
    ht->isActive = true;

    DWORD threadId = 0;
    ht->SnapshotThread = CreateThread(
        NULL, 0,
        SnapShotCycle,
        ht,
        0,
        &threadId
    );
    if (!ht->SnapshotThread) {
        ht->isActive = false;
        UnmapViewOfFile(addr);
        CloseHandle(hm);
        CloseHandle(hf);
        delete ht;
        return nullptr;
    }

    return ht;
}

BOOL HT::Close(const HTHANDLE* hthandle)
{
    if (!hthandle) return FALSE;

    HTHANDLE* ht = const_cast<HTHANDLE*>(hthandle);
    ht->isActive = false;

    if (ht->Addr) {
        FileHeader* hdr = (FileHeader*)ht->Addr;
        hdr->isActive = 0;  
        FlushViewOfFile(hdr, sizeof(FileHeader));
    }

    if (ht->SnapshotThread) {
        WaitForSingleObject(ht->SnapshotThread, INFINITE);
        CloseHandle(ht->SnapshotThread);
    }

    if (ht->Addr)        UnmapViewOfFile(ht->Addr);
    if (ht->FileMapping) CloseHandle(ht->FileMapping);
    if (ht->File)        CloseHandle(ht->File);
    if (ht->mutex)       CloseHandle(ht->mutex);

    delete ht;
    return TRUE;
}

HTHANDLE* HT::OpenExistHt(const char FileName[512])
{
    char objName[512];
    BuildObjectName(FileName, objName, sizeof(objName));

    std::cout << "[OpenExistHt] FileName = '" << FileName << "'\n" << "\tobjName = '" << objName << "'\n";

    HANDLE hm = OpenFileMappingA(
        FILE_MAP_ALL_ACCESS | FILE_MAP_READ | FILE_MAP_WRITE,
        FALSE,
        "filemap"
    );
    if (!hm) {
        std::cout << "[OpenExistHt] OpenFileMappingA FAILED, err=" << GetLastError() << "\n";
        return nullptr;
    }
    
    void* addr = MapViewOfFile(
        hm,
        FILE_MAP_ALL_ACCESS | FILE_MAP_READ | FILE_MAP_WRITE,
        0, 0,
        0
    );
    if (!addr) {
        std::cout << "[OpenExistHt] MapViewOfFile FAILED, err=" << GetLastError() << "\n";
        CloseHandle(hm);
        return nullptr;
    }

    FileHeader* hdr = (FileHeader*)addr;
    if (memcmp(hdr->signature, FILE_SIGNATURE, sizeof(FILE_SIGNATURE)) != 0 ||
        hdr->Capacity <= 0 ||
        hdr->MaxKeyLength <= 0 ||
        hdr->MaxPayloadLength <= 0 ||
        hdr->isActive == 0)
    {
        UnmapViewOfFile(addr);
        CloseHandle(hm);
        return nullptr;
    }

    HTHANDLE* ht = new HTHANDLE(
        hdr->Capacity,
        hdr->SecSnapshotInterval,
        hdr->MaxKeyLength,
        hdr->MaxPayloadLength,
        FileName
    );

    ht->File = nullptr;
    ht->FileMapping = hm;
    ht->Addr = addr;
    ht->lastsnaptime = time(NULL);
    ht->SnapshotThread = nullptr;
    ht->isActive = true;

    return ht;
}


BOOL HT::CloseExistHt(const HTHANDLE* hthandle)
{
    return HT::Close(hthandle);
}

// ----------------- Операции с элементами -----------------
char* HT::GetLastErrorHt(HTHANDLE* ht)
{
    if (!ht) return nullptr;
    return ht->LastErrorMessage;
}

void HT::print(const Element* element)
{
    if (!element) {
        cout << "(empty)" << endl;
        return;
    }

    cout << "Элемент:" << endl;
    cout << "# Key (" << element->keylength << "): ";
    cout.write((const char*)element->key, element->keylength);
    cout << endl;

    cout << "# Data (" << element->payloadlength << "): ";
    cout.write((const char*)element->payload, element->payloadlength);
    cout << endl;
}

BOOL HT::Insert(const HTHANDLE* hthandle, const Element* element)
{
    if (!hthandle || !element) return FALSE;
    HTHANDLE* ht = const_cast<HTHANDLE*>(hthandle);

    WaitForSingleObject(ht->mutex, INFINITE);

    if (!ht->Addr) {
        SetErrorMessage(ht, "Insert: invalid HTHANDLE");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    FileHeader* hdr = (FileHeader*)ht->Addr;

    if (!hdr->isActive) {
        SetErrorMessage(ht, "Storage is closed by owner");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    if (element->keylength <= 0 ||
        element->keylength > hdr->MaxKeyLength ||
        element->payloadlength < 0 ||
        element->payloadlength > hdr->MaxPayloadLength)
    {
        SetErrorMessage(ht, "Insert: key/payload length out of range");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    int capacity = hdr->Capacity;

    // база для индексов = начало массива бакетов
    int* baseBuckets = (int*)((char*)hdr + sizeof(FileHeader));
    int* buckets = baseBuckets;

    int nodeInts = MaxNodeInts(hdr->MaxKeyLength, hdr->MaxPayloadLength);
    int maxIndex = capacity * nodeInts;

    int* data_start = baseBuckets + capacity;
    int* data_end = data_start + capacity * nodeInts;

    unsigned int h = HashKey(element->key, element->keylength);
    int          idx = h % capacity;

    int cellIndex = buckets[idx];

    // проверка на дубликат
    while (cellIndex != 0)
    {
        if (cellIndex < 0 || cellIndex >= maxIndex) {
            SetErrorMessage(ht, "Insert: corrupted bucket index");
            ReleaseMutex(ht->mutex);
            return FALSE;
        }

        int* current = baseBuckets + cellIndex;

        int next = *current++;
        int key_len = *current++;
        char* key_ptr = (char*)current;
        current += IntsForBytes(key_len);

        int data_len = *current++;
        (void)data_len;
        char* data_ptr = (char*)current;
        (void)data_ptr;

        if (key_len == element->keylength &&
            memcmp(key_ptr, element->key, key_len) == 0)
        {
            SetErrorMessage(ht, "Insert: key already exists");
            ReleaseMutex(ht->mutex);
            return FALSE;
        }

        cellIndex = next;
    }

    // ищем позицию в data-области
    int total = hdr->count;
    int* p = data_start;
    for (int i = 0; i < total; ++i)
    {
        int next = *p++; (void)next;
        int key_len = *p++;
        p += IntsForBytes(key_len);
        int data_len = *p++;
        p += IntsForBytes(data_len);
    }

    int needed = 1 + 1 + IntsForBytes(element->keylength)
        + 1 + IntsForBytes(element->payloadlength);

    if (p + needed > data_end)
    {
        SetErrorMessage(ht, "Insert: no free memory");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    int newIndex = (int)(p - baseBuckets);

    *p++ = buckets[idx];           // next
    *p++ = element->keylength;     // key_len
    memcpy(p, element->key, element->keylength);
    p += IntsForBytes(element->keylength);

    *p++ = element->payloadlength; // data_len
    if (element->payloadlength > 0)
        memcpy(p, element->payload, element->payloadlength);
    p += IntsForBytes(element->payloadlength);

    buckets[idx] = newIndex;
    hdr->count++;

    ReleaseMutex(ht->mutex);
    return TRUE;
}

HT::Element* HT::Get(const HTHANDLE* hthandle, const Element* element)
{
    if (!hthandle || !element) return nullptr;
    HTHANDLE* ht = const_cast<HTHANDLE*>(hthandle);

    WaitForSingleObject(ht->mutex, INFINITE);

    if (!ht->Addr) {
        SetErrorMessage(ht, "Get: invalid HTHANDLE");
        ReleaseMutex(ht->mutex);
        return nullptr;
    }

    FileHeader* hdr = (FileHeader*)ht->Addr;

    if (!hdr->isActive) {
        SetErrorMessage(ht, "Storage is closed by owner");
        ReleaseMutex(ht->mutex);
        return FALSE; // или nullptr
    }

    int capacity = hdr->Capacity;

    int* baseBuckets = (int*)((char*)hdr + sizeof(FileHeader));
    int* buckets = baseBuckets;

    int nodeInts = MaxNodeInts(hdr->MaxKeyLength, hdr->MaxPayloadLength);
    int maxIndex = capacity * nodeInts;

    unsigned int h = HashKey(element->key, element->keylength);
    int          idx = h % capacity;

    int cellIndex = buckets[idx];

    while (cellIndex != 0)
    {
        if (cellIndex < 0 || cellIndex >= maxIndex) {
            SetErrorMessage(ht, "Get: corrupted bucket index");
            ReleaseMutex(ht->mutex);
            return nullptr;
        }

        int* current = baseBuckets + cellIndex;

        int next = *current++;
        int key_len = *current++;
        char* key_ptr = (char*)current;
        current += IntsForBytes(key_len);

        int data_len = *current++;
        char* data_ptr = (char*)current;

        if (key_len == element->keylength &&
            memcmp(key_ptr, element->key, key_len) == 0)
        {
            Element* res = new Element(key_ptr, key_len, data_ptr, data_len);
            ReleaseMutex(ht->mutex);
            return res;
        }

        cellIndex = next;
    }

    SetErrorMessage(ht, "Get: key not found");
    ReleaseMutex(ht->mutex);
    return nullptr;
}

BOOL HT::Delete(const HTHANDLE* hthandle, const Element* element)
{
    if (!hthandle || !element) return FALSE;
    HTHANDLE* ht = const_cast<HTHANDLE*>(hthandle);

    WaitForSingleObject(ht->mutex, INFINITE);

    if (!ht->Addr) {
        SetErrorMessage(ht, "Delete: invalid HTHANDLE");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    FileHeader* hdr = (FileHeader*)ht->Addr;

    if (!hdr->isActive) {
        SetErrorMessage(ht, "Storage is closed by owner");
        ReleaseMutex(ht->mutex);
        return FALSE; // или nullptr
    }

    int capacity = hdr->Capacity;

    int* baseBuckets = (int*)((char*)hdr + sizeof(FileHeader));
    int* buckets = baseBuckets;

    int nodeInts = MaxNodeInts(hdr->MaxKeyLength, hdr->MaxPayloadLength);
    int maxIndex = capacity * nodeInts;

    unsigned int h = HashKey(element->key, element->keylength);
    int          idx = h % capacity;

    int cellIndex = buckets[idx];
    int prevIndex = 0;

    while (cellIndex != 0)
    {
        if (cellIndex < 0 || cellIndex >= maxIndex) {
            SetErrorMessage(ht, "Delete: corrupted bucket index");
            ReleaseMutex(ht->mutex);
            return FALSE;
        }

        int* current = baseBuckets + cellIndex;
        int next = *current++;

        int key_len = *current++;
        char* key_ptr = (char*)current;
        current += IntsForBytes(key_len);

        int data_len = *current++; (void)data_len;
        char* data_ptr = (char*)current; (void)data_ptr;

        if (key_len == element->keylength &&
            memcmp(key_ptr, element->key, key_len) == 0)
        {
            if (prevIndex == 0) {
                buckets[idx] = next;
            }
            else {
                int* prev = baseBuckets + prevIndex;
                *prev = next; // правим поле next у предыдущего
            }

            // hdr->count не уменьшаем — физически узел остаётся
            ReleaseMutex(ht->mutex);
            return TRUE;
        }

        prevIndex = cellIndex;
        cellIndex = next;
    }

    SetErrorMessage(ht, "Delete: key not found");
    ReleaseMutex(ht->mutex);
    return FALSE;
}

BOOL HT::Update(const HTHANDLE* hthandle, const Element* oldelement,
    const void* newpayload, int newpayloadlength)
{
    if (!hthandle || !oldelement) return FALSE;
    HTHANDLE* ht = const_cast<HTHANDLE*>(hthandle);

    WaitForSingleObject(ht->mutex, INFINITE);

    if (!ht->Addr) {
        SetErrorMessage(ht, "Update: invalid HTHANDLE");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    FileHeader* hdr = (FileHeader*)ht->Addr;

    if (!hdr->isActive) {
        SetErrorMessage(ht, "Storage is closed by owner");
        ReleaseMutex(ht->mutex);
        return FALSE; // или nullptr
    }

    if (newpayloadlength < 0 ||
        newpayloadlength > hdr->MaxPayloadLength)
    {
        SetErrorMessage(ht, "Update: payload length out of range");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    int capacity = hdr->Capacity;

    int* baseBuckets = (int*)((char*)hdr + sizeof(FileHeader));
    int* buckets = baseBuckets;

    int nodeInts = MaxNodeInts(hdr->MaxKeyLength, hdr->MaxPayloadLength);
    int maxIndex = capacity * nodeInts;

    unsigned int h = HashKey(oldelement->key, oldelement->keylength);
    int          idx = h % capacity;

    int cellIndex = buckets[idx];

    while (cellIndex != 0)
    {
        if (cellIndex < 0 || cellIndex >= maxIndex) {
            SetErrorMessage(ht, "Update: corrupted bucket index");
            ReleaseMutex(ht->mutex);
            return FALSE;
        }

        int* current = baseBuckets + cellIndex;

        int next = *current++;
        int key_len = *current++;
        char* key_ptr = (char*)current;
        current += IntsForBytes(key_len);

        int& data_len = *current++;
        char* data_ptr = (char*)current;

        if (key_len == oldelement->keylength &&
            memcmp(key_ptr, oldelement->key, key_len) == 0)
        {
            if (newpayloadlength > data_len) {
                SetErrorMessage(ht, "Update: new payload too large");
                ReleaseMutex(ht->mutex);
                return FALSE;
            }

            if (newpayloadlength > 0)
                memcpy(data_ptr, newpayload, newpayloadlength);
            data_len = newpayloadlength;

            ReleaseMutex(ht->mutex);
            return TRUE;
        }

        cellIndex = next;
    }

    SetErrorMessage(ht, "Update: key not found");
    ReleaseMutex(ht->mutex);
    return FALSE;
}

// ----------------- Snapshot -----------------

BOOL HT::Snap(const HTHANDLE* hthandle)
{
    if (!hthandle) return FALSE;
    HTHANDLE* ht = const_cast<HTHANDLE*>(hthandle);

    WaitForSingleObject(ht->mutex, INFINITE);

    BOOL ok1 = FlushViewOfFile(ht->Addr, 0);
    BOOL ok2 = FlushFileBuffers(ht->File);

    if (!ok1 || !ok2) {
        SetErrorMessage(ht, "Snap: flush failed");
        ReleaseMutex(ht->mutex);
        return FALSE;
    }

    ReleaseMutex(ht->mutex);
    return TRUE;
}

static DWORD WINAPI SnapShotCycle(LPVOID param)
{
    HTHANDLE* ht = (HTHANDLE*)param;
    if (!ht) return 0;

    while (ht->isActive)
    {
        Sleep(200); // чуть чаще, чем раз в секунду проверяем

        time_t now = time(NULL);
        if (now >= ht->lastsnaptime + ht->SecSnapshotInterval)
        {
            HT::Snap(ht);
            ht->lastsnaptime = now;
            std::cout << "======== SNAPSHOT ========" << std::endl;
        }
    }

    return 0;
}

// helper - попытка преобразовать имя файла в имя объекта
// по итогу - "хардкод имён мэппингов и мьютексов"
static void BuildObjectName(const char* fileName, char* out, size_t outSize)
{
    strncpy_s(out, outSize, fileName, _TRUNCATE);
    for (char* p = out; *p; ++p)
        if (*p == '\\' || *p == '/')
            *p = '_';
}

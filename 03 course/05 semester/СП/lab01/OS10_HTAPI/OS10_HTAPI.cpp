#include "pch.h"
#include "framework.h"

#include "HT.h"
#include <iostream>
using namespace HT;
using namespace std;

#pragma region HTHANDLE::HTHANDLE()

HTHANDLE::HTHANDLE()
{
	Capacity = 0;
	SecSnapshotInterval = 0;
	MaxKeyLength = 0;
	MaxPayloadLength = 0;
	File = nullptr;
	FileMapping = nullptr;
	Addr = nullptr;
	LastErrorMessage[0] = 0;
	lastsnaptime = 0;
}
HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
{
	this->SecSnapshotInterval = SecSnapshotInterval;
	this->MaxKeyLength = MaxKeyLength;
	this->MaxPayloadLength = MaxPayloadLength;
	this->File = CreateFile((LPCWSTR)FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
}

#pragma endregion

#pragma region Element

Element::Element()
{
	this->key = nullptr;
	this->keylength = 0;
	this->payload = nullptr;
	this->payloadlength = 0;
}
Element::Element(const void* key, int keylength)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = nullptr;
	this->payloadlength = 0;
}
Element::Element(const void* key, int keylength, const void* payload, int payloadlength)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = payload;
	this->payloadlength = payloadlength;
}
Element::Element(Element* oldelement, const void* newpayload, int newpayloadlength)
{
	key = oldelement->key;
	keylength = oldelement->keylength;
	payload = newpayload;
	payloadlength = newpayloadlength;
}

#pragma endregion

HTHANDLE* HT::Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
{
	HTHANDLE* ht = new HTHANDLE();
	ht->Capacity = Capacity;
	ht->SecSnapshotInterval = SecSnapshotInterval;
	ht->MaxKeyLength = MaxKeyLength;
	ht->MaxPayloadLength = MaxPayloadLength;
	ht->mutex = CreateMutex(NULL, FALSE, (const wchar_t*)FileName);
	strcpy_s(ht->FileName, FileName);

	ht->File = CreateFileA(
		FileName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (ht->File == INVALID_HANDLE_VALUE) {
		strcpy_s(ht->LastErrorMessage, "HT::Create : CreateFileA failed");
		delete ht;
		return nullptr;
	}

	DWORD fileSize = Capacity * (MaxKeyLength + MaxPayloadLength + sizeof(int));

	ht->FileMapping = CreateFileMappingA(
		ht->File,
		NULL,
		PAGE_READWRITE,
		0,
		fileSize,
		NULL
	);

	if (!ht->FileMapping) {
		strcpy_s(ht->LastErrorMessage, "CreateFileMappingA failed");
		CloseHandle(ht->File);
		delete ht;
		return nullptr;
	}

	ht->Addr = MapViewOfFile(
		ht->FileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0,
		0
	);

	if (!ht->Addr) {
		strcpy_s(ht->LastErrorMessage, "MapViewOfFile failed");
		CloseHandle(ht->FileMapping);
		CloseHandle(ht->File);
		delete ht;
		return nullptr;
	}

	int* ht_start = (int*)ht->Addr;
	*ht_start = 0;

	int* cells = ht_start + 1;

	memset(cells, 0, sizeof(int) * Capacity);

	return ht;
}

HTHANDLE* HT::Open(const char FileName[512])
{
	HANDLE hFile = CreateFileA(
		FileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) return nullptr;

	HANDLE hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, NULL, NULL, NULL);
	if (!hMapFile) { CloseHandle(hFile); return nullptr; }

	void* lp_start = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, NULL, NULL, NULL);
	if (!lp_start) { CloseHandle(hMapFile); CloseHandle(hFile); return nullptr; }

	HTHANDLE* ht = (HTHANDLE*)lp_start;
	if (ht->Capacity <= 0 || ht->MaxKeyLength <= 0 || ht->MaxPayloadLength <= 0)
	{
		UnmapViewOfFile(lp_start);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		return nullptr;
	}

	ht->File = hFile;
	ht->FileMapping = hMapFile;
	ht->Addr = lp_start;
	ht->mutex = CreateMutex(NULL, FALSE, (const wchar_t*)FileName);

	return ht;
}

BOOL HT::Close(const HTHANDLE* hthandle)
{
	UnmapViewOfFile(hthandle->Addr);
	CloseHandle(hthandle->FileMapping);
	CloseHandle(hthandle->File);
	CloseHandle(hthandle->mutex);
	return TRUE;
}

void HT::print(const Element* element)
{
	if (!element) { cout << "(empty)" << endl; return; }

	cout << "Элемент:" << endl;
	cout << "# Key (" << element->keylength << "): "; cout.write((const char*)element->key, element->keylength); cout << endl;
	cout << "# Data (" << element->payloadlength << "): "; cout.write((const char*)element->payload, element->payloadlength); cout << endl;
}

char* HT::GetLastError(HTHANDLE* ht)
{
	if (!ht) return nullptr;
	return ht->LastErrorMessage;
}

BOOL HT::Insert(const HTHANDLE* hthandle, const Element* element)
{
	if (!hthandle) return FALSE;
	
	WaitForSingleObject(hthandle->mutex, INFINITE);

	if (!hthandle->Addr || !element) {
		strcpy_s(((HTHANDLE*)hthandle)->LastErrorMessage, "HT::Insert : invalid HTHANDLE or element");
		ReleaseMutex(hthandle->mutex);
		return FALSE;
	}


	int* ht_start = (int*)hthandle->Addr; int* count_cells = ht_start;
	int* cells = ht_start + 1; int capacity = hthandle->Capacity;

	auto hash = [](const void* key, int len) -> unsigned int
		{
			unsigned int h = 5381;
			const unsigned char* p = (const unsigned char*)key;
			for (int i = 0; i < len; i++)
				h = ((h << 5) + h) + p[i];
			return h;
		};

	int i = hash(element->key, element->keylength) % capacity;

	int cell = cells[i];

	// проверка на дубликат
	while (cell != 0)
	{
		int* current = ht_start + cell;

		int next = *current++; // ссылка на следующий узел

		int key_len = *current++;
		char* key_ptr = (char*)current; current += key_len / sizeof(int) + (key_len % sizeof(int) != 0 ? 1 : 0);

		int data_len = *current++;
		char* data_ptr = (char*)current;

		if (key_len == element->keylength && memcmp(key_ptr, element->key, key_len) == 0) 
		{ // если ключи равны, то выходим с ошибкой
			ReleaseMutex(hthandle->mutex);
			return FALSE;
		}

		cell = next;
	}


	int* data_range_start = ht_start + capacity + 1; // область данных, после ячеек

	// проход до конца для вставки в конец нового узла 
	int total = *count_cells;
	int* element_ptr = data_range_start;
	for (int i = 0; i < total; i++) {
		int next = *element_ptr; element_ptr += 1;

		int key_len = *element_ptr; element_ptr += 1 + (key_len + sizeof(int) - 1) / sizeof(int);

		int data_len = *element_ptr; element_ptr += 1 + (data_len + sizeof(int) - 1) / sizeof(int);
	}

	int new_cell = (int)(element_ptr - ht_start);

	*element_ptr = cells[i]; element_ptr += 1;

	*element_ptr = element->keylength; element_ptr += 1;
	memcpy(element_ptr, element->key, element->keylength); element_ptr += (element->keylength + sizeof(int) - 1) / sizeof(int);

	*element_ptr = element->payloadlength; element_ptr += 1;
	memcpy(element_ptr, element->payload, element->payloadlength); element_ptr += (element->payloadlength + sizeof(int) - 1) / sizeof(int);


	cells[i] = new_cell;

	(*count_cells)++;

	ReleaseMutex(hthandle->mutex);
	return TRUE;
}

BOOL HT::Delete(const HTHANDLE* hthandle, const HT::Element* element)
{
	WaitForSingleObject(hthandle->mutex, INFINITE);
	if (!hthandle) return FALSE;

	if (!hthandle->Addr || !element) {
		strcpy_s(((HTHANDLE*)hthandle)->LastErrorMessage, "HT::Delete : invalid HTHANDLE or element");
		ReleaseMutex(hthandle->mutex);
		return FALSE;
	}

	int* ht_start = (int*)hthandle->Addr; int* count_cells = (int*)ht_start;
	int* cells = ht_start + 1; int capacity = hthandle->Capacity;

	auto hash = [](const void* key, int len) -> unsigned int {
		unsigned int h = 5381;
		const unsigned char* p = (const unsigned char*)key;
		for (int i = 0; i < len; i++)
			h = ((h << 5) + h) + p[i];
		return h;
		};

	int i = hash(element->key, element->keylength) % capacity;

	int cell = cells[i];
	int prev_cell = 0;

	while (cell != 0) {
		int* current = ht_start + cell;
		int next_cell = *current++;

		int key_len = *current++;
		int* key_ptr = current; current += (key_len + sizeof(int) - 1) / sizeof(int);

		if (key_len == element->keylength && memcmp(key_ptr, element->key, key_len) == 0) {
			if (prev_cell == 0) { // удаляемый элемент - голова списка
				cells[i] = next_cell; 
			}
			else {
				int* prev_ptr = ht_start + prev_cell;
				*prev_ptr = next_cell;
			}

			//(*count_cells)--;
			ReleaseMutex(hthandle->mutex);
			return TRUE;
		}

		prev_cell = cell; cell = next_cell;
	}

	ReleaseMutex(hthandle->mutex);
	return FALSE;
}

HT::Element* HT::Get(const HTHANDLE* hthandle, const Element* element)
{
	WaitForSingleObject(hthandle->mutex, INFINITE);
	if (!hthandle) return FALSE;

	if (!hthandle->Addr || !element) {
		strcpy_s(((HTHANDLE*)hthandle)->LastErrorMessage, "HT::Get : invalid HTHANDLE or element");
		ReleaseMutex(hthandle->mutex);
		return FALSE;
	}

	int* ht_start = (int*)hthandle->Addr; int* count_cells = (int*)ht_start;
	int* cells = ht_start + 1; int capacity = hthandle->Capacity;

	auto hash = [](const void* key, int len) -> unsigned int {
		unsigned int h = 5381;
		const unsigned char* p = (const unsigned char*)key;
		for (int i = 0; i < len; i++)
			h = ((h << 5) + h) + p[i];
		return h;
		};

	int i = hash(element->key, element->keylength) % capacity;

	int cell = cells[i];

	while (cell != 0)
	{
		int* current = ht_start + cell;

		int next_cell = *current; current += 1;

		int key_len = *current; current += 1;
		int* key_ptr = current; current += (key_len + sizeof(int) - 1) / sizeof(int);

		int data_len = *current; current += 1;
		int* data_ptr = current;

		if (key_len == element->keylength && memcmp(key_ptr, element->key, key_len) == 0)
		{
			ReleaseMutex(hthandle->mutex);
			return new HT::Element(key_ptr, key_len, data_ptr, data_len);
		}

		cell = next_cell;
	}

	ReleaseMutex(hthandle->mutex);
	return nullptr;
}

BOOL HT::Update(const HTHANDLE* hthandle, const Element* oldelement, const void* newpayload, int newpayloadlength)
{
	WaitForSingleObject(hthandle->mutex, INFINITE);
	if (!hthandle) return FALSE;

	if (!hthandle->Addr || !oldelement) {
		strcpy_s(((HTHANDLE*)hthandle)->LastErrorMessage, "Update: invalid HTHANDLE or element");
		ReleaseMutex(hthandle->mutex);
		return FALSE;
	}

	int* ht_start = (int*)hthandle->Addr; int* count_cells = (int*)ht_start;
	int* cells = ht_start + 1; int capacity = hthandle->Capacity;

	auto hash = [](const void* key, int len) -> unsigned int {
		unsigned int h = 5381;
		const unsigned char* p = (const unsigned char*)key;
		for (int i = 0; i < len; i++)
			h = ((h << 5) + h) + p[i];
		return h;
		};

	int i = hash(oldelement->key, oldelement->keylength) % capacity;

	int cell = cells[i];

	while (cell != 0)
	{
		int* current = ht_start + cell;

		int next_cell = *current; current += 1;
		int key_len = *current; current += 1;
		int* key_ptr = current; current += (key_len + sizeof(int) - 1) / sizeof(int);

		int data_len = *current; current += 1;
		int* data_ptr = current;

		if (key_len == oldelement->keylength && memcmp(key_ptr, oldelement->key, key_len) == 0)
		{
			memcpy(data_ptr, newpayload, newpayloadlength);
			*(current - 1) = newpayloadlength;
			ReleaseMutex(hthandle->mutex);
			return TRUE;
		}

		cell = next_cell;
	}

	ReleaseMutex(hthandle->mutex);
	return FALSE;
}

BOOL HT::Snap(const HTHANDLE* hthandle)
{
	if (!hthandle || !hthandle->Addr) return FALSE;
	WaitForSingleObject(hthandle->mutex, INFINITE);
	if (!FlushViewOfFile(hthandle->Addr, 0) && !FlushFileBuffers(hthandle->File))
	{
		ReleaseMutex(hthandle->mutex);
		return false;
	}

	ReleaseMutex(hthandle->mutex);
	return true;
}

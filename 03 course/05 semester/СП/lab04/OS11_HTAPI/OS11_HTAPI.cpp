#include "pch.h"
#include "framework.h"

#include "HT.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <string.h>

#define INTERNAL_ELEMENT_SIZE(maxKey, maxPayload) (sizeof(int) * 2 + maxKey + maxPayload)
#define HT_FILE_HEADER_SIZE sizeof(HTFileHeader)

namespace HT {

	struct InternalElement {
		int keylength;
		int payloadlength;
	};

	// Функция для получения безопасного имени из пути файла (только имя файла)
	const char* getSafeName(const char* filePath) {
		const char* fileName = strrchr(filePath, '\\');
		if (fileName == NULL) {
			fileName = strrchr(filePath, '/');
		}
		return (fileName != NULL) ? (fileName + 1) : filePath;
	}

	unsigned long hash_function(const char* str, int len) {
		unsigned long hash = 5381;
		for (int i = 0; i < len; i++) {
			hash = ((hash << 5) + hash) + str[i];
		}
		return hash;
	}

	InternalElement* get_internal_element(const HTHANDLE* ht, int index) {
		char* baseAddr = (char*)ht->Addr;
		int elementSize = INTERNAL_ELEMENT_SIZE(ht->MaxKeyLength, ht->MaxPayloadLength);
		return (InternalElement*)(baseAddr + HT_FILE_HEADER_SIZE + elementSize * index);
	}

	DWORD WINAPI SnapshotWorker(LPVOID p) {
		HTHANDLE* ht = (HTHANDLE*)p;
		while (ht->isRunning) {
			Sleep(ht->SecSnapshotInterval * 1000);
			if (!ht->isRunning) break;

			if (ht->Mutex) {
				WaitForSingleObject(ht->Mutex, INFINITE);
				Snap(ht);
				ReleaseMutex(ht->Mutex);
			}
		}
		return 0;
	}

	HTHANDLE::HTHANDLE() : Capacity(0), SecSnapshotInterval(0), MaxKeyLength(0), MaxPayloadLength(0),
		File(INVALID_HANDLE_VALUE), FileMapping(NULL), Addr(NULL), lastsnaptime(0), isRunning(FALSE), SnapshotThread(NULL), Mutex(NULL) {
		memset(FileName, 0, sizeof(FileName));
		memset(LastErrorMessage, 0, sizeof(LastErrorMessage));
	}

	HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
		: HTHANDLE() {
		this->Capacity = Capacity;
		this->SecSnapshotInterval = SecSnapshotInterval;
		this->MaxKeyLength = MaxKeyLength;
		this->MaxPayloadLength = MaxPayloadLength;
		strcpy_s(this->FileName, FileName);
	}


	HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]) {
		HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);

		ht->File = CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (ht->File == INVALID_HANDLE_VALUE) {
			strcpy_s(ht->LastErrorMessage, "Create: File creation failed");
			delete ht;
			return NULL;
		}

		// Записываем заголовок файла
		HTFileHeader header;
		header.Capacity = Capacity;
		header.SecSnapshotInterval = SecSnapshotInterval;
		header.MaxKeyLength = MaxKeyLength;
		header.MaxPayloadLength = MaxPayloadLength;

		DWORD bytesWritten;
		if (!WriteFile(ht->File, &header, sizeof(header), &bytesWritten, NULL)) {
			CloseHandle(ht->File);
			strcpy_s(ht->LastErrorMessage, "Create: Failed to write file header");
			delete ht;
			return NULL;
		}

		// Устанавливаем размер файла: заголовок + данные
		long long mapSize = (long long)INTERNAL_ELEMENT_SIZE(MaxKeyLength, MaxPayloadLength) * Capacity;
		long long totalSize = HT_FILE_HEADER_SIZE + mapSize;

		LARGE_INTEGER fileSize;
		fileSize.QuadPart = totalSize;
		if (SetFilePointerEx(ht->File, fileSize, NULL, FILE_BEGIN) == FALSE ||
			SetEndOfFile(ht->File) == FALSE) {
			CloseHandle(ht->File);
			strcpy_s(ht->LastErrorMessage, "Create: Failed to set file size");
			delete ht;
			return NULL;
		}

		// Создаем именованное отображение файла для доступа из других процессов
		char mappingName[512];
		const char* safeName = getSafeName(FileName);
		sprintf_s(mappingName, "HT_MAPPING_%s", safeName);
		ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, 0, mappingName);
		if (!ht->FileMapping) {
			CloseHandle(ht->File);
			strcpy_s(ht->LastErrorMessage, "Create: CreateFileMapping failed");
			delete ht;
			return NULL;
		}

		ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!ht->Addr) {
			CloseHandle(ht->FileMapping);
			CloseHandle(ht->File);
			strcpy_s(ht->LastErrorMessage, "Create: MapViewOfFile failed");
			delete ht;
			return NULL;
		}

		// Инициализируем данные хранилища нулями (пропускаем заголовок)
		char* dataStart = (char*)ht->Addr + HT_FILE_HEADER_SIZE;
		ZeroMemory(dataStart, mapSize);

		// Создаем именованный мьютекс для синхронизации между процессами
		char mutexName[512];
		sprintf_s(mutexName, "HT_MUTEX_%s", safeName);
		ht->Mutex = CreateMutexA(NULL, FALSE, mutexName);
		if (!ht->Mutex) {
			UnmapViewOfFile(ht->Addr);
			CloseHandle(ht->FileMapping);
			CloseHandle(ht->File);
			strcpy_s(ht->LastErrorMessage, "Create: CreateMutex failed");
			delete ht;
			return NULL;
		}

		return ht;
	}

	HTHANDLE* Open(const char FileName[512]) {
		HTHANDLE* ht = new HTHANDLE();
		strcpy_s(ht->FileName, FileName);

		ht->File = CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (ht->File == INVALID_HANDLE_VALUE) {
			strcpy_s(ht->LastErrorMessage, "Open: File open failed");
			delete ht;
			return NULL;
		}

		// Читаем заголовок файла
		HTFileHeader header;
		DWORD bytesRead;
		if (!ReadFile(ht->File, &header, sizeof(header), &bytesRead, NULL) || bytesRead != sizeof(header)) {
			CloseHandle(ht->File);
			strcpy_s(ht->LastErrorMessage, "Open: Failed to read file header");
			delete ht;
			return NULL;
		}

		ht->Capacity = header.Capacity;
		ht->SecSnapshotInterval = header.SecSnapshotInterval;
		ht->MaxKeyLength = header.MaxKeyLength;
		ht->MaxPayloadLength = header.MaxPayloadLength;

		// Открываем существующее именованное отображение файла
		char mappingName[512];
		const char* safeName = getSafeName(FileName);
		sprintf_s(mappingName, "HT_MAPPING_%s", safeName);
		ht->FileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, mappingName);
		if (!ht->FileMapping) {
			// Если отображение не существует, создаем новое
			ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, 0, mappingName);
			if (!ht->FileMapping) {
				CloseHandle(ht->File);
				strcpy_s(ht->LastErrorMessage, "Open: CreateFileMapping failed");
				delete ht;
				return NULL;
			}
		}

		ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!ht->Addr) {
			CloseHandle(ht->FileMapping);
			CloseHandle(ht->File);
			strcpy_s(ht->LastErrorMessage, "Open: MapViewOfFile failed");
			delete ht;
			return NULL;
		}

		// Открываем существующий именованный мьютекс
		char mutexName[512];
		sprintf_s(mutexName, "HT_MUTEX_%s", safeName);
		ht->Mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, mutexName);
		if (!ht->Mutex) {
			// Если мьютекс не существует, создаем новый
			ht->Mutex = CreateMutexA(NULL, FALSE, mutexName);
			if (!ht->Mutex) {
				UnmapViewOfFile(ht->Addr);
				CloseHandle(ht->FileMapping);
				CloseHandle(ht->File);
				strcpy_s(ht->LastErrorMessage, "Open: CreateMutex failed");
				delete ht;
				return NULL;
			}
		}

		// Не запускаем поток снимков при открытии из другого процесса
		// Только процесс OS11_START должен управлять снимками

		return ht;
	}

	BOOL Snap(const HTHANDLE* hthandle) {
		if (!hthandle || !hthandle->Addr) return FALSE;

		if (!FlushViewOfFile(hthandle->Addr, 0)) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Snap: FlushViewOfFile failed");
			return FALSE;
		}
		const_cast<HTHANDLE*>(hthandle)->lastsnaptime = time(NULL);
		return TRUE;
	}

	BOOL Close(HTHANDLE* hthandle) {
		if (!hthandle) return FALSE;

		if (hthandle->SnapshotThread) {
			hthandle->isRunning = FALSE;
			WaitForSingleObject(hthandle->SnapshotThread, INFINITE);
			CloseHandle(hthandle->SnapshotThread);
		}

		if (hthandle->Mutex) {
			WaitForSingleObject(hthandle->Mutex, INFINITE);
			Snap(hthandle);
			ReleaseMutex(hthandle->Mutex);
			CloseHandle(hthandle->Mutex);
		} else {
			Snap(hthandle);
		}

		if (hthandle->Addr) UnmapViewOfFile(hthandle->Addr);
		if (hthandle->FileMapping) CloseHandle(hthandle->FileMapping);
		if (hthandle->File != INVALID_HANDLE_VALUE) CloseHandle(hthandle->File);

		delete hthandle;
		return TRUE;
	}

	BOOL Insert(const HTHANDLE* hthandle, const Element* element) {
		if (!hthandle || !element || element->keylength > hthandle->MaxKeyLength || element->payloadlength > hthandle->MaxPayloadLength) {
			if (hthandle) strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Insert: Invalid arguments");
			return FALSE;
		}

		if (hthandle->Mutex) {
			WaitForSingleObject(hthandle->Mutex, INFINITE);
		}

		unsigned long startIndex = hash_function((char*)element->key, element->keylength) % hthandle->Capacity;

		BOOL result = FALSE;
		for (int i = 0; i < hthandle->Capacity; ++i) {
			int currentIndex = (startIndex + i) % hthandle->Capacity;
			InternalElement* internal_el = get_internal_element(hthandle, currentIndex);

			if (internal_el->keylength == 0) {
				internal_el->keylength = element->keylength;
				internal_el->payloadlength = element->payloadlength;
				char* dataPtr = (char*)(internal_el + 1);
				memcpy(dataPtr, element->key, element->keylength);
				memcpy(dataPtr + hthandle->MaxKeyLength, element->payload, element->payloadlength);
				result = TRUE;
				break;
			}
			if (internal_el->keylength == element->keylength && memcmp((char*)(internal_el + 1), element->key, element->keylength) == 0) {
				strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Insert: Key already exists");
				result = FALSE;
				break;
			}
		}

		if (!result && !hthandle->LastErrorMessage[0]) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Insert: Storage is full");
		}

		if (hthandle->Mutex) {
			ReleaseMutex(hthandle->Mutex);
		}

		return result;
	}

	Element* Get(const HTHANDLE* hthandle, const Element* element) {
		if (!hthandle || !element) return NULL;

		if (hthandle->Mutex) {
			WaitForSingleObject(hthandle->Mutex, INFINITE);
		}

		unsigned long startIndex = hash_function((char*)element->key, element->keylength) % hthandle->Capacity;

		Element* result = NULL;
		for (int i = 0; i < hthandle->Capacity; ++i) {
			int currentIndex = (startIndex + i) % hthandle->Capacity;
			InternalElement* internal_el = get_internal_element(hthandle, currentIndex);

			if (internal_el->keylength == 0) {
				strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Get: Element not found");
				break;
			}
			if (internal_el->keylength == element->keylength && memcmp((char*)(internal_el + 1), element->key, element->keylength) == 0) {
				char* keyPtr = (char*)(internal_el + 1);
				char* payloadPtr = keyPtr + hthandle->MaxKeyLength;
				result = new Element(keyPtr, internal_el->keylength, payloadPtr, internal_el->payloadlength);
				break;
			}
		}

		if (!result && !hthandle->LastErrorMessage[0]) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Get: Element not found (storage scanned)");
		}

		if (hthandle->Mutex) {
			ReleaseMutex(hthandle->Mutex);
		}

		return result;
	}

	BOOL Delete(const HTHANDLE* hthandle, const Element* element) {
		if (!hthandle || !element) return FALSE;

		if (hthandle->Mutex) {
			WaitForSingleObject(hthandle->Mutex, INFINITE);
		}

		unsigned long startIndex = hash_function((char*)element->key, element->keylength) % hthandle->Capacity;

		BOOL result = FALSE;
		for (int i = 0; i < hthandle->Capacity; ++i) {
			int currentIndex = (startIndex + i) % hthandle->Capacity;
			InternalElement* internal_el = get_internal_element(hthandle, currentIndex);

			if (internal_el->keylength == 0) {
				strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Delete: Element not found");
				break;
			}

			if (internal_el->keylength == element->keylength && memcmp((char*)(internal_el + 1), element->key, element->keylength) == 0) {
				internal_el->keylength = 0;
				internal_el->payloadlength = 0;
				result = TRUE;
				break;
			}
		}

		if (!result && !hthandle->LastErrorMessage[0]) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Delete: Element not found (storage scanned)");
		}

		if (hthandle->Mutex) {
			ReleaseMutex(hthandle->Mutex);
		}

		return result;
	}

	BOOL Update(const HTHANDLE* hthandle, const Element* oldElement, const void* newpayload, int newpayloadlength) {
		if (!hthandle || !oldElement || newpayloadlength > hthandle->MaxPayloadLength) {
			if (hthandle) strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Update: Invalid arguments");
			return FALSE;
		}

		if (hthandle->Mutex) {
			WaitForSingleObject(hthandle->Mutex, INFINITE);
		}

		unsigned long startIndex = hash_function((char*)oldElement->key, oldElement->keylength) % hthandle->Capacity;

		BOOL result = FALSE;
		for (int i = 0; i < hthandle->Capacity; ++i) {
			int currentIndex = (startIndex + i) % hthandle->Capacity;
			InternalElement* internal_el = get_internal_element(hthandle, currentIndex);

			if (internal_el->keylength == 0) {
				strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Update: Element not found");
				break;
			}
			if (internal_el->keylength == oldElement->keylength && memcmp((char*)(internal_el + 1), oldElement->key, oldElement->keylength) == 0) {
				internal_el->payloadlength = newpayloadlength;
				char* dataPtr = (char*)(internal_el + 1);
				memcpy(dataPtr + hthandle->MaxKeyLength, newpayload, newpayloadlength);
				result = TRUE;
				break;
			}
		}

		if (!result && !hthandle->LastErrorMessage[0]) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Update: Element not found (storage scanned)");
		}

		if (hthandle->Mutex) {
			ReleaseMutex(hthandle->Mutex);
		}

		return result;
	}

	char* GetLastErrorMessage(HTHANDLE* ht) {
		return ht ? ht->LastErrorMessage : (char*)"Invalid HTHANDLE";
	}

	void print(const Element* element) {
		if (element == nullptr) {
			std::cout << "Element is NULL" << std::endl;
			return;
		}
		std::cout << "Element key (" << element->keylength << "b): " << std::string((char*)element->key, element->keylength) << std::endl;
		std::cout << "Element payload (" << element->payloadlength << "b): " << std::string((char*)element->payload, element->payloadlength) << std::endl;
	}
}


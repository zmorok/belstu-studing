#pragma once
#include <Windows.h>
#include <time.h>

#ifdef OS11_HTAPI_EXPORTS
#define OS11_HTAPI_API __declspec(dllexport)
#else
#define OS11_HTAPI_API __declspec(dllimport)
#endif

// Структура заголовка файла хранилища
struct HTFileHeader {
	int Capacity;
	int SecSnapshotInterval;
	int MaxKeyLength;
	int MaxPayloadLength;
};

// namespace HT: определяющие структуры для работы с HT-хранилищем 
namespace HT {

	// Структура HTHANDLE: дескриптор HT 
	struct HTHANDLE {
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);

		int Capacity;                 // емкость хранилища в количестве элементов 
		int SecSnapshotInterval;      // периодичность сохранения в сек. 
		int MaxKeyLength;             // максимальная длина ключа 
		int MaxPayloadLength;         // максимальная длина данных 
		char FileName[512];           // имя файла 

		HANDLE File;                  // File HANDLE 
		HANDLE FileMapping;           // Mapping File HANDLE 
		LPVOID Addr;                  // Addr != NULL, если mapview открыт 
		char LastErrorMessage[512];   // сообщение об последней ошибке 
		time_t lastsnaptime;          // время последнего snap'a 

		// Для поддержки асинхронного snapshot-потока
		BOOL isRunning;
		HANDLE SnapshotThread;
		HANDLE Mutex;                 // Именованный мьютекс для синхронизации между процессами
	};

	// Структура Element: элемент хранилища 
	struct Element {
		Element() : key(nullptr), keylength(0), payload(nullptr), payloadlength(0) {}
		Element(const void* key, int keylength) : key(key), keylength(keylength), payload(nullptr), payloadlength(0) {}
		Element(const void* key, int keylength, const void* payload, int payloadlength) : key(key), keylength(keylength), payload(payload), payloadlength(payloadlength) {}
		Element(Element* oldelement, const void* newpayload, int newpayloadlength) : key(oldelement->key), keylength(oldelement->keylength), payload(newpayload), payloadlength(newpayloadlength) {}

		const void* key;              // указатель на ключ 
		int keylength;                // длина ключа 
		const void* payload;          // данные 
		int payloadlength;            // длина данных 
	};

	// --- Экспортируемые функции API ---

	// Create: создает HT-хранилище 
	extern "C" OS11_HTAPI_API HTHANDLE* Create(
		int Capacity,
		int SecSnapshotInterval,
		int MaxKeyLength,
		int MaxPayloadLength,
		const char FileName[512]
	);

	// Open: открывает HT-хранилище 
	extern "C" OS11_HTAPI_API HTHANDLE* Open(
		const char FileName[512]
	);

	// Snap: выполняет snapshot 
	extern "C" OS11_HTAPI_API BOOL Snap(
		const HTHANDLE* hthandle
	);

	// Insert: вставляет элемент в хранилище 
	extern "C" OS11_HTAPI_API BOOL Insert(
		const HTHANDLE* hthandle,
		const Element* element
	);

	// Delete: удаляет элемент из хранилища 
	extern "C" OS11_HTAPI_API BOOL Delete(
		const HTHANDLE* hthandle,
		const Element* element
	);

	// Get: получает элемент из хранилища 
	extern "C" OS11_HTAPI_API Element* Get(
		const HTHANDLE* hthandle,
		const Element* element
	);

	// Update: обновляет элемент в хранилище 
	extern "C" OS11_HTAPI_API BOOL Update(
		const HTHANDLE* hthandle,
		const Element* oldElement,
		const void* newpayload,
		int newpayloadlength
	);

	// GetLastErrorMessage: получает сообщение об последней ошибке 
	extern "C" OS11_HTAPI_API char* GetLastErrorMessage(
		HTHANDLE* ht
	);

	// print: печатает элемент 
	extern "C" OS11_HTAPI_API void print(
		const Element* element
	);

	// Close: выполняет Snap и закрывает HT-хранилище 
	extern "C" OS11_HTAPI_API BOOL Close(
		HTHANDLE* hthandle
	);
}


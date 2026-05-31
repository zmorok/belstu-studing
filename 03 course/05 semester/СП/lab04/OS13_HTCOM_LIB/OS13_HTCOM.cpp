#include "pch.h"
#include "HT.h"
#include "../OS13_HTCOM/OS13_HTCOM.h"
#include <string.h>
#include <time.h>

// Определение GUID (должны совпадать с OS13_HTCOM)
// {C3F44A1D-2F45-47C2-9AE3-2B6FBBE00374}
const GUID CLSID_OS13 =
{ 0xc3f44a1d, 0x2f45, 0x47c2, { 0x9a, 0xe3, 0x2b, 0x6f, 0xbb, 0xe0, 0x3, 0x74 } };

// {0C6EEF7E-2EE4-4E16-B4F0-17420992EC7A}
const GUID IID_IHTStorage =
{ 0xc6eef7e, 0x2ee4, 0x4e16, { 0xb4, 0xf0, 0x17, 0x42, 0x9, 0x92, 0xec, 0x7a } };

namespace HT {

	// Вспомогательная функция для получения COM-интерфейса из HTHANDLE
	static IHTStorage* GetCOMInterface(HTHANDLE* ht) {
		if (!ht || !ht->pCOMInterface) {
			return nullptr;
		}
		return static_cast<IHTStorage*>(ht->pCOMInterface);
	}

	// Вспомогательная функция для создания COM-компонента
	static HRESULT CreateCOMComponent(IHTStorage** ppStorage) {
		if (ppStorage == nullptr) {
			return E_POINTER;
		}

		HRESULT hr = CoCreateInstance(
			CLSID_OS13,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IHTStorage,
			reinterpret_cast<void**>(ppStorage)
		);

		if (FAILED(hr)) {
			char errorMsg[512];
			sprintf_s(errorMsg, "CoCreateInstance failed: 0x%08X", hr);
			OutputDebugStringA(errorMsg);
		}

		return hr;
	}

	HTHANDLE::HTHANDLE() : Capacity(0), SecSnapshotInterval(0), MaxKeyLength(0), MaxPayloadLength(0),
		File(INVALID_HANDLE_VALUE), FileMapping(NULL), Addr(NULL), lastsnaptime(0), isRunning(FALSE), 
		SnapshotThread(NULL), Mutex(NULL), pCOMInterface(nullptr), pCOMHandle(nullptr) {
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

	extern "C" OS13_HTCOM_LIB_API HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]) {
		HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		if (FAILED(hr) && hr != RPC_E_CHANGED_MODE && hr != S_FALSE) {
			return nullptr;
		}

		// Создание COM-компонента
		IHTStorage* pStorage = nullptr;
		hr = CreateCOMComponent(&pStorage);
		if (FAILED(hr) || pStorage == nullptr) {
			return nullptr;
		}

		// Создание HTHANDLE
		HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
		if (ht == nullptr) {
			pStorage->Release();
			return nullptr;
		}

		// Сохранение указателя на COM-интерфейс
		ht->pCOMInterface = pStorage;

		// Вызов CreateHT через COM
		void* pHandle = nullptr;
		hr = pStorage->CreateHT(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName, &pHandle);
		if (FAILED(hr) || pHandle == nullptr) {
			char errorMsg[512];
			sprintf_s(errorMsg, "Create: COM CreateHT failed, HRESULT=0x%08X", hr);
			strcpy_s(ht->LastErrorMessage, errorMsg);
			OutputDebugStringA(errorMsg);
			pStorage->Release();
			delete ht;
			return nullptr;
		}

		ht->pCOMHandle = pHandle;
		return ht;
	}

	extern "C" OS13_HTCOM_LIB_API HTHANDLE* Open(const char FileName[512]) {
		HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		if (FAILED(hr) && hr != RPC_E_CHANGED_MODE && hr != S_FALSE) {
			return nullptr;
		}

		// Создание COM-компонента
		IHTStorage* pStorage = nullptr;
		hr = CreateCOMComponent(&pStorage);
		if (FAILED(hr) || pStorage == nullptr) {
			char errorMsg[512];
			sprintf_s(errorMsg, "Open: Failed to create COM component, HRESULT=0x%08X", hr);
			OutputDebugStringA(errorMsg);
			return nullptr;
		}

		// Создание HTHANDLE
		HTHANDLE* ht = new HTHANDLE();
		if (ht == nullptr) {
			pStorage->Release();
			return nullptr;
		}

		strcpy_s(ht->FileName, FileName);
		ht->pCOMInterface = pStorage;

		// Вызов OpenHT через COM
		void* pHandle = nullptr;
		hr = pStorage->OpenHT(FileName, &pHandle);
		if (FAILED(hr) || pHandle == nullptr) {
			if (pHandle != nullptr) {
				char errorMsg[512];
				pStorage->GetLastErrorMessage(pHandle, errorMsg, sizeof(errorMsg));
				strcpy_s(ht->LastErrorMessage, errorMsg);
			} else {
				strcpy_s(ht->LastErrorMessage, "Open: Failed to open HT storage");
			}
			pStorage->Release();
			delete ht;
			return nullptr;
		}

		ht->pCOMHandle = pHandle;
		return ht;
	}

	extern "C" OS13_HTCOM_LIB_API BOOL Snap(const HTHANDLE* hthandle) {
		if (!hthandle) {
			return FALSE;
		}

		IHTStorage* pStorage = GetCOMInterface(const_cast<HTHANDLE*>(hthandle));
		if (pStorage == nullptr) {
			return FALSE;
		}

		HRESULT hr = pStorage->SnapHT(hthandle->pCOMHandle);
		if (SUCCEEDED(hr)) {
			const_cast<HTHANDLE*>(hthandle)->lastsnaptime = time(NULL);
			return TRUE;
		}

		return FALSE;
	}

	extern "C" OS13_HTCOM_LIB_API BOOL Insert(const HTHANDLE* hthandle, const Element* element) {
		if (!hthandle || !element) {
			if (hthandle) {
				strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Insert: Invalid arguments");
			}
			return FALSE;
		}

		IHTStorage* pStorage = GetCOMInterface(const_cast<HTHANDLE*>(hthandle));
		if (pStorage == nullptr) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Insert: COM interface not available");
			return FALSE;
		}

		// Element в HTElement
		HTElement htElement;
		htElement.key = element->key;
		htElement.keylength = element->keylength;
		htElement.payload = element->payload;
		htElement.payloadlength = element->payloadlength;

		HRESULT hr = pStorage->Insert(hthandle->pCOMHandle, &htElement);
		if (SUCCEEDED(hr)) {
			return TRUE;
		}

		char errorMsg[512];
		pStorage->GetLastErrorMessage(hthandle->pCOMHandle, errorMsg, sizeof(errorMsg));
		strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, errorMsg);
		return FALSE;
	}

	extern "C" OS13_HTCOM_LIB_API BOOL Delete(const HTHANDLE* hthandle, const Element* element) {
		if (!hthandle || !element) {
			if (hthandle) {
				strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Delete: Invalid arguments");
			}
			return FALSE;
		}

		IHTStorage* pStorage = GetCOMInterface(const_cast<HTHANDLE*>(hthandle));
		if (pStorage == nullptr) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Delete: COM interface not available");
			return FALSE;
		}

		// Element в HTElement
		HTElement htElement;
		htElement.key = element->key;
		htElement.keylength = element->keylength;
		htElement.payload = nullptr;
		htElement.payloadlength = 0;

		HRESULT hr = pStorage->Delete(hthandle->pCOMHandle, &htElement);
		if (SUCCEEDED(hr)) {
			return TRUE;
		}

		char errorMsg[512];
		pStorage->GetLastErrorMessage(hthandle->pCOMHandle, errorMsg, sizeof(errorMsg));
		strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, errorMsg);
		return FALSE;
	}

	extern "C" OS13_HTCOM_LIB_API Element* Get(const HTHANDLE* hthandle, const Element* element) {
		if (!hthandle || !element) {
			return nullptr;
		}

		IHTStorage* pStorage = GetCOMInterface(const_cast<HTHANDLE*>(hthandle));
		if (pStorage == nullptr) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Get: COM interface not available");
			return nullptr;
		}

		// Element в HTElement
		HTElement htElement;
		htElement.key = element->key;
		htElement.keylength = element->keylength;
		htElement.payload = nullptr;
		htElement.payloadlength = 0;

		HTElementResult* pResult = nullptr;
		HRESULT hr = pStorage->Get(hthandle->pCOMHandle, &htElement, &pResult);
		if (FAILED(hr) || pResult == nullptr) {
			// Получаем сообщение об ошибке
			char errorMsg[512];
			pStorage->GetLastErrorMessage(hthandle->pCOMHandle, errorMsg, sizeof(errorMsg));
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, errorMsg);
			return nullptr;
		}

		// Сохранение размеров перед освобождением
		int keyLength = pResult->keylength;
		int payloadLength = pResult->payloadlength;

		// Копирование HTElementResult в Element
		void* keyCopy = new char[keyLength];
		if (keyCopy == nullptr) {
			pStorage->FreeElement(pResult);
			return nullptr;
		}
		memcpy(keyCopy, pResult->key, keyLength);

		// Выделение памяти для payload
		void* payloadCopy = nullptr;
		if (payloadLength > 0) {
			payloadCopy = new char[payloadLength];
			if (payloadCopy == nullptr) {
				delete[] static_cast<char*>(keyCopy);
				pStorage->FreeElement(pResult);
				return nullptr;
			}
			memcpy(payloadCopy, pResult->payload, payloadLength);
		}

		// Освобождение память под COM
		pStorage->FreeElement(pResult);

		// Создание Element с скопированными данными
		Element* result = new Element(keyCopy, keyLength, payloadCopy, payloadLength);
		if (result == nullptr) {
			delete[] static_cast<char*>(keyCopy);
			if (payloadCopy) {
				delete[] static_cast<char*>(payloadCopy);
			}
			return nullptr;
		}

		return result;
	}

	extern "C" OS13_HTCOM_LIB_API BOOL Update(const HTHANDLE* hthandle, const Element* oldElement, const void* newpayload, int newpayloadlength) {
		if (!hthandle || !oldElement) {
			if (hthandle) {
				strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Update: Invalid arguments");
			}
			return FALSE;
		}

		IHTStorage* pStorage = GetCOMInterface(const_cast<HTHANDLE*>(hthandle));
		if (pStorage == nullptr) {
			strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, "Update: COM interface not available");
			return FALSE;
		}

		// Element в HTElement
		HTElement htElement;
		htElement.key = oldElement->key;
		htElement.keylength = oldElement->keylength;
		htElement.payload = nullptr;
		htElement.payloadlength = 0;

		HRESULT hr = pStorage->Update(hthandle->pCOMHandle, &htElement, newpayload, newpayloadlength);
		if (SUCCEEDED(hr)) {
			return TRUE;
		}

		char errorMsg[512];
		pStorage->GetLastErrorMessage(hthandle->pCOMHandle, errorMsg, sizeof(errorMsg));
		strcpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage, errorMsg);
		return FALSE;
	}

	extern "C" OS13_HTCOM_LIB_API char* GetLastErrorMessage(HTHANDLE* ht) {
		if (!ht) {
			return (char*)"Invalid HTHANDLE";
		}

		IHTStorage* pStorage = GetCOMInterface(ht);
		if (pStorage != nullptr && ht->pCOMHandle != nullptr) {
			pStorage->GetLastErrorMessage(ht->pCOMHandle, ht->LastErrorMessage, sizeof(ht->LastErrorMessage));
		}

		return ht->LastErrorMessage;
	}

	void print(const Element* element) {
		if (!element) {
			std::cout << "Element: NULL" << std::endl;
			return;
		}

		std::cout << "Element:" << std::endl;
		std::cout << "  Key length: " << element->keylength << std::endl;
		if (element->key && element->keylength > 0) {
			std::cout << "  Key: ";
			for (int i = 0; i < element->keylength; i++) {
				std::cout << static_cast<const char*>(element->key)[i];
			}
			std::cout << std::endl;
		}
		std::cout << "  Payload length: " << element->payloadlength << std::endl;
		if (element->payload && element->payloadlength > 0) {
			std::cout << "  Payload: ";
			for (int i = 0; i < element->payloadlength; i++) {
				std::cout << static_cast<const char*>(element->payload)[i];
			}
			std::cout << std::endl;
		}
	}

	extern "C" OS13_HTCOM_LIB_API BOOL Close(HTHANDLE* hthandle) {
		if (!hthandle) {
			return FALSE;
		}

		IHTStorage* pStorage = GetCOMInterface(hthandle);
		if (pStorage != nullptr) {
			if (hthandle->pCOMHandle != nullptr) {
				pStorage->CloseHT(hthandle->pCOMHandle);
			}
			pStorage->Release();
		}
		delete hthandle;
		return TRUE;
	}
}
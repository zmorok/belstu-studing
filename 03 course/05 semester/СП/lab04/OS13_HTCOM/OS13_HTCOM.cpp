#include "pch.h"
#include "OS13_HTCOM.h"
#include "Registry.h"
#include "../OS11_HTAPI/HT.h"
#include <string.h>

// {C3F44A1D-2F45-47C2-9AE3-2B6FBBE00374}
const GUID CLSID_OS13 =
{ 0xc3f44a1d, 0x2f45, 0x47c2, { 0x9a, 0xe3, 0x2b, 0x6f, 0xbb, 0xe0, 0x3, 0x74 } };

// {0C6EEF7E-2EE4-4E16-B4F0-17420992EC7A}
const GUID IID_IHTStorage =
{ 0xc6eef7e, 0x2ee4, 0x4e16, { 0xb4, 0xf0, 0x17, 0x42, 0x9, 0x92, 0xec, 0x7a } };


// Счетчик блокировок
static ULONG g_cLock = 0;

// Реализация интерфейса IHTStorage в виде класса-компонента OS13 
class OS13 : public IHTStorage
{
private:
	ULONG m_cRef;  // Счетчик ссылок

public:
	OS13() : m_cRef(1)
	{
		InterlockedIncrement(&g_cLock);
	}

	~OS13()
	{
		InterlockedDecrement(&g_cLock);
	}

	// IUnknown методы
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) override
	{
		if (ppv == nullptr)
			return E_POINTER;

		if (riid == IID_IUnknown || riid == IID_IHTStorage)
		{
			*ppv = static_cast<IHTStorage*>(this);
		}
		else
		{
			*ppv = nullptr;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	ULONG __stdcall AddRef() override
	{
		return InterlockedIncrement(&m_cRef);
	}

	ULONG __stdcall Release() override
	{
		ULONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}
		return cRef;
	}

	// IHTStorage методы
	HRESULT __stdcall CreateHT(
		int Capacity,
		int SecSnapshotInterval,
		int MaxKeyLength,
		int MaxPayloadLength,
		const char FileName[512],
		void** ppHandle
	) override
	{
		if (ppHandle == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = HT::Create(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
		if (ht == nullptr)
		{
			return E_FAIL;
		}

		*ppHandle = static_cast<void*>(ht);
		return S_OK;
	}

	HRESULT __stdcall OpenHT(
		const char FileName[512],
		void** ppHandle
	) override
	{
		if (ppHandle == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = HT::Open(FileName);
		if (ht == nullptr)
		{
			return E_FAIL;
		}

		*ppHandle = static_cast<void*>(ht);
		return S_OK;
	}

	HRESULT __stdcall SnapHT(
		void* pHandle
	) override
	{
		if (pHandle == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = static_cast<HT::HTHANDLE*>(pHandle);
		BOOL result = HT::Snap(ht);
		return result ? S_OK : E_FAIL;
	}

	HRESULT __stdcall Insert(
		void* pHandle,
		const HTElement* pElement
	) override
	{
		if (pHandle == nullptr || pElement == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = static_cast<HT::HTHANDLE*>(pHandle);
		HT::Element element(pElement->key, pElement->keylength, pElement->payload, pElement->payloadlength);
		
		BOOL result = HT::Insert(ht, &element);
		return result ? S_OK : E_FAIL;
	}

	HRESULT __stdcall Delete(
		void* pHandle,
		const HTElement* pElement
	) override
	{
		if (pHandle == nullptr || pElement == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = static_cast<HT::HTHANDLE*>(pHandle);
		HT::Element element(pElement->key, pElement->keylength);
		
		BOOL result = HT::Delete(ht, &element);
		return result ? S_OK : E_FAIL;
	}

	HRESULT __stdcall Get(
		void* pHandle,
		const HTElement* pElement,
		HTElementResult** ppResult
	) override
	{
		if (pHandle == nullptr || pElement == nullptr || ppResult == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = static_cast<HT::HTHANDLE*>(pHandle);
		HT::Element searchElement(pElement->key, pElement->keylength);
		
		HT::Element* result = HT::Get(ht, &searchElement);
		if (result == nullptr)
		{
			return E_FAIL;
		}

		// Выделяем память для результата
		HTElementResult* pResult = (HTElementResult*)CoTaskMemAlloc(sizeof(HTElementResult));
		if (pResult == nullptr)
		{
			delete result;  // Освобождаем память, выделенную HT::Get
			return E_OUTOFMEMORY;
		}

		// Копируем ключ
		pResult->keylength = result->keylength;
		pResult->key = CoTaskMemAlloc(result->keylength);
		if (pResult->key == nullptr)
		{
			delete result;  // Освобождаем память, выделенную HT::Get
			CoTaskMemFree(pResult);
			return E_OUTOFMEMORY;
		}
		memcpy(pResult->key, result->key, result->keylength);

		// Копируем payload
		pResult->payloadlength = result->payloadlength;
		if (result->payloadlength > 0)
		{
			pResult->payload = CoTaskMemAlloc(result->payloadlength);
			if (pResult->payload == nullptr)
			{
				delete result;  // Освобождаем память, выделенную HT::Get
				CoTaskMemFree(pResult->key);
				CoTaskMemFree(pResult);
				return E_OUTOFMEMORY;
			}
			memcpy(pResult->payload, result->payload, result->payloadlength);
		}
		else
		{
			pResult->payload = nullptr;
		}

		// Освобождаем память, выделенную HT::Get
		delete result;

		*ppResult = pResult;
		return S_OK;
	}

	HRESULT __stdcall Update(
		void* pHandle,
		const HTElement* pOldElement,
		const void* newpayload,
		int newpayloadlength
	) override
	{
		if (pHandle == nullptr || pOldElement == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = static_cast<HT::HTHANDLE*>(pHandle);
		HT::Element oldElement(pOldElement->key, pOldElement->keylength);
		
		BOOL result = HT::Update(ht, &oldElement, newpayload, newpayloadlength);
		return result ? S_OK : E_FAIL;
	}

	HRESULT __stdcall GetLastErrorMessage(
		void* pHandle,
		char* pBuffer,
		int bufferSize
	) override
	{
		if (pHandle == nullptr || pBuffer == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = static_cast<HT::HTHANDLE*>(pHandle);
		char* errorMsg = HT::GetLastErrorMessage(ht);
		
		if (errorMsg == nullptr)
		{
			pBuffer[0] = '\0';
			return S_OK;
		}

		strcpy_s(pBuffer, bufferSize, errorMsg);
		return S_OK;
	}

	HRESULT __stdcall CloseHT(
		void* pHandle
	) override
	{
		if (pHandle == nullptr)
			return E_POINTER;

		HT::HTHANDLE* ht = static_cast<HT::HTHANDLE*>(pHandle);
		BOOL result = HT::Close(ht);
		return result ? S_OK : E_FAIL;
	}

	HRESULT __stdcall FreeElement(
		HTElementResult* pElement
	) override
	{
		if (pElement == nullptr)
			return E_POINTER;

		if (pElement->key != nullptr)
		{
			CoTaskMemFree(pElement->key);
		}
		if (pElement->payload != nullptr)
		{
			CoTaskMemFree(pElement->payload);
		}
		CoTaskMemFree(pElement);
		return S_OK;
	}
};

// Фабрика классов
class OS13Factory : public IClassFactory
{
private:
	ULONG m_cRef;

public:
	OS13Factory() : m_cRef(1)
	{
		InterlockedIncrement(&g_cLock);
	}

	~OS13Factory()
	{
		InterlockedDecrement(&g_cLock);
	}

	// IUnknown методы
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) override
	{
		if (ppv == nullptr)
			return E_POINTER;

		if (riid == IID_IUnknown || riid == IID_IClassFactory)
		{
			*ppv = static_cast<IClassFactory*>(this);
		}
		else
		{
			*ppv = nullptr;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	ULONG __stdcall AddRef() override
	{
		return InterlockedIncrement(&m_cRef);
	}

	ULONG __stdcall Release() override
	{
		ULONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}
		return cRef;
	}

	// IClassFactory методы
	HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) override
	{
		if (ppv == nullptr)
			return E_POINTER;

		if (pUnkOuter != nullptr)
			return CLASS_E_NOAGGREGATION;

		OS13* pOS13 = new OS13();
		if (pOS13 == nullptr)
			return E_OUTOFMEMORY;

		HRESULT hr = pOS13->QueryInterface(riid, ppv);
		pOS13->Release();

		return hr;
	}

	HRESULT __stdcall LockServer(BOOL fLock) override
	{
		if (fLock)
			InterlockedIncrement(&g_cLock);
		else
			InterlockedDecrement(&g_cLock);
		return S_OK;
	}
};

// Экспортируемые функции DLL

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	if (ppv == nullptr)
		return E_POINTER;

	if (rclsid != CLSID_OS13)
		return CLASS_E_CLASSNOTAVAILABLE;

	OS13Factory* pFactory = new OS13Factory();
	if (pFactory == nullptr)
		return E_OUTOFMEMORY;

	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	pFactory->Release();

	return hr;
}

STDAPI DllCanUnloadNow(void)
{
	return (g_cLock == 0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer(void)
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)&DllRegisterServer,
		&hModule
	);
	return RegisterServer(
		hModule,
		CLSID_OS13,
		L"OS13 HT Component",
		L"OS13",
		L"OS13.1"
	);
}

STDAPI DllUnregisterServer(void)
{
	return UnregisterServer(
		CLSID_OS13,
		L"OS13",
		L"OS13.1"
	);
}


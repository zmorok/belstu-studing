#pragma once

#include <windows.h>
#include <objbase.h>
#include <guiddef.h>

namespace HT {
	struct HTHANDLE;
	struct Element;
}

struct HTElement {
	const void* key;              // указатель на ключ
	int keylength;                // длина ключа
	const void* payload;          // данные
	int payloadlength;            // длина данных
};

// Element через COM
struct HTElementResult {
	void* key;                    // указатель на ключ (выделенная память)
	int keylength;                // длина ключа
	void* payload;                // данные (выделенная память)
	int payloadlength;            // длина данных
};

interface IHTStorage : public IUnknown
{
	// Create: создает HT-хранилище
	virtual HRESULT __stdcall CreateHT(
		int Capacity,
		int SecSnapshotInterval,
		int MaxKeyLength,
		int MaxPayloadLength,
		const char FileName[512],
		void** ppHandle
	) = 0;

	// Open: открывает HT-хранилище
	virtual HRESULT __stdcall OpenHT(
		const char FileName[512],
		void** ppHandle
	) = 0;

	// Snap: выполняет snapshot
	virtual HRESULT __stdcall SnapHT(
		void* pHandle
	) = 0;

	// Insert: вставляет элемент в хранилище
	virtual HRESULT __stdcall Insert(
		void* pHandle,
		const HTElement* pElement
	) = 0;

	// Delete: удаляет элемент из хранилища
	virtual HRESULT __stdcall Delete(
		void* pHandle,
		const HTElement* pElement
	) = 0;

	// Get: получает элемент из хранилища
	virtual HRESULT __stdcall Get(
		void* pHandle,
		const HTElement* pElement,
		HTElementResult** ppResult
	) = 0;

	// Update: обновляет элемент в хранилище
	virtual HRESULT __stdcall Update(
		void* pHandle,
		const HTElement* pOldElement,
		const void* newpayload,
		int newpayloadlength
	) = 0;

	// GetLastErrorMessage: получает сообщение об последней ошибке
	virtual HRESULT __stdcall GetLastErrorMessage(
		void* pHandle,
		char* pBuffer,
		int bufferSize
	) = 0;

	// Close: закрывает HT-хранилище
	virtual HRESULT __stdcall CloseHT(
		void* pHandle
	) = 0;

	// FreeElement: освобождает память, выделенную для HTElementResult
	virtual HRESULT __stdcall FreeElement(
		HTElementResult* pElement
	) = 0;
};

// GUID для CLSID_OS13
// {C3F44A1D-2F45-47C2-9AE3-2B6FBBE00374}
extern const GUID CLSID_OS13;

// GUID для IID_IHTStorage
// {0C6EEF7E-2EE4-4E16-B4F0-17420992EC7A}
extern const GUID IID_IHTStorage;

#include "pch.h"
#include "OS12.h"
#include "MathFactory.h"
#include <new>

class MathFactory :public IClassFactory {
public:
	MathFactory() :m_ref(1) {}
	virtual ~MathFactory() {}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override {
		if (!ppv) return E_POINTER;
		*ppv = nullptr;
		if (IsEqualIID(riid, IID_IUnknown)||IsEqualIID(riid, IID_IClassFactory)) {
			*ppv = static_cast<IClassFactory*>(this);
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef() override {
		return InterlockedIncrement(&m_ref);
	}

	STDMETHODIMP_(ULONG) Release() override {
		ULONG res = InterlockedDecrement(&m_ref);
		if (res == 0) delete this;
		return res;
	}

	STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override {
		if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;
		if (!ppvObject) return E_POINTER;
		return CreateOS12Instance(riid, ppvObject);
	}

	STDMETHODIMP LockServer(BOOL flock) override {
		if (flock) InterlockedIncrement(&m_ref);
		else InterlockedDecrement(&m_ref);
		return S_OK;
	}

private:
	volatile LONG m_ref;
};


HRESULT CreateMathFactoryInstance(REFIID riid, void** ppv) {
	if (!ppv) return E_POINTER;
	*ppv = nullptr;
	MathFactory* pObj = new (std::nothrow) MathFactory();
	if (!pObj) return E_OUTOFMEMORY;
	HRESULT hr = pObj->QueryInterface(riid, ppv);
	pObj->Release();
	return hr;
}
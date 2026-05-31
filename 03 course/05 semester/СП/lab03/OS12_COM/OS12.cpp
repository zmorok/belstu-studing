#include "pch.h"
#include "OS12.h"
#include <atomic>

LONG g_cObjects = 0;
LONG g_cServerLocks = 0;

class OS12 :public IAdder, public IMultiplier {
public:
	OS12() :m_ref(1) { InterlockedIncrement(&g_cObjects); }
	virtual ~OS12() { InterlockedDecrement(&g_cObjects); }

	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override {
		if (!ppv) return E_POINTER;
		*ppv = nullptr;

		if (IsEqualIID(riid, IID_IUnknown))			*ppv = static_cast<IUnknown*>(static_cast<IAdder*>(this));
		else if (IsEqualIID(riid, IID_IAdder))		*ppv = static_cast<IAdder*>(this);
		else if (IsEqualIID(riid, IID_IMultiplier)) *ppv = static_cast<IMultiplier*>(this);
		else										return E_NOINTERFACE;

		AddRef();
		return S_OK;
	}

	STDMETHODIMP_(ULONG) AddRef() override { return InterlockedIncrement(&m_ref); }

	STDMETHODIMP_(ULONG) Release()override {

		ULONG res = InterlockedDecrement(&m_ref);
		if (res == 0) delete this;
		return res;
	}

	STDMETHODIMP Add(const double x, const double y, double& z) override {
		z = x + y;
		return S_OK;
	}

	STDMETHODIMP Sub(const double x, const double y, double& z) override {
		z = x - y;
		return S_OK;
	}
	STDMETHODIMP Mul(const double x, const double y, double& z) override {
		z = x * y;
		return S_OK;
	}
	STDMETHODIMP Div(const double x, const double y, double& z)override {
		if (y == 0.0) {	return E_INVALIDARG; }
		z = x / y;
		return S_OK;
	}
private:
	volatile LONG m_ref;
};

HRESULT CreateOS12Instance(REFIID riid, void** ppv) {
	if (!ppv) return E_POINTER;
	*ppv = nullptr;

	OS12* pObj = new (std::nothrow) OS12();
	if (!pObj) return E_OUTOFMEMORY;

	HRESULT hr = pObj->QueryInterface(riid, ppv);
	pObj->Release();
	return hr;
}
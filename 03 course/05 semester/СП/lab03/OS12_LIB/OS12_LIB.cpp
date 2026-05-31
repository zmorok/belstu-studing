#include "pch.h"
#include "OS12_Lib.h"
#include <Unknwn.h>
#include <iostream>
#include "../OS12_COM/OS12.h"
#include <stdexcept>
#include <string>

using namespace std;

class OS12Exception : public runtime_error {
public:
    explicit OS12Exception(const string& msg, HRESULT hr = E_FAIL) : runtime_error(msg), m_hr(hr) {}
    HRESULT hr() const noexcept { return m_hr; }
private:
    HRESULT m_hr;
};

struct OS12_Context {
    IAdder* pIAdder = nullptr;
    IMultiplier* pIMultiplier = nullptr;
    bool coInitialized = false;
};

static void ThrowHr(HRESULT hr, const char* message) {
    if (FAILED(hr)) {
        string msg = message;
        msg += " (HRESULT=0x";

        char buf[16];
        sprintf_s(buf, "%08X", static_cast<unsigned int>(hr));
        msg += buf;
        msg += ")";

        throw OS12Exception(msg, hr);
    }
}

static OS12_Context* ValidateHandle(OS12HANDLE h) {
    if (h == nullptr) {
        throw invalid_argument("OS12 handle is null");
    }
    return reinterpret_cast<OS12_Context*>(h);
}

namespace OS12 {

    OS12HANDLE Init() {
        OS12_Context* ctx = nullptr;

        try {
            ctx = new OS12_Context();
        }
        catch (const bad_alloc&) {
            throw runtime_error("Out of memory allocation OS12_Context structure");
        }

        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        if (hr == RPC_E_CHANGED_MODE) ctx->coInitialized = false;
        else if (SUCCEEDED(hr)) ctx->coInitialized = true;
        else {
            delete ctx;
            ThrowHr(hr, "CoInitializeEx failed");
        }

        IAdder* pAdder = nullptr;
        hr = CoCreateInstance(CLSID_OS12, nullptr, CLSCTX_INPROC_SERVER,
            IID_IAdder, reinterpret_cast<void**>(&pAdder));

        if (FAILED(hr) || !pAdder) {
            if (ctx->coInitialized) CoUninitialize();
            delete ctx;
            ThrowHr(hr, "CoCreateInstance for IAdder failed");
        }

        IMultiplier* pMultiplier = nullptr;
        hr = pAdder->QueryInterface(IID_IMultiplier, reinterpret_cast<void**>(&pMultiplier));
        if (FAILED(hr) || !pMultiplier) {
            pAdder->Release();
            if (ctx->coInitialized) CoUninitialize();
            delete ctx;
            ThrowHr(hr, "QueryInterface for IMultiplier failed");
        }

        ctx->pIAdder = pAdder;
        ctx->pIMultiplier = pMultiplier;
        return reinterpret_cast<OS12HANDLE>(ctx);
    }

    namespace Adder {

        double Add(OS12HANDLE h, double x, double y) {
            OS12_Context* ctx = ValidateHandle(h);
            if (!ctx->pIAdder) throw runtime_error("Adder interface not initialized");

            double result = 0.0;
            HRESULT hr = ctx->pIAdder->Add(x, y, result);

            if (FAILED(hr)) ThrowHr(hr, "IAdder::Add failed");
            return result;
        }

        double Sub(OS12HANDLE h, double x, double y) {
            OS12_Context* ctx = ValidateHandle(h);
            if (!ctx->pIAdder) throw runtime_error("Adder interface not initialized");

            double result = 0.0;
            HRESULT hr = ctx->pIAdder->Sub(x, y, result);

            if (FAILED(hr)) ThrowHr(hr, "IAdder::Sub failed");
            return result;
        }
    }

    namespace Multiplier {

        double Mul(OS12HANDLE h, double x, double y) {
            OS12_Context* ctx = ValidateHandle(h);
            if (!ctx->pIMultiplier) throw runtime_error("Multiplier interface not initialized");

            double result = 0.0;
            HRESULT hr = ctx->pIMultiplier->Mul(x, y, result);

            if (FAILED(hr)) ThrowHr(hr, "IMultiplier::Mul failed");
            return result;
        }

        double Div(OS12HANDLE h, double x, double y) {
            OS12_Context* ctx = ValidateHandle(h);
            if (!ctx->pIMultiplier) throw runtime_error("Multiplier interface not initialized");

            double result = 0.0;
            HRESULT hr = ctx->pIMultiplier->Div(x, y, result);

            if (FAILED(hr)) ThrowHr(hr, "IMultiplier::Div failed");
            return result;
        }
    }

    void Dispose(OS12HANDLE h) {
        if (h == nullptr) return;

        OS12_Context* ctx = reinterpret_cast<OS12_Context*>(h);

        if (ctx->pIMultiplier) {
            ctx->pIMultiplier->Release();
            ctx->pIMultiplier = nullptr;
        }
        if (ctx->pIAdder) {
            ctx->pIAdder->Release();
            ctx->pIAdder = nullptr;
        }

        if (ctx->coInitialized) {
            CoUninitialize();
            ctx->coInitialized = false;
        }

        delete ctx;
    }
}
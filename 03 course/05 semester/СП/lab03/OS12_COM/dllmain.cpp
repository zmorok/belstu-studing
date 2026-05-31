#include "pch.h"
#include "OS12.h"
#include "MathFactory.h"
#include <objbase.h>
#include <strsafe.h>
#include <new>

HMODULE g_hModule = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
    }
    return TRUE;
}

extern "C" __declspec(dllexport) STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (!IsEqualCLSID(rclsid, CLSID_OS12)) return CLASS_E_CLASSNOTAVAILABLE;
    return CreateMathFactoryInstance(riid, ppv);
}


extern "C" __declspec(dllexport) STDAPI DllCanUnloadNow() {
    return (g_cObjects == 0 && g_cServerLocks == 0) ? S_OK : S_FALSE;
}

static HRESULT SetRegKeyValue(HKEY hRoot, LPCWSTR subKey, LPCWSTR valueName, LPCWSTR valueData) {
    HKEY hKey = nullptr;
    LONG l = RegCreateKeyExW(hRoot, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

    if (l != ERROR_SUCCESS) {
        return HRESULT_FROM_WIN32(l);
    }
    if (valueData) {
        l = RegSetValueExW(hKey, valueName, 0, REG_SZ, (const BYTE*)valueData, (DWORD)((wcslen(valueData) + 1) * sizeof(wchar_t)));
    }

    RegCloseKey(hKey);
    return HRESULT_FROM_WIN32(l);
}


extern "C" __declspec(dllexport) STDAPI DllRegisterServer() {
    wchar_t modulePath[MAX_PATH];

    if (!GetModuleFileNameW(g_hModule, modulePath, ARRAYSIZE(modulePath))) return HRESULT_FROM_WIN32(GetLastError());

    LPOLESTR clsidString = nullptr;

    HRESULT hr = StringFromCLSID(CLSID_OS12, &clsidString);
    if (FAILED(hr)) return hr;
    

    wchar_t keyPath[512];

    hr = StringCchPrintfW(keyPath, ARRAYSIZE(keyPath), L"CLSID\\%s", clsidString);
    if (SUCCEEDED(hr)) hr = SetRegKeyValue(HKEY_CLASSES_ROOT, keyPath, NULL, L"OS12 COM Object");
    if (FAILED(hr)) {
        CoTaskMemFree(clsidString);
        return hr;
    }

    hr = StringCchPrintfW(keyPath, ARRAYSIZE(keyPath), L"CLSID\\%s\\InprocServer32", clsidString);
    if (SUCCEEDED(hr)) hr = SetRegKeyValue(HKEY_CLASSES_ROOT, keyPath, NULL, modulePath);
    if (FAILED(hr)) {
        CoTaskMemFree(clsidString);
        return hr;
    }

    hr = SetRegKeyValue(HKEY_CLASSES_ROOT, keyPath, L"ThreadingModel", L"Both");

    if (FAILED(hr)) {
        CoTaskMemFree(clsidString);
        return hr;
    }

    hr = SetRegKeyValue(HKEY_CLASSES_ROOT, L"OS12_COM.1", NULL, L"OS12 COM Object");
    if (FAILED(hr)) {
        CoTaskMemFree(clsidString);
        return hr;
    }

    hr = SetRegKeyValue(HKEY_CLASSES_ROOT, L"OS12_COM.1\\CLSID", NULL, clsidString);
    if (FAILED(hr)) {
        CoTaskMemFree(clsidString);
        return hr;
    }

    CoTaskMemFree(clsidString);
    return hr;
}

extern "C" __declspec(dllexport) STDAPI DllUnregisterServer() {

    HRESULT hr = S_OK;
    LPOLESTR clsidString = nullptr;

    hr = StringFromCLSID(CLSID_OS12, &clsidString);
    if (FAILED(hr)) {

        return hr;
    }

    wchar_t keyPath[512];
    if (SUCCEEDED(StringCchPrintfW(keyPath, ARRAYSIZE(keyPath), L"CLSID\\%s", clsidString)))
    {

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0600)

        if (RegDeleteTreeW(HKEY_CLASSES_ROOT, keyPath) != ERROR_SUCCESS)
            hr = S_FALSE;
#else
        wchar_t inprocPath[512];
        if (SUCCEEDED(StringCchPrintfW(inprocPath, ARRAYSIZE(inprocPath), L"%s\\InprocServer32", keyPath)))
        {
            RegDeleteKeyW(HKEY_CLASSES_ROOT, inprocPath);
        }

        RegDeleteKeyW(HKEY_CLASSES_ROOT, keyPath);
#endif
    }

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0600)

    if (RegDeleteTreeW(HKEY_CLASSES_ROOT, L"OS12_COM.1") != ERROR_SUCCESS) {
        hr = S_FALSE;
    }

#else

    RegDeleteKeyW(HKEY_CLASSES_ROOT, L"OS12_COM.1\\CLSID");
    RegDeleteKeyW(HKEY_CLASSES_ROOT, L"OS12_COM.1");

#endif

    CoTaskMemFree(clsidString);
    return hr;

}

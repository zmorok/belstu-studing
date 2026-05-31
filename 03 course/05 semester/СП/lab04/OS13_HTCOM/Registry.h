#pragma once

#include <windows.h>
#include <objbase.h>

HRESULT RegisterServer(HMODULE hModule,
	const CLSID& clsid,
	const WCHAR* szFriendlyName,
	const WCHAR* szVerIndProgID,
	const WCHAR* szProgID);

HRESULT UnregisterServer(const CLSID& clsid,
	const WCHAR* szVerIndProgID,
	const WCHAR* szProgID);


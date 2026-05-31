#pragma once
#include <Windows.h>
#include <Unknwn.h>


HRESULT CreateMathFactoryInstance(REFIID riid, void** ppv);
#pragma once
#include <objbase.h>
#include <Unknwn.h>
#include <Windows.h>
	
// {B02A5BC9-40C6-401B-9D2B-2B5E2549B3CE}
static const GUID IID_IAdder =
{ 0xb02a5bc9, 0x40c6, 0x401b, { 0x9d, 0x2b, 0x2b, 0x5e, 0x25, 0x49, 0xb3, 0xce } };

// {3D4BFE2B-79DC-4A56-8252-A19A5DF5C7B5}
static const GUID IID_IMultiplier =
{ 0x3d4bfe2b, 0x79dc, 0x4a56, { 0x82, 0x52, 0xa1, 0x9a, 0x5d, 0xf5, 0xc7, 0xb5 } };

// {6029CFA5-5C78-4EEA-912E-3A3333A90B96}
static const GUID CLSID_OS12 =
{ 0x6029cfa5, 0x5c78, 0x4eea, { 0x91, 0x2e, 0x3a, 0x33, 0x33, 0xa9, 0xb, 0x96 } };

__interface IAdder :public IUnknown {
	
	virtual HRESULT __stdcall Add(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Sub(const double x, const double y, double& z) = 0;
};

__interface IMultiplier :public IUnknown {
	virtual HRESULT __stdcall Mul(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Div(const double x, const double y, double& z) = 0;
};

HRESULT CreateOS12Instance(REFIID riid, void** ppv);

extern LONG g_cObjects;
extern LONG g_cServerLocks;
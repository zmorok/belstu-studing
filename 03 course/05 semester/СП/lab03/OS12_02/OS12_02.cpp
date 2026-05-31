#include <iostream>
#include "../OS12_COM/OS12.h"
#define IERR(s) std::cout<<"Error: "<<s<<std::endl;
#define IRES(s,r) std::cout<<s<<r<<std::endl;

IAdder* pIAdder = nullptr;
IMultiplier* pIMultiplier = nullptr;

int main() {
	IUnknown* pIUnknown = NULL;
	CoInitialize(NULL);

	HRESULT hr0 = CoCreateInstance(CLSID_OS12, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);

	if (SUCCEEDED(hr0)) {
		std::cout << "CoCreateInstsance succeeded" << std::endl;

		if (SUCCEEDED(pIUnknown->QueryInterface(IID_IAdder, (void**)&pIAdder))) {
			{
				double z = 0.0;
				if (!SUCCEEDED(pIAdder->Add(2.0, 3.0, z))) { IERR("IAdder::Add"); }
				else IRES("IAdder::Add = ", z);
			}
			{
				double z = 0.0;
				if (!SUCCEEDED(pIAdder->Sub(2.0, 3.0, z))) { IERR("IAdder::Sub"); }
				else IRES("IAdder::Sub = ", z);
			}

			if (SUCCEEDED(pIAdder->QueryInterface(IID_IMultiplier,(void**)&pIMultiplier))) {
				{
					double z = 0.0;
					if (!SUCCEEDED(pIMultiplier->Mul(2.0, 3.0, z))) { IERR("IMultiplier::Mul"); }
					else IRES("IMultiplier::Mul = ", z);
				}
				{
					double z = 0.0;
					if (!SUCCEEDED(pIMultiplier->Div(2.0, 3.0, z))) { IERR("IMultiplier::Mul"); }
					else IRES("IMultiplier::Mul = ", z);
				}
				if (SUCCEEDED(pIMultiplier->QueryInterface(IID_IAdder, (void**)&pIAdder))) {
					double z = 0.0;
					if (!SUCCEEDED(pIAdder->Add(2.0, 3.0, z))) { IERR("IAdder::Add"); }
					else IRES("IAdder::Add = ", z);
					
					pIAdder->Release();
				}
				else IERR("IMultiplier->IAdder");
				pIMultiplier->Release();
			}
		}
		else IERR("IAdder");
	}
	else std::cout << "CoCreateInstance error" << std::endl;
	pIUnknown->Release();
	CoFreeUnusedLibraries();
	return 0;
}

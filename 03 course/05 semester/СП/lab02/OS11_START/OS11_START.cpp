#define _CRT_SECURE_NO_WARNINGS            
#include <iostream>
#include <string>
#include <windows.h>
#include "../OS11_HTAPI/OS11_HTAPI.h"       
#pragma comment(lib,"..\\x64\\Debug\\OS11_HTAPI.lib")

using namespace std;                      

static string GetExeDirectory();

int main(int argc, char* argv[])
{
	char* fileName;

	string directoryPath = GetExeDirectory();
	string filePath = string(directoryPath) + "HT_n12001.ht";

	try
	{
		if (argv[1])
		{
			fileName = argv[1];
			filePath = string(directoryPath) + fileName;
		}


		HT::HTHANDLE* HT;
		cout << "Trying to open HT-Storage..." << filePath <<  endl;
		HT = HT::Open(filePath.c_str());
		if (HT == NULL) throw "Invalid handle";

		cout << "HT-Storage STARTED " << endl;
		cout << "Filename: " << filePath << endl;
		cout << "SnapshotIntervalSec: " << HT->SecSnapshotInterval << endl;
		cout << "Capacity: " << HT->Capacity << endl;
		cout << "MaxKeyLength: " << HT->MaxKeyLength << endl;
		cout << "MaxPayloadLength: " << HT->MaxPayloadLength << endl;

		int b;
		cin >> b;                                     
		HT::Close(HT);
	}
	catch (const char* err)
	{
		cout << err << endl; return 1;                                      
	}

	return 0;                                           
}

// функция получения пути к директории исполняемого файла
static string GetExeDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	string path(buffer);
	return path.substr(0, path.find_last_of("\\/") + 1);
}
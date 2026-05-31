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
	int capacity = 2000;
	int snapshotIntervalSec = 3;
	int maxKeyLength = 4;
	int maxPayloadLength = 4;
	char* fileName = nullptr;

	string directoryPath = GetExeDirectory();
	string filePath = string(directoryPath) + "HT_n12001.ht";

	try
	{
		if (argv[1] && argv[2] && argv[3] && argv[4] && argv[5])
		{
			capacity = stoi(argv[2]);
			snapshotIntervalSec = stoi(argv[3]);
			maxKeyLength = stoi(argv[4]);
			maxPayloadLength = stoi(argv[5]);

			fileName = argv[1];
			filePath = string(directoryPath) + fileName;
		}
		
		HT::HTHANDLE* HT;
		HT = HT::Create(capacity, snapshotIntervalSec, maxKeyLength, maxPayloadLength, filePath.c_str());
		if (HT == NULL)	throw "Invalid handle";  
		
		try {

		HT::Close(HT);
		}
		catch (string er) { cout << "closing ex: " << er << endl; return 1; }
		


		cout << "HT-Storage CREATED " << endl << flush;
		cout << "Filepath: " << filePath << "\n";
		cout << "\nSnapshotIntervalSec: " << snapshotIntervalSec << endl;
		cout << "Capacity: " << capacity << endl;
		cout << "MaxKeyLength: " << maxKeyLength << endl;
		cout << "MaxPayloadLength: " << maxPayloadLength << endl;
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
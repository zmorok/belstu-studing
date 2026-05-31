#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include "../OS13_HTCOM_LIB/HT.h"

#pragma comment(lib, "OS13_HTCOM_LIB.lib")

using namespace std;

static string GetExeDirectory()
{
	char path[MAX_PATH];
	DWORD len = GetModuleFileNameA(NULL, path, MAX_PATH);
	if (len == 0 || len == MAX_PATH) return "";
	string fullPath(path);
	size_t pos = fullPath.find_last_of("\\/");
	if (pos == string::npos) return "";
	return fullPath.substr(0, pos);
}

int main(int argc, char* argv[]) {

	string nameHT = "HT2.ht";

	string filename;
	int capacity = 0;
	int snapshotinterval = 0;
	int maxkeylength = 0;
	int maxdatalength = 0;

	if (argc != 6) {
		capacity = 1000;
		snapshotinterval = 5;
		maxkeylength = 10;
		maxdatalength = 200;

		string exeDir = GetExeDirectory();
		if (!exeDir.empty()) cout << "Executable directory: " << exeDir << endl;
		else cout << "Executable directory: <failed to determine>" << endl;

		filename = exeDir.empty() ? "HT.ht" : (exeDir + "\\" + nameHT);
	}
	else {
		filename = argv[1];
		capacity = atoi(argv[2]);
		snapshotinterval = atoi(argv[3]);
		maxkeylength = atoi(argv[4]);
		maxdatalength = atoi(argv[5]);
	}


	if (capacity <= 0 || snapshotinterval < 0 || maxkeylength <= 0 || maxdatalength <= 0) {
		cout << "Error: Invalid parameters. All parameters must be positive integers." << endl;
		return 1;
	}

	HT::HTHANDLE* ht = HT::Create(capacity, snapshotinterval, maxkeylength, maxdatalength, filename.c_str());
	
	if (ht == nullptr) {
		cout << "Error: Failed to create HT storage." << endl;
		cout << "Last Error: " << HT::GetLastErrorMessage(ht) << endl;
		return 1;
	}

	cout << "HT-Storage Created filename=" << filename
		<< ", snapshotinterval=" << snapshotinterval 
		<< ", capacity=" << capacity 
		<< ", maxkeylength=" << maxkeylength 
		<< ", maxdatalength=" << maxdatalength << endl;

	HT::Close(ht);
	return 0;
}
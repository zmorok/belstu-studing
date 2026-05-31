#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include "../OS13_HTCOM_LIB/HT.h"

#pragma comment(lib, "OS13_HTCOM_LIB.lib")

using namespace std;

// Создание имени события на основе имени файла
static string GetStartEventName(const char* filename) {
	stringstream ss;
	ss << "Global\\OS13_START_Event_";
	string fname = filename;
	for (size_t i = 0; i < fname.length(); i++) {
		if (fname[i] == '\\' || fname[i] == '/' || fname[i] == ':') {
			fname[i] = '_';
		}
	}
	ss << fname;
	return ss.str();
}

static string GetStopEventName(const char* filename) {
	stringstream ss;
	ss << "Global\\OS13_STOP_Event_";
	string fname = filename;
	for (size_t i = 0; i < fname.length(); i++) {
		if (fname[i] == '\\' || fname[i] == '/' || fname[i] == ':') {
			fname[i] = '_';
		}
	}
	ss << fname;
	return ss.str();
}

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

	string nameHT = "HT.ht";

	string filename;

	if (argc != 2) {
		string exeDir = GetExeDirectory();
		if (!exeDir.empty()) cout << "Executable directory: " << exeDir << endl;
		else cout << "Executable directory: <failed to determine>" << endl;

		filename = exeDir.empty() ? "HT.ht" : (exeDir + "\\" + nameHT);
	}
	else {
		filename = argv[1];
	}

	const string startEventName = GetStartEventName(filename.c_str());
	HANDLE hStartEvent = OpenEventA(SYNCHRONIZE, FALSE, startEventName.c_str());
	if (hStartEvent) {
		DWORD state = WaitForSingleObject(hStartEvent, 0);
		if (state == WAIT_OBJECT_0) {
			const string stopEventName = GetStopEventName(filename.c_str());
			HANDLE hStopEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, stopEventName.c_str());
			if (!hStopEvent) {
				hStopEvent = CreateEventA(NULL, FALSE, FALSE, stopEventName.c_str());
			}

			if (hStopEvent) {
				cout << "HT-Storage Stop: requesting OS13_START shutdown for filename=" << filename << endl;
				SetEvent(hStopEvent);
				CloseHandle(hStopEvent);

				// Ждем, пока OS13_START сбросит Start-событие (клиенты перейдут в ожидание)
				const DWORD maxWaitMs = 5000;
				const DWORD startTick = GetTickCount();
				while (GetTickCount() - startTick < maxWaitMs) {
					DWORD s = WaitForSingleObject(hStartEvent, 0);
					if (s == WAIT_TIMEOUT) {
						cout << "OS13_START stopped (event reset). Clients are waiting." << endl;
						CloseHandle(hStartEvent);
						return 0;
					}
					Sleep(50);
				}

				cout << "Warning: stop requested, but OS13_START did not reset event within timeout." << endl;
				CloseHandle(hStartEvent);
				return 0;
			}

			cout << "Warning: failed to open/create stop-request event; falling back to snapshot." << endl;
		}
		CloseHandle(hStartEvent);
	}

	HT::HTHANDLE* ht = HT::Open(filename.c_str());
	if (ht == nullptr) {
		cout << "Error: Failed to open HT storage." << endl;
		cout << "Last Error: " << HT::GetLastErrorMessage(ht) << endl;
		return 1;
	}

	cout << "HT-Storage Stop filename=" << filename << endl;

	if (ht->Mutex) {
		WaitForSingleObject(ht->Mutex, INFINITE);
		HT::Snap(ht);
		ReleaseMutex(ht->Mutex);
	} else {
		HT::Snap(ht);
	}

	cout << "Snapshot completed." << endl;

	HT::Close(ht);

	cout << "HT-Storage access stopped. Other applications cannot access the storage until it is opened again." << endl;

	return 0;
}

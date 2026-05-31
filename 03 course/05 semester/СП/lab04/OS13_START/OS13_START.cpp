#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "../OS13_HTCOM_LIB/HT.h"

#pragma comment(lib, "OS13_HTCOM_LIB.lib")

using namespace std;

// Создание имени события на основе имени файла
string GetEventName(const char* filename) {
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

static string GetSafeName(const string& filePath) {
	size_t pos = filePath.find_last_of("\\/");
	return (pos == string::npos) ? filePath : filePath.substr(pos + 1);
}

static string GetMutexName(const string& filePath) {
	stringstream ss;
	ss << "HT_MUTEX_" << GetSafeName(filePath);
	return ss.str();
}

static string GetMappingName(const string& filePath) {
	stringstream ss;
	ss << "HT_MAPPING_" << GetSafeName(filePath);
	return ss.str();
}

struct InternalElement {
	int keylength;
	int payloadlength;
};

static string FormatLocalTime(time_t t) {
	tm timeinfo = {};
	localtime_s(&timeinfo, &t);
	stringstream ss;
	ss << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

static void PrintStats(const HTFileHeader& header, const char* baseAddr, const HT::HTHANDLE* ht, time_t startTime) {
	HANDLE hMutex = (ht != nullptr) ? ht->Mutex : NULL;
	if (hMutex) {
		WaitForSingleObject(hMutex, INFINITE);
	}

	int used = -1;
	const bool hasMapping = (baseAddr != nullptr && header.Capacity > 0);
	if (hasMapping) {
		used = 0;
		const size_t elementSize = sizeof(InternalElement) + (size_t)header.MaxKeyLength + (size_t)header.MaxPayloadLength;
		const size_t offset = sizeof(HTFileHeader);
		for (int i = 0; i < header.Capacity; ++i) {
			const InternalElement* el = reinterpret_cast<const InternalElement*>(baseAddr + offset + elementSize * (size_t)i);
			if (el->keylength > 0) ++used;
		}
	}

	const time_t lastSnapTime = (ht != nullptr) ? ht->lastsnaptime : 0;

	if (hMutex) {
		ReleaseMutex(hMutex);
	}

	const int freeSlots = (used >= 0) ? (header.Capacity - used) : -1;
	const double fillPercent = (used >= 0 && header.Capacity > 0) ? (100.0 * used / header.Capacity) : 0.0;

	time_t now = time(nullptr);
	cout << "[STATS " << FormatLocalTime(now) << "] "
		<< "uptime=" << (now - startTime) << "s, "
		<< "capacity=" << header.Capacity << ", ";

	if (used >= 0) {
		cout << "used=" << used
			<< " (" << fixed << setprecision(1) << fillPercent << "%), "
			<< "free=" << freeSlots;
	} else {
		cout << "used=n/a, free=n/a";
	}
	if (lastSnapTime != 0) {
		cout << ", lastsnap=" << FormatLocalTime(lastSnapTime);
	}
	cout << endl;
}

// Функция потока для периодического сохранения
DWORD WINAPI SnapshotThreadFunc(LPVOID p) {
	HT::HTHANDLE* ht = (HT::HTHANDLE*)p;
	if (ht->SecSnapshotInterval <= 0) return 0;

	while (ht->isRunning) {
		for (int i = 0; i < ht->SecSnapshotInterval * 10 && ht->isRunning; ++i) {
			Sleep(100);
		}
		if (!ht->isRunning) break;

		if (ht->Mutex) WaitForSingleObject(ht->Mutex, INFINITE);
		if (HT::Snap(ht)) {
			ht->lastsnaptime = time(nullptr);
		}
		if (ht->Mutex) {
			ReleaseMutex(ht->Mutex);
		}
	}
	return 0;
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

	// Открытие или создание именованного события для синхронизации с клиентскими процессами
	string eventName = GetEventName(filename.c_str());
	HANDLE hEvent = OpenEventA(
		EVENT_MODIFY_STATE | SYNCHRONIZE,	// Доступ для изменения состояния и ожидания
		FALSE,                              // Наследование не требуется
		eventName.c_str()                   // Имя события
	);

	// Если такого события нет, то создаётся новое
	if (hEvent == NULL) {
		hEvent = CreateEventA(
			NULL,           // Атрибуты безопасности (NULL = наследование)
			TRUE,           // Manual reset (TRUE = ручной сброс)
			TRUE,           // Initial state (TRUE = сигнальное состояние)
			eventName.c_str() // Имя события
		);

		if (hEvent == NULL) {
			cout << "Error: Failed to create synchronization event" << endl;
			return 1;
		}
		cout << "Created synchronization event: " << eventName << endl;
	} else {
		// Событие существует, устанавливается оно в сигнальное состояние
		SetEvent(hEvent);
		cout << "Opened and signaled existing synchronization event: " << eventName << endl;
	}

	// Событие запроса остановки (используется OS13_STOP)
	string stopEventName = GetStopEventName(filename.c_str());
	HANDLE hStopEvent = CreateEventA(NULL, FALSE, FALSE, stopEventName.c_str());
	if (hStopEvent == NULL) {
		cout << "Error: Failed to create stop-request event" << endl;
		ResetEvent(hEvent);
		CloseHandle(hEvent);
		return 1;
	}
	ResetEvent(hStopEvent);

	// Чтение заголовка
	HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Error: Failed to open file " << filename << endl;
		ResetEvent(hEvent);
		CloseHandle(hEvent);
		CloseHandle(hStopEvent);
		return 1;
	}

	HTFileHeader header;
	DWORD bytesRead;
	if (!ReadFile(hFile, &header, sizeof(header), &bytesRead, NULL) || bytesRead != sizeof(header)) {
		CloseHandle(hFile);
		ResetEvent(hEvent);
		CloseHandle(hEvent);
		CloseHandle(hStopEvent);
		cout << "Error: Failed to read file header" << endl;
		return 1;
	}
	CloseHandle(hFile);

	HT::HTHANDLE* ht = HT::Open(filename.c_str());
	if (ht == nullptr) {
		cout << "Error: Failed to open HT storage." << endl;
		cout << "Last Error: " << HT::GetLastErrorMessage(ht) << endl;
		ResetEvent(hEvent);
		CloseHandle(hEvent);
		CloseHandle(hStopEvent);
		return 1;
	}

	// Заполняем поля дескриптора, чтобы корректно работали snapshot/статистика
	ht->Capacity = header.Capacity;
	ht->SecSnapshotInterval = header.SecSnapshotInterval;
	ht->MaxKeyLength = header.MaxKeyLength;
	ht->MaxPayloadLength = header.MaxPayloadLength;

	// Открываем/создаем именованный мьютекс и отображение файла для статистики
	const string mutexName = GetMutexName(filename);
	ht->Mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, mutexName.c_str());
	if (!ht->Mutex) {
		ht->Mutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
	}

	const string mappingName = GetMappingName(filename);
	HANDLE hMapping = OpenFileMappingA(FILE_MAP_READ, FALSE, mappingName.c_str());
	if (!hMapping) {
		HANDLE hFileForMap = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFileForMap != INVALID_HANDLE_VALUE) {
			hMapping = CreateFileMappingA(hFileForMap, NULL, PAGE_READONLY, 0, 0, mappingName.c_str());
			CloseHandle(hFileForMap);
		}
	}

	const char* mappedAddr = nullptr;
	if (hMapping) {
		mappedAddr = reinterpret_cast<const char*>(MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0));
	}

	cout << "HT-Storage Start filename=" << filename
		<< ", snapshotinterval=" << header.SecSnapshotInterval
		<< ", capacity=" << header.Capacity
		<< ", maxkeylength=" << header.MaxKeyLength
		<< ", maxdatalength=" << header.MaxPayloadLength << endl;
	cout << "HT-Storage is running. Client processes can now access the storage." << endl;
	cout << "Press any key or run OS13_STOP to stop..." << endl;

	// Поток для периодического сохранения
	ht->isRunning = TRUE;
	DWORD threadId;
	HANDLE snapshotThread = CreateThread(NULL, 0, SnapshotThreadFunc, ht, 0, &threadId);
	if (snapshotThread == NULL) {
		cout << "Error: Failed to create snapshot thread" << endl;
		ResetEvent(hEvent);

		if (mappedAddr) {
			UnmapViewOfFile(mappedAddr);
		}
		if (hMapping) {
			CloseHandle(hMapping);
		}

		HANDLE hMutex = ht->Mutex;
		ht->Mutex = NULL;
		HT::Close(ht);
		if (hMutex) {
			CloseHandle(hMutex);
		}

		CloseHandle(hStopEvent);
		return 1;
	}

	time_t startTime = time(nullptr);
	ht->lastsnaptime = 0;

	const int statsIntervalSec = (header.SecSnapshotInterval > 0) ? header.SecSnapshotInterval : 5;
	auto nextStatsAt = chrono::steady_clock::now() + chrono::seconds(statsIntervalSec);

	// Первичный вывод
	PrintStats(header, mappedAddr, ht, startTime);

	while (true) {
		if (_kbhit()) {
			_getch();
			break;
		}
		if (WaitForSingleObject(hStopEvent, 0) == WAIT_OBJECT_0) {
			cout << "Stop requested by OS13_STOP." << endl;
			break;
		}

		auto now = chrono::steady_clock::now();
		if (now >= nextStatsAt) {
			PrintStats(header, mappedAddr, ht, startTime);
			nextStatsAt = now + chrono::seconds(statsIntervalSec);
		}

		Sleep(50);
	}

	cout << "Stopping HT-Storage..." << endl;

	// Сброс события
	ResetEvent(hEvent);
	cout << "Synchronization event reset. Client processes are now paused." << endl;

	// Остановка потока сохранения
	ht->isRunning = FALSE;
	WaitForSingleObject(snapshotThread, INFINITE);
	CloseHandle(snapshotThread);

	// Финальное сохранение
	if (ht->Mutex) {
		WaitForSingleObject(ht->Mutex, INFINITE);
		HT::Snap(ht);
		ht->lastsnaptime = time(nullptr);
		ReleaseMutex(ht->Mutex);
	}

	HANDLE hMutex = ht->Mutex;
	ht->Mutex = NULL;
	HT::Close(ht);

	if (mappedAddr) {
		UnmapViewOfFile(mappedAddr);
	}
	if (hMapping) {
		CloseHandle(hMapping);
	}
	if (hMutex) {
		CloseHandle(hMutex);
	}

	CloseHandle(hStopEvent);
	// CloseHandle(hEvent);  // Событие будет автоматически закрыто системой, когда все процессы закроют свои дескрипторы
	cout << "HT-Storage stopped. Event remains for client processes." << endl;
	return 0;
}

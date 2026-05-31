#include <iostream>
#include <Windows.h>
#include <string>
#include <random>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "../OS13_HTCOM_LIB/HT.h"

#pragma comment(lib, "OS13_HTCOM_LIB.lib")

using namespace std;

// Глобальные переменные для корректного завершения
volatile bool g_running = true;
HT::HTHANDLE* g_ht = nullptr;
HANDLE g_hStartEvent = NULL; // Дескриптор события OS13_START

// Функция для создания имени события на основе имени файла
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

// Открытие события OS13_START
bool OpenStartEvent(const char* filename) {
	string eventName = GetEventName(filename);
	
	g_hStartEvent = OpenEventA(
		SYNCHRONIZE,  // Доступ для ожидания
		FALSE,        // Наследование не требуется
		eventName.c_str() // Имя события
	);

	if (g_hStartEvent == NULL) {
		// Событие еще не создано, ожидание
		while (g_hStartEvent == NULL && g_running) {
			Sleep(100);
			g_hStartEvent = OpenEventA(SYNCHRONIZE, FALSE, eventName.c_str());
		}
	}

	return (g_hStartEvent != NULL);
}

// Функция для ожидания события OS13_START
void WaitForStartEvent(const char* filename)
{
	if (g_hStartEvent == NULL) if (!OpenStartEvent(filename)) return;

	DWORD r = WaitForSingleObject(g_hStartEvent, 0);
	if (r == WAIT_OBJECT_0) return;

	cout << "[OS_0403] Ожидаем..." << endl;
	while (g_running)
	{
		r = WaitForSingleObject(g_hStartEvent, INFINITE);
		if (r == WAIT_OBJECT_0) return;
		if (r == WAIT_FAILED) {
			CloseHandle(g_hStartEvent);
			g_hStartEvent = NULL;
			if (!OpenStartEvent(filename)) return;
			continue;
		}
		return;
	}
}

// Обработчик сигнала Ctrl+C
BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) {
	if (dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_BREAK_EVENT || dwCtrlType == CTRL_CLOSE_EVENT) {
		cout << "\n[OS_0403] Получен сигнал завершения. Завершение работы..." << endl;
		g_running = false;
		return TRUE;
	}
	return FALSE;
}

// Функция для протоколирования
void LogOperation(const string& operation, const string& key, bool success, const string& errorMsg = "") {
	time_t now = time(nullptr);
	tm timeinfo = {};
	localtime_s(&timeinfo, &now);
	
	stringstream ss;
	ss << "[" << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "] ";
	ss << "[OS_0403] " << operation;
	if (!key.empty()) {
		ss << " key=" << key;
	}
	ss << " result=" << (success ? "SUCCESS" : "FAILED");
	if (!success && !errorMsg.empty()) {
		ss << " error=" << errorMsg;
	}
	ss << endl;
	
	cout << ss.str();
}

// Генерация случайного ключа из 50 вариантов
string GenerateRandomKey(int index) {
	stringstream ss;
	ss << "k";
	ss << setfill('0') << setw(2) << index;
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

	if (!SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE)) {
		cout << "[OS_0403] Ошибка: не удалось установить обработчик Ctrl+C" << endl;
		return 1;
	}

	cout << "[OS_0403] Запуск приложения..." << endl;
	cout << "[OS_0403] Подключение к HT-хранилищу: " << filename << endl;

	// Открытие события синхронизации с OS13_START
	if (!OpenStartEvent(filename.c_str())) {
		cout << "[OS_0403] Ошибка: не удалось открыть событие синхронизации" << endl;
		cout << "[OS_0403] Убедитесь, что OS13_START запущен для этого файла" << endl;
		return 1;
	}

	// Ожидание запуска
	cout << "[OS_0403] Ожидание запуска OS13_START..." << endl;
	WaitForStartEvent(filename.c_str());
	cout << "[OS_0403] OS13_START обнаружен. Подключение к HT-хранилищу..." << endl;

	// Открытие HT-хранилища
	g_ht = HT::Open(filename.c_str());
	if (g_ht == nullptr) {
		cout << "[OS_0403] Ошибка: не удалось открыть HT-хранилище" << endl;
		cout << "[OS_0403] Убедитесь, что OS13_START запущен для этого файла" << endl;
		CloseHandle(g_hStartEvent);
		return 1;
	}

	LogOperation("Open", "", true);
	cout << "[OS_0403] Успешно подключено к HT-хранилищу" << endl;
	cout << "[OS_0403] Начало работы. Для остановки нажмите Ctrl+C" << endl;

	// Генератор случайных чисел
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 49);

	int operationCount = 0;
	int successCount = 0;
	int failCount = 0;
	int notFoundCount = 0;

	while (g_running) {
		WaitForStartEvent(filename.c_str());

		int keyIndex = dis(gen);
		string key = GenerateRandomKey(keyIndex);
		HT::Element* element = new HT::Element(key.c_str(), (int)key.length());
		HT::Element* existingElement = HT::Get(g_ht, element);
		if (existingElement != nullptr) {
			bool success = HT::Delete(g_ht, element);
			
			if (success) {
				successCount++;
				LogOperation("Delete", key, true);
			} else {
				failCount++;
				string errorMsg = HT::GetLastErrorMessage(g_ht);
				LogOperation("Delete", key, false, errorMsg);
			}
			
			delete existingElement;
		} else {
			notFoundCount++;
			LogOperation("Delete", key, false, "Element not found");
		}
		
		delete element;
		operationCount++;

		Sleep(1000);
	}

	cout << "\n[OS_0403] Завершение работы..." << endl;
	cout << "[OS_0403] Статистика:" << endl;
	cout << "  Всего операций: " << operationCount << endl;
	cout << "  Успешных удалений: " << successCount << endl;
	cout << "  Неудачных удалений: " << failCount << endl;
	cout << "  Элементы не найдены: " << notFoundCount << endl;

	if (g_ht != nullptr) {
		if (HT::Close(g_ht)) LogOperation("Close", "", true);
		else LogOperation("Close", "", false, HT::GetLastErrorMessage(g_ht));
		g_ht = nullptr;
	}

	if (g_hStartEvent != NULL) {
		CloseHandle(g_hStartEvent);
		g_hStartEvent = NULL;
	}

	cout << "[OS_0403] Приложение завершено." << endl;
	return 0;
}
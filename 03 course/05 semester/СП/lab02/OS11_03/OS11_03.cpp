#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <windows.h>
#include "../OS11_HTAPI/OS11_HTAPI.h"
#pragma comment(lib,"..\\x64\\Debug\\OS11_HTAPI.lib")

using namespace std;

static void logger(const string& level, const string& message);
static void print_error(char* str);
static string GetExeDirectory();

int main(int argc, char* argv[])
{
	//srand(time(0));
	random_device rand_device;
	mt19937 rand(rand_device());
	logger("INFO", "Программа удаления элементов запущена.");
 
	char* fileName;

	const char* directoryPath = "D:\\Programming\\SP\\lab02\\";
	string filePath = string(directoryPath) + "HT_n12000.ht";

	try
	{
		if (argv[1])
		{
			fileName = argv[1];
			filePath = string(directoryPath) + fileName;
		}

		cout << "filename: " << filePath << endl;
		logger("INFO", "Попытка открыть хэш-таблицу: " + string("test.ht"));

		HT::HTHANDLE* HT = nullptr;

		for (;;)
		{
			HANDLE mutex = OpenMutexA(SYNCHRONIZE, FALSE, "mutex");
			if (!mutex) {
				cout << "OpenMutexA FAILED, err=" << GetLastError() << "Waiting for ht....\n";
				logger("ERROR", "Не удалось открыть мьютекс для хэш-таблицы");

				if (HT) {
					HT::CloseExistHt(HT);
					HT = nullptr;
					logger("INFO", "Хэш-таблица закрыта после ошибки открытия мьютекса");
				}

				Sleep(3000);
				continue;
			}
			if (!HT) {
				HT = HT::OpenExistHt(filePath.c_str());
				cout << HT;
				if (HT == NULL)
				{
					logger("ERROR", "Не удалось открыть хэш-таблицу (Invalid handle)");
					throw "Invalid handle";
				}
				else logger("INFO", "Хэш-таблица успешно открыта");
			}
			CloseHandle(mutex);


			int numberKey = rand() % 50 + 1;
			string key;
			stringstream convert;
			convert << numberKey;
			key = convert.str();
			cout << key << ' ' << key.length() << endl;

			logger("INFO", "Попытка удалить элемент с ключом: " + key);

			HT::Element* el;
			HT::Element* el1 = new HT::Element(key.c_str(), key.length() + 1, "0", 2);

			if ((el = HT::Get(HT, el1)) == NULL)
			{
				print_error(HT::GetLastErrorHt(HT));
				logger("WARNING", "Элемент с ключом " + key + " не найден.");
			}
			else
			{
				HT::print(el);

				if (!HT::Delete(HT, el1))
				{
					print_error(HT::GetLastErrorHt(HT));
					logger("ERROR", "Не удалось удалить элемент с ключом " + key);
				}
				else
				{
					cout << "DELETE SUCCESS" << endl;
					logger("INFO", "Элемент успешно удалён: " + key);
				}
			}

			Sleep(1000);
		}

		int b;
		cin >> b;
	}
	catch (const char* err)
	{
		cout << err << endl;
		logger("ERROR", string("Исключение: ") + err);
		return 1;
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

// функция записи сообщений в лог-файл "program.log" и вывода их на экран
static void logger(const string& level, const string& message)
{
	static mutex logMutex;
	static ofstream logFile("program.log", ios::app);

	lock_guard<mutex> lock(logMutex);

	auto now = chrono::system_clock::now();
	time_t now_time = chrono::system_clock::to_time_t(now);
	tm local_tm;
	localtime_s(&local_tm, &now_time);

	char timeStr[64];	// "YYYY-MM-DD HH:MM:SS"
	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &local_tm);

	string logLine = "[" + string(timeStr) + "] [" + level + "] " + message;
	cout << logLine << endl;
	if (logFile.is_open()) {
		logFile << logLine << endl; logFile.flush();

	}
}

// функция вывода строки ошибки на экран и записи её в лог
static void print_error(char* str)
{
	cout << "ERROR:\t";
	int i = 0;
	while (str[i]) { cout << str[i]; i++; }
	cout << endl;
	logger("ERROR", str);
}
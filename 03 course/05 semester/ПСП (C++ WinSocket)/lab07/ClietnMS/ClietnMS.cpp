#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.LIB")

#include "SetErrorMsgText.h"
#include "SetupConsole.h"

using namespace std;

int main()
{
    SetupConsoleLocale();

    cout << "[ClientMS] Старт приложения\n";
    const char* MS_NAME = R"(\\*\mailslot\Box)";

    try
    {
		cout << "[ClientMS] Попытка подключения к: " << MS_NAME << endl;

        HANDLE hMailslot;
        if ((hMailslot = CreateFileA(
            MS_NAME,
            GENERIC_WRITE,
            FILE_SHARE_READ,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            0
        )) == INVALID_HANDLE_VALUE) throw SetErrorMsgText("Ошибка CreateFileA (Подключение к 'Box')", WSAGetLastError());

        string msg = "[ClientMS] Подключен к: " + string(MS_NAME);
        cout << msg << endl;;

        int count = 0;

		cout << "Количество сообщений для отправки: "; cin >> count;
		cout << "Задержка 1сек? (да-1/нет-0): "; bool delay; cin >> delay;
		cout << "[ClientMS] Начало передачи сообщений\n";

        string hi = "Hello from Mailslot-client ";

        int i = 0;
        while (i < count) {
            string tempbuf = hi + to_string(++i);
            DWORD br = 0;
            if (!WriteFile(hMailslot, tempbuf.c_str(), (DWORD)tempbuf.size(), &br, NULL))
                throw SetErrorMsgText("Ошибка WriteFile", WSAGetLastError());

            cout << "[ClientMS] Записано " << br << " байт: " << tempbuf << "\n";
            if (tempbuf == "exit") break;
            if (delay) Sleep(1000);
        }

        if (!CloseHandle(hMailslot)) throw SetErrorMsgText("Ошибка CloseHandle", WSAGetLastError());
    }
    catch (string err)
    {
		cerr << "Код = " << WSAGetLastError() << "\n";
        cerr << "[Client] Exception: " << err << "\n";
        system("pause");
        return 1;
    }

    cout << "[ClientMS] Завершение без ошибок\n";
    return 0;
}
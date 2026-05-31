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

    cout << "[ServerMS] Старт приложения\n";

    const char* MS_NAME = R"(\\.\mailslot\Box)";
    const DWORD MAX_MSG = 500;
    const DWORD READ_TIMEOUT = 3 * 60 * 1000;

    try 
    {
        HANDLE hMailslot;
        if ((hMailslot = CreateMailslotA(MS_NAME, MAX_MSG, READ_TIMEOUT, NULL)) == INVALID_HANDLE_VALUE) throw SetErrorMsgText("Ошибка CreateMailslot", WSAGetLastError());

        char temp[MAX_MSG + 1] = {0};
        memset(temp, 0, sizeof(temp));

        while (true)
        {
            
            DWORD br = 0;
            if (!ReadFile(hMailslot, &temp, MAX_MSG - 1, &br, NULL)) throw SetErrorMsgText("Ошибка ReadFile (Timeout)", WSAGetLastError());
			
            temp[br] = '\0';

            cout << "[ServerMS] Прочитано " << br << " байт: " << temp << "\n";

            if (strcmp(temp, "exit") == 0) break;
            memset(temp, 0, MAX_MSG + 1);
        }
        

        if (!CloseHandle(hMailslot)) throw SetErrorMsgText("Ошибка CloseHandle", WSAGetLastError());
    }
    catch (string err)
    {
		cerr << "Код = " << WSAGetLastError() << "\n";
        cerr << "[Server] Exception: " << err << "\n";
        system("pause");
        return 1;
    }

    cout << "[ServerMS] Завершение без ошибок\n";
    return 0;
}
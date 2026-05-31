#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

static string to_utf8(const wstring& ws) {
    if (ws.empty()) return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), nullptr, 0, nullptr, nullptr);
    string out(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), (LPSTR)out.data(), len, nullptr, nullptr);
    return out;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Снимок процессов
    HANDLE snapProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapProc == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка: CreateToolhelp32Snapshot (process), код = " << GetLastError() << endl;
        return 1;
    }

    // Снимок потоков
    HANDLE snapThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapThread == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка: CreateToolhelp32Snapshot (thread), код = " << GetLastError() << endl;
        CloseHandle(snapProc);
        return 1;
    }

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);

    if (!Process32FirstW(snapProc, &pe)) {
        cerr << "Ошибка: Process32First, код = " << GetLastError() << endl;
        CloseHandle(snapProc);
        CloseHandle(snapThread);
        return 1;
    }

    cout << left << setw(51) << "Имя процесса"
        << setw(10) << "PID"
        << setw(10) << "PPID"
        << "Потоки (TID)" << endl;
    cout << string(100, '-') << endl;

    do {
        string name = to_utf8(pe.szExeFile);
        DWORD pid = pe.th32ProcessID;
        DWORD ppid = pe.th32ParentProcessID;

        cout << left << setw(40) << name
            << setw(10) << pid
            << setw(10) << ppid;

        // Для этого процесса выводим список его потоков
        THREADENTRY32 te{};
        te.dwSize = sizeof(te);
        bool hasThreads = false;

        if (Thread32First(snapThread, &te)) {
            do {
                if (te.th32OwnerProcessID == pid) {
                    if (!hasThreads) {
                        hasThreads = true;
                    }
                    cout << te.th32ThreadID << ' ';
                }
            } while (Thread32Next(snapThread, &te));
        }

        if (!hasThreads) {
            cout << "(нет потоков / нет данных)";
        }

        cout << endl;

    } while (Process32NextW(snapProc, &pe));

    CloseHandle(snapProc);
    CloseHandle(snapThread);
    return 0;
}

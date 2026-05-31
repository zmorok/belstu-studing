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

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка: CreateToolhelp32Snapshot, код = " << GetLastError() << endl;
        return 1;
    }

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);

    if (!Process32FirstW(snap, &pe)) {
        cerr << "Ошибка: Process32First, код = " << GetLastError() << endl;
        CloseHandle(snap);
        return 1;
    }

    cout << left << setw(76) << "Имя процесса" << setw(10) << "PID" << setw(10) << "PPID" << endl;
    cout << string(85, '-') << endl;

    do {
        string name = to_utf8(pe.szExeFile);
        DWORD pid = pe.th32ProcessID;
        DWORD ppid = pe.th32ParentProcessID;
        cout << left << setw(65) << name << setw(10) << pid << setw(10) << ppid << endl;
    } while (Process32NextW(snap, &pe));

    CloseHandle(snap);
    return 0;
}
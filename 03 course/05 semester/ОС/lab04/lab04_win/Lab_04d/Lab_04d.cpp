#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DWORD currentPID = GetCurrentProcessId();
    cout << "Текущий PID процесса: " << currentPID << "\n\n";
    cout << "Список потоков процесса:\n";
    cout << "-------------------------\n";

    HANDLE snapThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapThread == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка: CreateToolhelp32Snapshot (thread), код = " << GetLastError() << endl;
        return 1;
    }

    THREADENTRY32 te{};
    te.dwSize = sizeof(te);

    bool found = false;

    if (Thread32First(snapThread, &te)) {
        do {
            if (te.th32OwnerProcessID == currentPID) { cout << "TID = " << te.th32ThreadID << endl; found = true; }
        } while (Thread32Next(snapThread, &te));
    }

    if (!found) cout << "(Нет потоков / данные не найдены)" << endl;

    CloseHandle(snapThread);
    return 0;
}

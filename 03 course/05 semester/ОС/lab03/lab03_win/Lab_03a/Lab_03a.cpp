#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct ChildRunA {
    string note;
    PROCESS_INFORMATION pi{};
    bool created = false;
    DWORD lastError = 0;
};

static void PrintLastErrorA(const char* where, DWORD code) {
    LPSTR msg = nullptr;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, 0, (LPSTR)&msg, CREATE_NEW_CONSOLE, nullptr);
    cerr << "[ERR] " << where << ": (" << code << ") " << (msg ? msg : "") << "\n";
    if (msg) LocalFree(msg);
}

static string ToShortPathA(const string& longPath) {
    DWORD need = GetShortPathNameA(longPath.c_str(), nullptr, 0);
    if (need == 0) return longPath;
    string out(need, '\0');
    DWORD got = GetShortPathNameA(longPath.c_str(), (LPSTR)out.data(), need);
    if (got == 0) return longPath;
    if (!out.empty() && out.back() == '\0') out.pop_back();
    return out;
}

int main() {

    string childExe = "D:\\Studying\\3 курс\\выполненные\\ОС\\lab03\\lab03_win\\x64\\Debug\\Lab_03x.exe";
    const string iterations = "23";
    
    char exe[] = "D:\\Studying\\3 курс\\выполненные\\ОС\\lab03\\lab03_win\\x64\\Debug\\Lab_03x.exe";
    char separator[] = " ";
    int iters = 23;
    char num[16];
    _itoa(iters, num, 10);

	char* space = (char*)strcat(exe, separator);
	LPSTR fullExe = (LPSTR)strcat(space, num);
    
    vector<ChildRunA> runs;

    // ---------------- Case #1 ----------------
    {
        STARTUPINFOA si{}; si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};
        ChildRunA r; r.note = "Case #1: lpApplicationName=EXE, lpCommandLine=NULL";

        BOOL ok = CreateProcessA(
            fullExe,
            nullptr,        
            nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr,
            &si, &pi
        );
        r.created = (ok == TRUE); r.lastError = ok ? 0 : GetLastError();
        if (!ok) PrintLastErrorA("CreateProcessA (case #1)", r.lastError);
        else { r.pi = pi; cout << "[OK ] Started " << r.note << " (PID=" << pi.dwProcessId << ")\n"; }
        runs.push_back(r);
    }

    // ---------------- Case #2 ----------------
    {
        STARTUPINFOA si{}; si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};
        ChildRunA r; r.note = "Case #2: lpApplicationName=NULL, lpCommandLine=\"<exe> 23 \"";

        BOOL ok = CreateProcessA(
            nullptr,
            fullExe,
            nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr,
            &si, &pi
        );
        r.created = (ok == TRUE);
        r.lastError = ok ? 0 : GetLastError();
        if (!ok) PrintLastErrorA("CreateProcessA (case #2)", r.lastError);
        
        else { r.pi = pi; cout << "[OK ] Started " << r.note << " (PID=" << pi.dwProcessId << ")\n"; }
        runs.push_back(r);
    }

    // ---------------- Case #3 ----------------
    {
        STARTUPINFOA si{}; si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};
        ChildRunA r; r.note = "Case #3: lpApplicationName=EXE, lpCommandLine=\" 10 \"";

        BOOL ok = CreateProcessA(
            childExe.c_str(),
            (LPSTR)iterations.c_str(),
            nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr,
            &si, &pi
        );
        r.created = (ok == TRUE);
        r.lastError = ok ? 0 : GetLastError();
        if (!ok) PrintLastErrorA("CreateProcessA (case #3)", r.lastError);
        else { r.pi = pi; cout << "[OK ] Started " << r.note << " (PID=" << pi.dwProcessId << ")\n"; }
        runs.push_back(r);
    }

	// ожидание завершения всех запущенных процессов
    vector<HANDLE> waitHandles; waitHandles.reserve(runs.size());
    for (auto& r : runs) if (r.created) waitHandles.push_back(r.pi.hProcess);

    if (!waitHandles.empty()) {
        DWORD wr = WaitForMultipleObjects( (DWORD)waitHandles.size(), waitHandles.data(), TRUE, INFINITE );
        if (wr >= WAIT_OBJECT_0 && wr < WAIT_OBJECT_0 + waitHandles.size())
            cout << "[OK ] All child processes finished.\n";
        else
            cerr << "[WRN] WaitForMultipleObjects returned " << wr << "\n";
    }
    else cerr << "[WRN] No child processes were started.\n";
    

	// очистка ресурсов и вывод кодов завершения
    for (auto& r : runs) {
        if (!r.created) {
            cerr << "[FAIL] " << r.note << " (CreateProcess error=" << r.lastError << ")\n";
            continue;
        }
        DWORD ec = 0;
        if (GetExitCodeProcess(r.pi.hProcess, &ec)) cout << "[INFO] " << r.note << " exit code = " << ec << "\n";
        CloseHandle(r.pi.hThread);
        CloseHandle(r.pi.hProcess);
    }

    return 0;
}
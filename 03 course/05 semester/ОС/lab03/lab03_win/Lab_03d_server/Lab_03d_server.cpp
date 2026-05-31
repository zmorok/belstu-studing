#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <process.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct ChildInfo {
    PROCESS_INFORMATION pi{};
    HANDLE hRead = nullptr;   // родитель читает отсюда
    string note;
};

static void PrintLastErrorA(const char* where, DWORD code) {
    LPSTR msg = nullptr;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, 0, (LPSTR)&msg, 0, nullptr);
    cerr << "[ERR] " << where << ": (" << code << ") "
        << (msg ? msg : "") << "\n";
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

struct ReaderParam {
    HANDLE hRead;
    string tag;
};

static unsigned __stdcall ReaderThread(void* p) { // чтение из pipe до eof, вывод в stdout
    ReaderParam* rp = (ReaderParam*)p;
    const DWORD BUF = 4096;
    char buf[BUF];
    DWORD got = 0;
    while (ReadFile(rp->hRead, buf, BUF, &got, nullptr) && got > 0) {
        cout << "[" << rp->tag << "] " << string(buf, buf + got);
        cout.flush();
        got = 0;
    }
    CloseHandle(rp->hRead);
    delete rp;
    return 0;
}

int main(int argc, char* argv[]) {

    if (argc != 4) {
        cerr << "Usage: Lab-03d-server <procCount> <low> <high>\n";
        return 1;
    }

    long procCount = strtol(argv[1], nullptr, NULL);
    long low = strtol(argv[2], nullptr, NULL);
    long high = strtol(argv[3], nullptr, NULL);
    if (procCount <= 0 || low > high || low < 0 || high < 0) {
        cerr << "Invalid arguments\n";
        return 1;
    }

    string path = "D:\\Studying\\3 курс\\выполненные\\ОС\\lab03\\lab03_win\\x64\\Release\\";
    string exe = "Lab_03d_client.exe";
    string clientExe = path + exe;

    clientExe = ToShortPathA(clientExe);

    cout << "Using client exe: " << clientExe << "\n";
    cout << "SERVER PID=" << GetCurrentProcessId()
        << " start, procs=" << procCount
        << " range=[" << low << "," << high << "]\n";

    long total = high - low + 1;
    long base = total / procCount;
    long rest = total % procCount;

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;   // потомку разрешаем наследовать hWrite

    vector<ChildInfo> children;
    children.reserve(procCount);
    vector<HANDLE> threads;

    long curL = low;
    for (long i = 0; i < procCount; ++i) {
        long len = base + (i < rest ? 1 : 0);
        long curH = curL + len - 1;

        HANDLE hRead = nullptr, hWrite = nullptr;
        if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
            PrintLastErrorA("CreatePipe", GetLastError());
            break;
        }

        string clientExeQuoted = "\"" + clientExe + "\"";
        string fullCmd = clientExeQuoted + " " + to_string(curL) + " " + to_string(curH);
        vector<char> cmdline(fullCmd.begin(), fullCmd.end());
        cmdline.push_back('\0');

        STARTUPINFOA si{};
        si.cb = sizeof(si);
		si.dwFlags = STARTF_USESTDHANDLES; // использовать наши потоки ввода-вывода через пайп
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdOutput = hWrite;
        si.hStdError = hWrite;

        PROCESS_INFORMATION pi{};
        BOOL ok = CreateProcessA(
            nullptr,
            cmdline.data(),
            nullptr, nullptr,
            TRUE,
            NULL,
            nullptr, nullptr,
            &si, &pi
        );
        CloseHandle(hWrite);

        ChildInfo ci;
        ci.pi = pi;
        ci.hRead = hRead;
        ci.note = "child#" + to_string(i) +
            " [" + to_string(curL) + "," + to_string(curH) + "]";

        if (!ok) {
            PrintLastErrorA("CreateProcessA(client)", GetLastError());
            CloseHandle(hRead);
        }
        else {
            cout << "[OK ] started " << ci.note
                << " PID=" << pi.dwProcessId << "\n";
            children.push_back(ci);

            ReaderParam* rp = new ReaderParam{ ci.hRead, ci.note };
            uintptr_t th = _beginthreadex(nullptr, 0, &ReaderThread, rp, 0, nullptr);
            if (th) threads.push_back((HANDLE)th);
            else cerr << "[WRN] reader thread create failed\n";
        }

        curL = curH + 1;
    }

    // завершение всех дочерних
    if (!children.empty()) {
        vector<HANDLE> procs;
        procs.reserve(children.size());
        for (auto& c : children) procs.push_back(c.pi.hProcess);
        WaitForMultipleObjects((DWORD)procs.size(), procs.data(), TRUE, INFINITE);
    }

    // завершение потоков-ридеров
    if (!threads.empty()) {
        WaitForMultipleObjects((DWORD)threads.size(), threads.data(), TRUE, INFINITE);
        for (HANDLE t : threads) CloseHandle(t);
    }

    // коды возврата
    for (auto& c : children) {
        DWORD ec = 0;
        if (GetExitCodeProcess(c.pi.hProcess, &ec)) {
            cout << "[INFO] " << c.note << " exit=" << ec << "\n";
        }
        CloseHandle(c.pi.hThread);
        CloseHandle(c.pi.hProcess);
    }

    cout << "SERVER done.\n";
    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define ITERATIONS 90
#define SEM_NAME "Global\\Lab06_Semaphore"

char username[256];
int  nameLen = 0;

static void work(const char* procName, HANDLE hSem)
{
    DWORD pid = GetCurrentProcessId();
    int inInterval = 0;

    for (int i = 0; i <= ITERATIONS; ++i) {
        DWORD tid = GetCurrentThreadId();

        if (i == 30) {
            WaitForSingleObject(hSem, INFINITE);
            inInterval = 1;
        }

        char letter = username[i % nameLen];

        printf("PID=%lu\tTID=%lu\tПроцесс=%s\tИтерация=%d\t%c\n", pid, tid, procName, i, letter);

        Sleep(100);

        if (i == 60 && inInterval) {
            ReleaseSemaphore(hSem, 1, NULL);
            inInterval = 0;
        }
    }

    if (inInterval) ReleaseSemaphore(hSem, 1, NULL);
}

int main(int argc, char* argv[])
{
    DWORD username_len = sizeof(username);
    if (!GetUserNameA(username, &username_len)) { fprintf(stderr, "GetUserNameA failed with error %lu\n", GetLastError()); return 1; }

    nameLen = (int)strlen(username);
    if (nameLen <= 0) { fprintf(stderr, "Username length is 0\n"); return 2; }

    const char* procName = "MAIN";
    int isChild = 0;

    if (argc >= 2) { procName = argv[1]; isChild = 1; }

    HANDLE hSem = CreateSemaphoreA(NULL, 1, 1, SEM_NAME);
    if (hSem == NULL) { fprintf(stderr, "CreateSemaphore failed: %lu\n", GetLastError()); return 3; }

    if (isChild) {
        work(procName, hSem);
        CloseHandle(hSem);
        return 0;
    }

    char exePath[MAX_PATH];
    if (!GetModuleFileNameA(NULL, exePath, sizeof(exePath))) {
        fprintf(stderr, "GetModuleFileName failed: %lu\n", GetLastError());
        CloseHandle(hSem);
        return 4;
    }

    STARTUPINFOA siA, siB;
    PROCESS_INFORMATION piA, piB;
    ZeroMemory(&siA, sizeof(siA));
    ZeroMemory(&siB, sizeof(siB));
    ZeroMemory(&piA, sizeof(piA));
    ZeroMemory(&piB, sizeof(piB));
    siA.cb = sizeof(siA);
    siB.cb = sizeof(siB);

    char cmdLineA[512];
    char cmdLineB[512];

    snprintf(cmdLineA, sizeof(cmdLineA), "\"%s\" A", exePath);
    snprintf(cmdLineB, sizeof(cmdLineB), "\"%s\" B", exePath);

    BOOL okA = CreateProcessA(NULL, cmdLineA, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siA, &piA);
    if (!okA) {
        fprintf(stderr, "CreateProcess(A) failed: %lu\n", GetLastError());
        CloseHandle(hSem);
        return 5;
    }

    BOOL okB = CreateProcessA(NULL, cmdLineB, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siB, &piB);
    if (!okB) {
        fprintf(stderr, "CreateProcess(B) failed: %lu\n", GetLastError());
        CloseHandle(piA.hProcess);
        CloseHandle(piA.hThread);
        CloseHandle(hSem);
        return 6;
    }

    work(procName, hSem);

    WaitForSingleObject(piA.hProcess, INFINITE);
    WaitForSingleObject(piB.hProcess, INFINITE);

    CloseHandle(piA.hProcess); CloseHandle(piA.hThread);
    CloseHandle(piB.hProcess); CloseHandle(piB.hThread);
    CloseHandle(hSem);
    return 0;
}
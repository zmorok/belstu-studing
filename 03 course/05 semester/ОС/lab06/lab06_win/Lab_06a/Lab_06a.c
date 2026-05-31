#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

#define ITERATIONS 90

CRITICAL_SECTION crit_section;

char username[256];
int  nameLen = 0;

typedef struct ThreadParams {
    const char* name;
} ThreadParams;

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    ThreadParams* params = (ThreadParams*)lpParam;
    const char* threadName = params->name;

    DWORD pid = GetCurrentProcessId();

    int inInterval = 0;

    for (int i = 0; i <= ITERATIONS; i++) {
        DWORD tid = GetCurrentThreadId();

        if (i == 30) {
            EnterCriticalSection(&crit_section);
            inInterval = 1;
        }

        char letter = username[i % nameLen];

        printf("PID=%lu\tTID=%lu\tПоток=\t%s\tИтерация=%d\t%c\n", pid, tid, threadName, i, letter);
        Sleep(100);

        if (i == 60 && inInterval) {
            LeaveCriticalSection(&crit_section);
            inInterval = 0;
        }
    }

    if (inInterval) LeaveCriticalSection(&crit_section);
    
    return 0;
}

int main(void)
{
    DWORD username_len = sizeof(username);
    if (!GetUserNameA(username, &username_len)) { fprintf(stderr, "GetUserNameA failed with error %lu\n", GetLastError()); return 1; }

    nameLen = (int)strlen(username);
    if (nameLen <= 0) { fprintf(stderr, "Username length is 0\n"); return 2; }

    printf("Username: %s,\nLength: %d\n\n", username, nameLen);

    InitializeCriticalSection(&crit_section);

    ThreadParams paramsA = { "A" };
    ThreadParams paramsB = { "B" };
    ThreadParams paramsMain = { "MAIN" };

    HANDLE threads[2];

    threads[0] = CreateThread(NULL, 0, ThreadFunc, &paramsA, 0, NULL);
    if (threads[0] == NULL) {
        fprintf(stderr, "CreateThread(A) failed: %lu\n", GetLastError());
        DeleteCriticalSection(&crit_section);
        return 3;
    }

    threads[1] = CreateThread(NULL, 0, ThreadFunc, &paramsB, 0, NULL);
    if (threads[1] == NULL) {
        fprintf(stderr, "CreateThread(B) failed: %lu\n", GetLastError());
        CloseHandle(threads[0]);
        DeleteCriticalSection(&crit_section);
        return 4;
    }

    ThreadFunc(&paramsMain);

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);

    DeleteCriticalSection(&crit_section);

    return 0;
}
#include <windows.h>
#include <stdio.h>


DWORD WINAPI Lab_04x(LPVOID lpParam)
{
    if (lpParam == NULL) { fprintf(stderr, "Lab_04x: lpParam is NULL\n"); return 1; }
    int iterations = (int)lpParam;

    char username[200];
    DWORD username_len = sizeof(username);
    if (!GetUserNameA(username, &username_len)) { fprintf(stderr, "Lab_04x: GetUserNameA failed with error %lu\n", GetLastError()); return 5; }
    if (iterations <= 0) { fprintf(stderr, "Lab_04x: iterations must be > 0\n"); return 2; }
    if (username == NULL || username[0] == '\0') { fprintf(stderr, "Lab_04x: username is empty\n"); return 3; }

    int nameLen = (int)strlen(username);
    if (nameLen == 0) { fprintf(stderr, "Lab_04x: username length is 0\n"); return 4; }

    DWORD pid = GetCurrentProcessId();
    for (int i = 0; i < iterations; ++i) {
        DWORD tid = GetCurrentThreadId();
        char letter = username[i % nameLen];
        printf("PID=%lu\t- TID=%lu\t- №Итерации %d\t- %c\n", pid, tid, i + 1, letter);
        Sleep(350);
    }
    return 0;
}

#include <iostream>

using namespace std;

int main()
{
    HANDLE threads[2] = {
        CreateThread(NULL, 0, Lab_04x, (LPVOID)50, 0, NULL),
        CreateThread(NULL, 0, Lab_04x, (LPVOID)125, 0, NULL)
    };

    if (!threads[0] || !threads[1]) {
        printf("CreateThread error: %lu\n", GetLastError());
        if (threads[0]) CloseHandle(threads[0]);
        if (threads[1]) CloseHandle(threads[1]);
        return 1;
    }

    Lab_04x((LPVOID)100);
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
    return 0;
}
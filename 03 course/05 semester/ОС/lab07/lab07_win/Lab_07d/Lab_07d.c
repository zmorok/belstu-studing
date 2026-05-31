#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

static void BuildChildPath(char* outPath, size_t outSize, const char* childExeName)
{
    char parentPath[MAX_PATH];
    GetModuleFileNameA(NULL, parentPath, MAX_PATH);

    char* slash = parentPath;
    for (char* p = parentPath; *p; ++p) if (*p == '\\' || *p == '/') slash = p;
    *slash = '\0';

    snprintf(outPath, outSize, "%s\\%s", parentPath, childExeName);
}

static int StartChild(const char* exePath, int seconds, PROCESS_INFORMATION* pi)
{
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(pi, sizeof(*pi));

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "\"%s\" %d", exePath, seconds);

    BOOL ok = CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, pi);

    if (!ok)
    {
        printf("CreateProcess failed.\n  Cmd: %s\n  Error: %lu\n", cmd, GetLastError());
        return 0;
    }
    return 1;
}

int main()
{
    char childPath[MAX_PATH];
    BuildChildPath(childPath, sizeof(childPath), "Lab_07x.exe");

    printf("Child exe path: %s\n", childPath);
    system("pause");

    PROCESS_INFORMATION p1, p2;

    if (!StartChild(childPath, 60, &p1)) return 1;
    if (!StartChild(childPath, 120, &p2))
    {
        WaitForSingleObject(p1.hProcess, INFINITE);
        CloseHandle(p1.hThread);
        CloseHandle(p1.hProcess);
        return 1;
    }

    printf("Запуск дочерних процессов:\n");
    printf("  Дочерний #1 PID=%lu (60 sec)\n", p1.dwProcessId);
    printf("  Дочерний #2 PID=%lu (120 sec)\n", p2.dwProcessId);

    HANDLE procs[2] = { p1.hProcess, p2.hProcess };
    WaitForMultipleObjects(2, procs, TRUE, INFINITE);

    DWORD ec1 = 0, ec2 = 0;
    GetExitCodeProcess(p1.hProcess, &ec1);
    GetExitCodeProcess(p2.hProcess, &ec2);

    printf("\nЗавершение дочерних процессов:\n");
    printf("  Дочерний #1 exit code: %lu\n", ec1);
    printf("  Дочерний #2 exit code: %lu\n", ec2);

    CloseHandle(p1.hThread);
    CloseHandle(p1.hProcess);
    CloseHandle(p2.hThread);
    CloseHandle(p2.hProcess);

    return 0;
}

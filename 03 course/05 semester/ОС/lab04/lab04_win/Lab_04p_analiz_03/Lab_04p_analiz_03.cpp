#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) if (n % i == 0) return false;
    return true;
}

int WorkerMain(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stderr, "worker: неверное число аргументов\n");
        return 1;
    }

    char* endptr = NULL;
    long startL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') return 1;
    long endL = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') return 1;

    int start = (int)startL;
    int end = (int)endL;

    int count = 0;
    for (int n = start; n <= end; ++n) {
        if (isPrime(n))
            ++count;
    }

    // возвращение числа через завершение процесса
    ExitProcess((UINT)count);
}

int main(int argc, char* argv[])
{

    if (argc >= 2 && _stricmp(argv[1], "worker") == 0) return WorkerMain(argc, argv);

    if (argc != 3) {
        printf("Использование для измерений:\n");
        printf("  %s <Нижний_порог> <Верхний_порог>\n", argv[0]);
        printf("Например: %s 1 2097152\n", argv[0]);
        return 1;
    }

    char* endptr = NULL;
    long lowL = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный нижний порог диапазона\n"); return 1; }

    long highL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный верхний порог диапазона\n"); return 1; }

    if (lowL > highL) { printf("Ошибка: нижний порог больше верхнего\n"); return 1; }

    int low = (int)lowL;
    int high = (int)highL;

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    int cores = (int)si.dwNumberOfProcessors;
    int maxProcs = 2 * cores;
    if (maxProcs > MAXIMUM_WAIT_OBJECTS) maxProcs = MAXIMUM_WAIT_OBJECTS;

    char exePath[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, exePath, MAX_PATH);
    if (len == 0 || len == MAX_PATH) { printf("Ошибка GetModuleFileName\n"); return 1; }

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    printf("Диапазон: [%d; %d], логических процессоров: %d, процессы: 1..%d (x2)\n", low, high, cores, maxProcs);
    printf("Procs\tTime_ms\tPrimesCount\n");

    HANDLE procHandles[MAXIMUM_WAIT_OBJECTS];

    for (int procCount = 1; procCount <= maxProcs; procCount *= 2) {
        int totalRange = high - low + 1;
        int baseSize = totalRange / procCount;
        int remainder = totalRange % procCount;

        int current = low;

        LARGE_INTEGER t1, t2;
        QueryPerformanceCounter(&t1);

        int totalPrimes = 0;

        // дочерний процесс
        for (int i = 0; i < procCount; ++i) {
            int partSize = baseSize + (i < remainder ? 1 : 0);
            int start = current;
            int end = start + partSize - 1;

            if (partSize <= 0) { start = 0; end = -1; }

            current = end + 1;

            char cmdLine[256];
            sprintf(cmdLine, "\"%s\" worker %d %d", exePath, start, end);

            STARTUPINFOA siProc;
            PROCESS_INFORMATION piProc;
            ZeroMemory(&siProc, sizeof(siProc));
            siProc.cb = sizeof(siProc);
            ZeroMemory(&piProc, sizeof(piProc));

            BOOL ok = CreateProcessA(
                NULL,
                cmdLine,
                NULL,
                NULL,
                FALSE,
                CREATE_NO_WINDOW,
                NULL,
                NULL,
                &siProc,
                &piProc
            );

            if (!ok) {
                printf("Ошибка CreateProcess #%d, код = %lu\n", i, GetLastError());
                procCount = maxProcs + 1;
                break;
            }

            procHandles[i] = piProc.hProcess;
            CloseHandle(piProc.hThread);
        }

        if (procCount > maxProcs) break;

        DWORD wr = WaitForMultipleObjects(procCount, procHandles, TRUE, INFINITE);
        if (wr == WAIT_FAILED) printf("Ошибка WaitForMultipleObjects, код = %lu\n", GetLastError());
        

        for (int i = 0; i < procCount; ++i) {
            DWORD exitCode = 0;
            if (GetExitCodeProcess(procHandles[i], &exitCode)) totalPrimes += (int)exitCode;
            CloseHandle(procHandles[i]);
        }

        QueryPerformanceCounter(&t2);
        double elapsed_ms = (t2.QuadPart - t1.QuadPart) * 1000.0 / freq.QuadPart;

        printf("%d\t%.3f\t%d\n", procCount, elapsed_ms, totalPrimes);
    }

    return 0;
}
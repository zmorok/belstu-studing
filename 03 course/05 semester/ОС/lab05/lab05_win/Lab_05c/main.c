#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Helpers.h"

DWORD WINAPI WorkerThread(LPVOID param)
{
    clock_t start = clock();

    int id = (int)(DWORD_PTR)param;

    for (int i = 1; i <= 1000000; i++)
    {
        if (i % 1000 == 0)
        {
            Sleep(200);
            DWORD pid = GetCurrentProcessId();
            DWORD tid = GetCurrentThreadId();
            DWORD cpu = GetCurrentProcessorNumber();

            printf("[Поток %d] Итерация: %d\tPID=%lu\tTID=%lu\tПроцессор=%lu\n",
                id, i, pid, tid, cpu);
        }
    }

    clock_t end = clock();
    printf("! Поток %d завершился за %.3f секунд.\n", id, (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        printf("Использование: Lab_05c.exe P1 P2 P3 P4\n");
        return 1;
    }

    DWORD_PTR affinityMask = (DWORD_PTR)_strtoui64(argv[1], NULL, 0);

    int pProcess = atoi(argv[2]);
    int pThread1 = atoi(argv[3]);
    int pThread2 = atoi(argv[4]);

    DWORD processPriority = MapPriorityClass(pProcess);
    int threadPriority1 = MapThreadPriority(pThread1);
    int threadPriority2 = MapThreadPriority(pThread2);

    HANDLE hProcess = GetCurrentProcess();

    if (affinityMask != 0) SetProcessAffinityMask(hProcess, affinityMask);

    SetPriorityClass(hProcess, processPriority);

    HANDLE t1 = CreateThread(NULL, 0, WorkerThread, (LPVOID)1, 0, NULL);
    HANDLE t2 = CreateThread(NULL, 0, WorkerThread, (LPVOID)2, 0, NULL);

    if (!t1 || !t2)
    {
        printf("Ошибка создания потоков.\n");
        return 1;
    }

    SetThreadPriority(t1, threadPriority1);
    SetThreadPriority(t2, threadPriority2);

    system("pause");

    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);

    CloseHandle(t1);
    CloseHandle(t2);

    return 0;
}
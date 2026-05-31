#include "Helpers.h"

int main()
{
    clock_t start = clock();

    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();

    const int totalIterations = 1'000'000;

    for (int i = 1; i <= totalIterations; ++i)
    {
        if (i % 1000 == 0)
        {
            Sleep(200);

            DWORD pid = GetCurrentProcessId();
            DWORD tid = GetCurrentThreadId();
            DWORD pc  = GetPriorityClass(hProcess);
            int   tp  = GetThreadPriority(hThread);
            DWORD cpu = GetCurrentProcessorNumber();

            printf("Итерации: %d \n", i);
            printf("PID: %lu \n", pid);
            printf("TID: %lu \n", tid);
            printf("Класс приоритета процесса: %d (%s) \n", pc, GetPriorityClassName(pc));
            printf("Приоритет потока: %d (%s) \n", tp, GetThreadPriorityName(tp));
            printf("Номер процессора: %lu \n", cpu);
            printf("----------------------------------------------------\n");
        }
    }

    clock_t end = clock();
    float timeSeconds = (float)(end - start) / CLOCKS_PER_SEC;

    printf("\nВремя с момента запуска: %.3f секунд\n\n", timeSeconds);
    system("pause");

    return 0;
}
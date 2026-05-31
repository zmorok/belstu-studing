#include <windows.h>
#include <stdio.h>
#include "Helpers.h"

void PrintBinaryMask(const char* message, DWORD_PTR mask)
{
    printf("%s", message);

    int bits = (int)(sizeof(DWORD_PTR) * 8);
    for (int i = bits - 1; i >= 0; --i)
    {
        unsigned int bit = (unsigned int)((mask >> i) & 1);
        printf("%u", bit);
        if (i % 4 == 0) printf(" ");
    }
    printf("\n");
}

int CountSetBits(DWORD_PTR mask)
{
    int count = 0;
    while (mask)
    {
        if (mask & 1) ++count;
        mask >>= 1;
    }
    return count;
}

int main()
{
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    HANDLE hProcess = GetCurrentProcess();
    DWORD  priorityClass = GetPriorityClass(hProcess);

    HANDLE hThread = GetCurrentThread();
    int    threadPriority = GetThreadPriority(hThread);

    DWORD_PTR processAffinityMask = 0;
    DWORD_PTR systemAffinityMask = 0;

    BOOL affinityOk = GetProcessAffinityMask(
        hProcess,
        &processAffinityMask,
        &systemAffinityMask
    );

    DWORD currentProcessor = GetCurrentProcessorNumber();

    printf("PID: %lu\n", (unsigned long)processId);
    printf("TID: %lu\n\n", (unsigned long)threadId);

    printf("Класс приоритетов текущего процесса:\t%lu\t(%s)\n",
        (unsigned long)priorityClass,
        GetPriorityClassName(priorityClass));

    printf("Приоритет текущего потока:\t\t%d\t(%s)\n\n",
        threadPriority,
        GetThreadPriorityName(threadPriority));

    if (affinityOk)
    {
        PrintBinaryMask("Маска родственности процесса:\n  ", processAffinityMask);
        PrintBinaryMask("Системная маска родственности:\n  ", systemAffinityMask);

        int cpuCount = CountSetBits(processAffinityMask);
        printf("\nКоличество процессоров, доступных процессу: %d\n", cpuCount);
    }
    else
    {
        printf("Не удалось получить маски аффинности. Код ошибки: %lu\n", (unsigned long)GetLastError());
    }

    printf("Номер процессора для текущего потока: %lu\n", (unsigned long)currentProcessor);

    return 0;
}
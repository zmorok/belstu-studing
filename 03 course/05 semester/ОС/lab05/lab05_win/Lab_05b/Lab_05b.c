#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Helpers.h"

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        printf("Использование:\n  Lab_05b.exe P1 P2 P3\n\n");
        printf("P1 - целое число, маска родственности процессоров (DWORD_PTR)\n");
        printf("P2 - целое число, класс приоритета первого процесса\n");
        printf("P3 - целое число, класс приоритета второго процесса\n\n");
        printf("Коды приоритетов:\n");
        printf("  1 - IDLE\n");
        printf("  2 - BELOW_NORMAL\n");
        printf("  3 - NORMAL\n");
        printf("  4 - ABOVE_NORMAL\n");
        printf("  5 - HIGH\n");
        printf("  6 - REALTIME\n");
        return 1;
    }

    unsigned long long affinityMaskULL = strtoull(argv[1], NULL, 0);
    DWORD_PTR affinityMask = (DWORD_PTR)affinityMaskULL;

    int p2 = atoi(argv[2]);
    int p3 = atoi(argv[3]);

    DWORD prioClass2 = MapPriorityClass(p2);
    DWORD prioClass3 = MapPriorityClass(p3);

    printf("Запуск с параметрами:\n");
    printf("  P1 (маска родственности): %llu\n", affinityMaskULL);
    printf("  P2 (класс приоритета 1-го процесса): %d (%s)\n", p2, PriorityClassName(prioClass2));
    printf("  P3 (класс приоритета 2-го процесса): %d (%s)\n\n", p3, PriorityClassName(prioClass3));

    char cmdLine1[] = "Lab_05x.exe";
    char cmdLine2[] = "Lab_05x.exe";

    STARTUPINFOA si1; PROCESS_INFORMATION pi1;
    STARTUPINFOA si2; PROCESS_INFORMATION pi2;

    ZeroMemory(&si1, sizeof(si1));
    ZeroMemory(&pi1, sizeof(pi1));
    ZeroMemory(&si2, sizeof(si2));
    ZeroMemory(&pi2, sizeof(pi2));

    si1.cb = sizeof(si1);
    si2.cb = sizeof(si2);

    if (!CreateProcessA(NULL, cmdLine1, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si1, &pi1))
    {
        printf("Не удалось создать первый процесс. Код ошибки: %lu\n", (unsigned long)GetLastError());
        return 1;
    }

    if (!CreateProcessA(NULL, cmdLine2, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2))
    {
        printf("Не удалось создать второй процесс. Код ошибки: %lu\n", (unsigned long)GetLastError());

        TerminateProcess(pi1.hProcess, 1);
        CloseHandle(pi1.hThread);
        CloseHandle(pi1.hProcess);
        return 1;
    }

    if (!SetPriorityClass(pi1.hProcess, prioClass2))
    printf("Не удалось установить приоритет для 1-го процесса. Код ошибки: %lu\n", (unsigned long)GetLastError());
   
    if (!SetPriorityClass(pi2.hProcess, prioClass3))
    printf("Не удалось установить приоритет для 2-го процесса. Код ошибки: %lu\n", (unsigned long)GetLastError());
    
    if (affinityMask != 0)
    {
        if (!SetProcessAffinityMask(pi1.hProcess, affinityMask))
        printf("Не удалось установить аффинность для 1-го процесса. Код ошибки: %lu\n", (unsigned long)GetLastError());
        
        if (!SetProcessAffinityMask(pi2.hProcess, affinityMask))
        printf("Не удалось установить аффинность для 2-го процесса. Код ошибки: %lu\n", (unsigned long)GetLastError());      
    }

    printf("Оба дочерних процесса запущены.\n");

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi1.hThread);
    CloseHandle(pi1.hProcess);
    CloseHandle(pi2.hThread);
    CloseHandle(pi2.hProcess);

    printf("Дочерние процессы завершили работу.\n");
    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

int main(void)
{
    HANDLE waitableTimer = CreateWaitableTimerA(NULL, FALSE, NULL);
    if (!waitableTimer)
    {printf("Failed to create timer. Error: %lu\n", GetLastError()); return 1;}

    LARGE_INTEGER dueTime;
    dueTime.QuadPart = -30000000LL;

    if (!SetWaitableTimer(waitableTimer, &dueTime, 3000, NULL, NULL, FALSE))
    {
        printf("Failed to set timer. Error: %lu\n", GetLastError());
        CloseHandle(waitableTimer);
        return 1;
    }

    unsigned long long iterationCount = 0;
    ULONGLONG startTime = GetTickCount64();
    int periodsElapsed = 0;

    while (TRUE)
    {
        iterationCount++;

        DWORD timerStatus = WaitForSingleObject(waitableTimer, 0);
        if (timerStatus == WAIT_OBJECT_0)
        {
            periodsElapsed++;
            printf("[После %02d секунд] Итерации: %llu\n", periodsElapsed * 3, iterationCount);
        }

        if (GetTickCount64() - startTime >= 15000)
        {
            printf("[После 15 секунд] Итерации: %llu\n", iterationCount);
            break;
        }
    }

    CancelWaitableTimer(waitableTimer);
    CloseHandle(waitableTimer);
    return 0;
}
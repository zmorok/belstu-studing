#include "Helpers.h"

DWORD MapPriorityClass(int p)
{
    switch (p)
    {
        case 1: return IDLE_PRIORITY_CLASS;
        case 2: return BELOW_NORMAL_PRIORITY_CLASS;
        case 3: return NORMAL_PRIORITY_CLASS;
        case 4: return ABOVE_NORMAL_PRIORITY_CLASS;
        case 5: return HIGH_PRIORITY_CLASS;
        case 6: return REALTIME_PRIORITY_CLASS;
    }
    return NORMAL_PRIORITY_CLASS;
}

int MapThreadPriority(int p)
{
    switch (p)
    {
        case 1: return THREAD_PRIORITY_LOWEST;
        case 2: return THREAD_PRIORITY_BELOW_NORMAL;
        case 3: return THREAD_PRIORITY_NORMAL;
        case 4: return THREAD_PRIORITY_ABOVE_NORMAL;
        case 5: return THREAD_PRIORITY_HIGHEST;
    }
    return THREAD_PRIORITY_NORMAL;
}
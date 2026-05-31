#include "Helpers.h"
#include <windows.h>

const char* GetPriorityClassName(DWORD pc)
{
    switch (pc)
    {
    case IDLE_PRIORITY_CLASS:           return "IDLE_PRIORITY_CLASS";
    case BELOW_NORMAL_PRIORITY_CLASS:   return "BELOW_NORMAL_PRIORITY_CLASS";
    case NORMAL_PRIORITY_CLASS:         return "NORMAL_PRIORITY_CLASS";
    case ABOVE_NORMAL_PRIORITY_CLASS:   return "ABOVE_NORMAL_PRIORITY_CLASS";
    case HIGH_PRIORITY_CLASS:           return "HIGH_PRIORITY_CLASS";
    case REALTIME_PRIORITY_CLASS:       return "REALTIME_PRIORITY_CLASS";
    default:                            return "UNKNOWN_PRIORITY_CLASS";
    }
}

const char* GetThreadPriorityName(int tp)
{
    switch (tp)
    {
    case THREAD_PRIORITY_IDLE:          return "THREAD_PRIORITY_IDLE";
    case THREAD_PRIORITY_LOWEST:        return "THREAD_PRIORITY_LOWEST";
    case THREAD_PRIORITY_BELOW_NORMAL:  return "THREAD_PRIORITY_BELOW_NORMAL";
    case THREAD_PRIORITY_NORMAL:        return "THREAD_PRIORITY_NORMAL";
    case THREAD_PRIORITY_ABOVE_NORMAL:  return "THREAD_PRIORITY_ABOVE_NORMAL";
    case THREAD_PRIORITY_HIGHEST:       return "THREAD_PRIORITY_HIGHEST";
    case THREAD_PRIORITY_TIME_CRITICAL: return "THREAD_PRIORITY_TIME_CRITICAL";
    default:                            return "UNKNOWN_THREAD_PRIORITY";
    }
}
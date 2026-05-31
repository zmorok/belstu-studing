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
    default: return NORMAL_PRIORITY_CLASS;
    }
}

const char* PriorityClassName(DWORD pc)
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

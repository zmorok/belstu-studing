#include <windows.h>
#include <stdio.h>

int main()
{
    SYSTEMTIME utcTime, localTime;

    GetSystemTime(&utcTime); GetLocalTime(&localTime);

    int utcTotalSeconds   = utcTime.wHour   * 3600 + utcTime.wMinute   * 60 + utcTime.wSecond;
    int localTotalSeconds = localTime.wHour * 3600 + localTime.wMinute * 60 + localTime.wSecond;

    int timezoneOffsetSeconds = localTotalSeconds - utcTotalSeconds;

    if (timezoneOffsetSeconds > 12 * 3600)          timezoneOffsetSeconds -= 24 * 3600;
    else if (timezoneOffsetSeconds < -12 * 3600)    timezoneOffsetSeconds += 24 * 3600;

    int timezoneOffsetHours = timezoneOffsetSeconds / 3600;
    char sign  = (timezoneOffsetHours >= 0) ? '+' : '-';
    int offset = (timezoneOffsetHours >= 0) ? timezoneOffsetHours : -timezoneOffsetHours;

    printf("%04d-%02d-%02dT%02d:%02d:%02d%c%02d\n",
        localTime.wYear, localTime.wMonth, localTime.wDay,
        localTime.wHour, localTime.wMinute, localTime.wSecond, sign, offset);

    return 0;
}
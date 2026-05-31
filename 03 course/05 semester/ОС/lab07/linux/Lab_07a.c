#include <stdio.h>
#include <time.h>

int main()
{
    time_t now = time(NULL);

    struct tm gmt_tm, loc_tm;
    gmtime_r(&now, &gmt_tm); localtime_r(&now, &loc_tm);

    int utcTotalSeconds =
        gmt_tm.tm_hour * 3600 +
        gmt_tm.tm_min  * 60 +
        gmt_tm.tm_sec;

    int localTotalSeconds =
        loc_tm.tm_hour * 3600 +
        loc_tm.tm_min  * 60 +
        loc_tm.tm_sec;

    int timezoneOffsetSeconds = localTotalSeconds - utcTotalSeconds;

    if (timezoneOffsetSeconds > 12 * 3600) timezoneOffsetSeconds -= 24 * 3600;
    else if (timezoneOffsetSeconds < -12 * 3600) timezoneOffsetSeconds += 24 * 3600;

    int timezoneOffsetHours = timezoneOffsetSeconds / 3600;
    char sign = (timezoneOffsetHours >= 0) ? '+' : '-';
    int offset = (timezoneOffsetHours >= 0) ? timezoneOffsetHours : -timezoneOffsetHours;

    printf("%04d-%02d-%02dT%02d:%02d:%02d%c%02d\n",
           loc_tm.tm_year + 1900,
           loc_tm.tm_mon + 1,
           loc_tm.tm_mday,
           loc_tm.tm_hour,
           loc_tm.tm_min,
           loc_tm.tm_sec,
           sign,
           offset);

    return 0;
}
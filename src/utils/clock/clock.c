#include "clock.h"

int _gettimeofday( struct timeval *tv, void *tzvp )
{
    time_t timestamp = clock_get_time();
    tv->tv_sec = timestamp;
    tv->tv_usec = 0;

    return 0;
}

time_t clock_get_time(void)
{
    uint32_t year, month, date, dayofweek;
    uint32_t hour, minute, second;

    rtc_get_date(RTC, &year, &month, &date, &dayofweek);
    rtc_get_time(RTC, &hour, &minute, &second);

    struct calendar_date calendar =
    {
        .second = second,
        .minute = minute,
        .hour = hour,
        .date = date - 1,       // calendar uses 0-30 for days of month
        .month = month - 1,     // calendar uses 0-11 for months
        .year = year,
        .dayofweek = dayofweek
    };

    return calendar_date_to_timestamp(&calendar);
}

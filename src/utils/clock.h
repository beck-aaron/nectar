/*******************************************************************************
 * File:    clock.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <rtc.h>
#include <sys/time.h>
#include <calendar.h>

/**
 * @brief Implementation of library function to enable clock functions
 * needed for clock.h header file
 *
 * @param tv
 * @param tzvp
 *
 * @return 
 */
int _gettimeofday( struct timeval *tv, void *tzvp );

inline static time_t clock_get_time(void)
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

#endif /* _CLOCK_H_ */

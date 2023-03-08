#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "../Nectar.h"
#include "MySpi1.h"

typedef union
{
    struct
    {
        uint8_t milliseconds;
        uint8_t seconds;
        uint8_t minutes;
        uint8_t hours;
    };

    uint8_t buffer[8];

} clock_time_t;

clock_time_t bcd_time, int_time;

typedef union
{
    struct
    {
        uint8_t weekday;
        uint8_t month;
        uint8_t day;
        uint8_t year;
    };

    uint8_t buffer[8];

} clock_date_t;

clock_date_t bcd_date, int_date;

void initialize_clock(void);
uint8_t get_minutes(void);
uint8_t get_seconds(void);
uint8_t get_milliseconds(void);

#endif // _CLOCK_H_

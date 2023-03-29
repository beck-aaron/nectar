#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>
#include <stdbool.h>

// TODO wrap timer utility here, essentially when a timer is init, it records
// the time of initialization.  Every time, the timer_expired function is called,
// the current system time is subtracted from the timer start time to determine
// how long the timer has been running for.  If this time is greater or equal to
// the timer value that it was initialized with, the function returns true.

typedef struct
{
    time_t start_time;
    double time_limit;

} timer;

inline static timer timer_init(double time_limit)
{
    return (timer){time(NULL), time_limit};
}

inline static bool timer_expired(timer* t)
{
    time_t current_time = time(&current_time);
    return difftime(current_time, t->start_time) >= t->time_limit;
}

#endif /* _TIMER_H_ */

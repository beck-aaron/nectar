#include "clock.h"

int _gettimeofday( struct timeval *tv, void *tzvp )
{
    time_t timestamp = clock_get_time();
    tv->tv_sec = timestamp;
    tv->tv_usec = 0;

    return 0;
}


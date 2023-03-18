/*******************************************************************************
 * File:    clock.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <sys/time.h>
#include <calendar.h>
#include <rtc.h>

int _gettimeofday(struct timeval *tv, void *tzvp);
time_t clock_get_time(void);

#endif /* _CLOCK_H_ */

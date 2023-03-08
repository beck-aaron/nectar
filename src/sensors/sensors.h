/*******************************************************************************
 * File:    sensors.h
 * Author:  Aaron Beck
 *
 * Includes all the custom sensor drivers.
 *
 ******************************************************************************/
#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "coz-ir.h"
#include "telaire-t6615.h"
//#include "trisonica-mini.h"

typedef struct
{
    void (*init)(void);  // function to initialize sensor
    void (*read)(void);  // function to retrieve data from sensor and log it

} sensor_t;

#endif // _SENSORS_H_

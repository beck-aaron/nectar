/*******************************************************************************
 * File:    devices.c
 * Author:  Aaron Beck
 *
 * Stores the data structures for each device used with the Nectar driver
 * and a function to initialize all the peripherals used for external devices.
 *
 *
 * Each device will be initialized using a timer set by the RTT peripheral, this
 * is used to determine which devices are connected to the system.
 *
 ******************************************************************************/
#include "devices.h"
#include "../nectar.h"

void devices_init(void* driver)
{
    nectar_t* nectar = driver;
    xbee_init(&nectar->xbee);
    coz_ir_init(&nectar->coz_ir);
}

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

void devices_init(xbee_t* xbee, coz_ir_t* coz_ir, telaire_t* telaire, trisonica_t* trisonica)
{
    xbee_init(xbee);
    coz_ir_init(coz_ir);
//  telaire_init(telaire);
//  trisonica_init(trisonica);
}

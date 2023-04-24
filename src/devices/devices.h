/*******************************************************************************
 * File:    devices.h
 * Author:  Aaron Beck
 *
 * An entry point to initialize all the peripherals used for external devices
 * and the drivers used to communicate with those devices.
 *
 ******************************************************************************/
#ifndef DEVICES_H_
#define DEVICES_H_

#include "xbee/xbee.h"
#include "coz_ir/coz_ir.h"
#include "telaire/telaire.h"
#include "trisonica/trisonica.h"

/**
 * void devices_init(xbee_t* xbee, coz_ir_t* coz_ir, telaire_t* telaire, trisonica_t* trisonica);
 *
 * attempt to connect with each device, if device is connected
 * the state variable for each device will be set to DEVICE_CONNECTED
 * this is used to compute the bitmask for our subpayloads.
 *
 * acknowledge the fact that the bitmask is wasting space within subpayload
 * since we are not currently supporting hot-swapping of sensors
 */
void devices_init(xbee_t* xbee, coz_ir_t* coz_ir, telaire_t* telaire, trisonica_t* trisonica);

#endif /* DEVICES_H_ */

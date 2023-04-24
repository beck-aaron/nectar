/*******************************************************************************
 * File:    coz_ir.h
 * Author:  Aaron Beck
 *
 * Description: driver for the COZ-IR A sensor.  This is a very barebones
 * driver that polls the device for temperature, humidity, and co2 ppm.  No
 * error checking is performed.  The device is assumed to be plugged in. There
 * is much room for improvement on this driver. DMA is not used at the moment,
 * the only commands used on this device are very short and do not warrant DMA
 * currently although this could be implemented in the future.
 *
 ******************************************************************************/
#ifndef _COZ_IR_H_
#define _COZ_IR_H_

#include <dma.h>
#include <state.h>
#include <timer.h>
#include <logger.h>

#define COZ_IR_MAX_TX   256 // arbitrary values for now. replace!
#define COZ_IR_MAX_RX   256
#define BASE_TEN 10

typedef enum
{
    COZ_IR_CO2_PPM,
    COZ_IR_TEMPERATURE,
    COZ_IR_HUMIDITY,

} coz_ir_data_t;

typedef struct
{
    uint8_t  delimiter;
    uint16_t length;

    volatile device_state_t state;

    vector_t tx_buffer;
    vector_t rx_buffer;
    serial_state_t rx_state;

    uint16_t co2_ppm;
    float    temperature;
    float    humidity;

} coz_ir_t;

// Implemented
void coz_ir_init(coz_ir_t* coz_ir);
uint16_t coz_ir_get_ppm(coz_ir_t* coz_ir);
float coz_ir_get_temp(coz_ir_t* coz_ir);
float coz_ir_get_humidity(coz_ir_t* coz_ir);

// TODO
void coz_ir_transmit(coz_ir_t* coz_ir);
void coz_ir_receive(coz_ir_t* coz_ir);
void coz_ir_encode(coz_ir_t* coz_ir);
void coz_ir_decode(coz_ir_t* coz_ir);

#endif /* _COZ_IR_H_ */

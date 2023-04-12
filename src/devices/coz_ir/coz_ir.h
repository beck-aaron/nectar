#ifndef _COZ_IR_H_
#define _COZ_IR_H_

#include <dma.h>
#include <state.h>
#include <timer.h>
#include <logger.h>
#include "commands.h"

#define COZ_IR_MAX_TX   256 // arbitrary values for now. replace!
#define COZ_IR_MAX_RX   256

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

    vector_t co2_ppm;
    vector_t temperature;
    vector_t humidity;

} coz_ir_t;

void coz_ir_init(coz_ir_t* coz_ir);
void coz_ir_transmit(coz_ir_t* coz_ir);
void coz_ir_receive(coz_ir_t* coz_ir);
void coz_ir_encode(coz_ir_t* coz_ir);
void coz_ir_decode(coz_ir_t* coz_ir);
float coz_ir_get_data(coz_ir_t* coz_ir, coz_ir_data_t type);


#endif /* _COZ_IR_H_ */

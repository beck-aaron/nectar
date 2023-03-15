#ifndef _COZ_IR_H_
#define _COZ_IR_H_

#include <asf.h>
#include <logger.h>

#define COZ_IR_MAX_TX   256 // arbitrary values for now. replace!
#define COZ_IR_MAX_RX   256
#define coz_ir_receive  COZ_IR_UART_HANDLER

void coz_ir_encode(void);
void coz_ir_decode(void);
void coz_ir_transmit(void);
void coz_ir_configure(void);

typedef struct
{
    // specific data types go here
    uint8_t tx_data[COZ_IR_MAX_TX];
    uint8_t rx_data[COZ_IR_MAX_RX];

    // sensor functions here
    void (*encode)(void);
    void (*decode)(void);
    void (*transmit)(void);
    void (*configure)(void);

} coz_ir_t;
extern coz_ir_t coz_ir;

#endif /* _COZ_IR_H_ */

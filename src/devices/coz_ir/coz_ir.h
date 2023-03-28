#ifndef _COZ_IR_H_
#define _COZ_IR_H_

#include <logger.h>

#define COZ_IR_MAX_TX   256 // arbitrary values for now. replace!
#define COZ_IR_MAX_RX   256

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

void coz_ir_init(coz_ir_t* coz_ir);
void coz_ir_transmit(coz_ir_t* coz_ir);
void coz_ir_receive(coz_ir_t* coz_ir);
void coz_ir_encode(coz_ir_t* coz_ir);
void coz_ir_decode(coz_ir_t* coz_ir);


#endif /* _COZ_IR_H_ */

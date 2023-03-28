#include "coz_ir.h"

void coz_ir_init(coz_ir_t* coz_ir)
{
    //todo: init buffers here
    serial_uart_init(COZ_IR);

    // configure coz_ir here
    LOG(DEBUG_LEVEL, "Initialized serial interface for coz-ir.");
}


void coz_ir_encode(coz_ir_t* coz_ir)
{
}

void coz_ir_decode(coz_ir_t* coz_ir)
{
}

void coz_ir_transmit(coz_ir_t* coz_ir)
{
}

void coz_ir_receive(coz_ir_t* coz_ir)
{
}

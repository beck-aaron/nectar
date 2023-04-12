/*******************************************************************************
 * File:    trisonica.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "trisonica.h"

void trisonica_init(trisonica_t* trisonica)
{
    //todo: init buffers here
    serial_uart_init(TRISONICA);

    // configure trisonica here

//  LOG(DEBUG_LEVEL, "Initialized serial interface for trisonica mini.");
}

void trisonica_encode(trisonica_t* trisonica)
{
}

void trisonica_decode(trisonica_t* trisonica)
{
}

void trisonica_transmit(trisonica_t* trisonica)
{
}

void trisonica_receive(trisonica_t* trisonica)
{
}

void trisonica_configure(trisonica_t* trisonica)
{
}

/*******************************************************************************
 * File:    telaire.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _TELAIRE_H_
#define _TELAIRE_H_

#include <vector.h>
#include "tsunami.h"

#define TELAIRE_MAX_TX  256  // TODO: determine how much memory our
#define TELAIRE_MAX_RX  256  // program has access to... fix these values
#define TELAIRE_HEADER_FLAG 0xFF
#define TELAIRE_HEADER_ADDR 0xFE
#define telaire_receive TRISONICA_UART_HANDLER

void telaire_encode(void);
void telaire_decode(void);
void telaire_transmit(void);
void telaire_configure(void);
void telaire_push(void* value, size_t size);

typedef struct
{
    uint8_t         flag;
    uint8_t         address;
    uint8_t         length;
    tsunami_cmd_t   command;

    union
    {
        cmd_read_t      cmd_read;
        cmd_update_t    cmd_update;
        cmd_status_t    cmd_status;
        cmd_test_t      cmd_test;
    };

    // specific data types go here
    vector_t tx_buffer;
    vector_t rx_buffer;

    void (*encode)(void);
    void (*decode)(void);
    void (*transmit)(void);
    void (*configure)(void);
    void (*push)(void* value, size_t size);

} telaire_t;
extern telaire_t telaire;

#endif /* _TELAIRE_H_ */

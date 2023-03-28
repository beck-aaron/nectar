/*******************************************************************************
 * File:    telaire.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _TELAIRE_H_
#define _TELAIRE_H_

#include <logger.h>
#include <vector.h>
#include "tsunami.h"

#define TELAIRE_MAX_TX  256  // TODO: determine how much memory our
#define TELAIRE_MAX_RX  256  // program has access to... fix these values
#define TELAIRE_HEADER_FLAG 0xFF
#define TELAIRE_HEADER_ADDR 0xFE

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

} telaire_t;

void telaire_init(telaire_t* telaire);
void telaire_transmit(telaire_t* telaire);
void telaire_receive(telaire_t* telaire);
void telaire_encode(telaire_t* telaire);
void telaire_decode(telaire_t* telaire);
void telaire_configure(telaire_t* telaire);

#endif /* _TELAIRE_H_ */

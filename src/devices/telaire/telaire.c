/*******************************************************************************
 * File:    telaire.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "telaire.h"

void telaire_init(telaire_t* telaire)
{
    telaire->flag = TELAIRE_HEADER_FLAG;
    telaire->address = TELAIRE_HEADER_ADDR;
    vector_init(TELAIRE_MAX_TX, &telaire->tx_buffer);
    vector_init(TELAIRE_MAX_RX, &telaire->rx_buffer);
    serial_uart_init(TELAIRE);

    // configure telaire here
//  LOG(DEBUG_LEVEL, "Initialized serial interface for telaire.");
}

void telaire_transmit(telaire_t* telaire)
{
}

void telaire_receive(telaire_t* telaire)
{
}

void telaire_configure(telaire_t* telaire)
{
}

void telaire_encode(telaire_t* telaire)
{
    vector_push(&telaire->flag, sizeof(uint8_t), &telaire->tx_buffer);
    vector_push(&telaire->address, sizeof(uint8_t), &telaire->tx_buffer);

    switch(telaire->command)
    {
        case CMD_READ:
//          telaire_encode_read_cmd();
            break;

        case CMD_WARMUP:
        case CMD_STATUS:
        case CMD_HALT:
        case CMD_STREAM_DATA:
            vector_push(&telaire->command, sizeof(uint8_t), &telaire->tx_buffer);
            break;

        case CMD_UPDATE:
//          telaire_encode_update_cmd();
            break;

        case CMD_LOOPBACK:
//          telaire_encode_loopback_cmd();
            break;

        case CMD_SELF_TEST_START:
            break;

        case CMD_SELF_TEST_RESULTS:
            break;
    }
}

void telaire_decode(telaire_t* telaire)
{
}

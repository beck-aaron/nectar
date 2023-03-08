/*******************************************************************************
 * File:    telarie-t6615.c
 * Author:  Aaron Beck
 *
 * Drives the telaire-t6615 sensor, attached to a UART interface defined by
 * config/conf_uart_serial.h
 *
 * We configure the sensor in the initialization function to stream data.
 * This prevents us sending a request for data and not receiving a response.
 *
 * However, this means that the cpu is bounded by waiting for this IO operation.
 * TODO: figure out how to use DMA in order to remove this wait time for the CPU
 *
 ******************************************************************************/
#include "telaire-t6615.h"

static uint16_t co2_ppm = 0;
static uint8_t response[] = {0x00, 0x00, 0x00, 0x00, 0x00};

void telaire_init(void)
{
    uint8_t command[] = {0xFF, 0xFE, 0x01, 0xBD};
    usart_serial_options_t usart_options =
    {
        .baudrate   = TELAIRE_UART_BAUDRATE,
        .charlength = TELAIRE_UART_CHAR_LENGTH,
        .paritytype = TELAIRE_UART_PARITY,
        .stopbits   = TELAIRE_UART_STOP_BITS
    };

    //delay_s(5); // wait for sensor to boot up.
    usart_serial_init(TELAIRE_UART, &usart_options);
    usart_serial_write_packet(TELAIRE_UART, command, sizeof(command));
    LOG(DEBUG_LEVEL, "initialized telaire-t6615 interface");
}

void telaire_read(void)
{
    usart_serial_read_packet(TELAIRE_UART, response, sizeof(response));
    LOGHEX(DEBUG_LEVEL, "Telaire response", response, 5);

    co2_ppm = (response[sizeof(response)-2] << 8) | response[sizeof(response)-1];
    LOG(DEBUG_LEVEL, "CO2 level: %u ppm", co2_ppm);
}

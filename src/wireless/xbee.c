/*
 *
 * xbee driver for SAME70
 *
 * Note: ignoring xdmac for rapid prototyping, plan to integrate this later
 * Goal: intialize XBEE and configure one AT command
 * Prerequisite: XBEE must be configured to match serial settings in conf/conf_uart_serial.h
 *
 */

#include "xbee.h"

uint8_t response_idx = 0;
uint8_t response[256] = {'\0'};

void xbee_init(void)
{
    usart_serial_options_t usart_options =
    {
        .baudrate   = XBEE_UART_BAUDRATE,
        .charlength = XBEE_UART_CHAR_LENGTH,
        .paritytype = XBEE_UART_PARITY,
        .stopbits   = XBEE_UART_STOP_BITS
    };

    usart_enable_interrupt(XBEE_UART, US_IER_RXRDY);
    irq_register_handler(XBEE_UART_IRQ, 1);
    usart_serial_init(XBEE_UART, &usart_options);
    LOG(DEBUG_LEVEL, "Initialized xbee interface.");

    xbee_configure();
}

// shoves bytes into local response buffer as soon as they arrive
void xbee_uart_handler(void)
{
    usart_serial_read_packet(XBEE_UART, &response[response_idx++], 1);
}

void xbee_configure(void)
{
    LOG(DEBUG_LEVEL, "Configuring xbee with local AT commands.");
    // create frame data, set frame data within api frame, store api frame
    // within zigbee packet
    frame_data_t frame_data = { .at_command = {0x1, VL, 0} };
    api_frame_t api_frame = { AT_COMMAND, &frame_data };
    xbee_packet_t packet = { FRAME_DELIMITER, ZERO, &api_frame, ZERO };

    // when structs are all populated with data, encode to encoder's buffer
    encoder_init();
    encode_packet(&packet);
    xbee_write(encoder_get(), encoder_size());

    // after we receive data, we need to decode into our structs again
    LOGHEX(RX_LEVEL, "Received data over xbee UART", response, sizeof(response));
    LOG(DEBUG_LEVEL, "Completed xbee configuration. Ready to connect...");
    encoder_destroy(); // do we need to destroy the encoder at all?
}

void xbee_write(const void* data, const size_t size)
{
    LOGHEX(TX_LEVEL, "Writing data over xbee UART.", data, size);
    usart_serial_write_packet(XBEE_UART, (uint8_t*)data, size);
}

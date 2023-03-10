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

//#define COMMAND_MODE "+++"
//#define AT_COMMAND "AT"
//#define AT_COMMAND_SIZE 2
//#define CARRIAGE_RETURN "\r"
//#define COMMAND_MODE_LENGTH 3
//#define RETURN_CODE_LENGTH 3

#define xbee_uart_handler XBEE_UART_HANDLER

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

    usart_serial_init(XBEE_UART, &usart_options);
    LOG(DEBUG_LEVEL, "initialized xbee interface");

    //irq_initialize_vectors();
    usart_enable_interrupt(XBEE_UART, US_IER_RXRDY);
    irq_register_handler(XBEE_UART_IRQ, 0);

    xbee_configure();
    LOG(DEBUG_LEVEL, "configured xbee device as routing device");
}

void xbee_uart_handler(void)
{
    usart_serial_read_packet(XBEE_UART, &response[response_idx++], 1);
}

void xbee_configure(void)
{
    // create frame data, set frame data within api frame, store api frame
    // within zigbee packet
    frame_data_t frame_data = {.at_command = {0x1, VL, 0}};
    api_frame_t api_frame = {AT_COMMAND, &frame_data};
    xbee_packet_t packet = {FRAME_DELIMITER, ZERO, &api_frame, ZERO};

    // when structs are all populated with data, encode to encoder's buffer
    encoder_init();

    encode_packet(&packet);
    LOG(DEBUG_LEVEL, "encoding local at command: read version long");
    xbee_write(encoder_get(), encoder_size());

    // after we receive data, we need to decode into our structs again
    LOGHEX(RX_LEVEL, "received xbee local AT response", response, sizeof(response));
    encoder_destroy();
}

void xbee_read_cmd(const char* cmd)
{
}

void xbee_set_cmd(const char* cmd, uint16_t parameter)
{
}

void xbee_write(const void* data, const size_t size)
{
    LOGHEX(TX_LEVEL, "sending data to xbee", data, size);
    usart_serial_write_packet(XBEE_UART, (uint8_t*)data, size);
}

void xbee_read(void)
{
    uint8_t received_byte = 0;
    usart_serial_getchar(XBEE_UART, &received_byte);
    while (received_byte != '\r')
    {
        usart_serial_getchar(XBEE_UART, &received_byte);
        LOG(WARNING_LEVEL, "received %c", received_byte);
    }
}

void xbee_return_code(void)
{
}

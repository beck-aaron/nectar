#include "xbee.h"
#include "logger.h"

xbee_t xbee = 
{
    .delimiter  = API_FRAME_DELIMITER,
    .test       = test_xbee,
    .encode     = encode_xbee,
    .decode     = decode_xbee,
    .configure  = configure_xbee,
    .transmit   = transmit_xbee
}; 

void test_xbee(void)
{
}

static void encode_at_command(const at_command_t* at_command)
{
    vector_push(&at_command->frame_id, sizeof(uint8_t), xbee.tx_buffer);
    vector_push(&at_command->code, sizeof(uint16_t), xbee.tx_buffer);
//  vector_push(&at_command->param, sizeof(uint16_t), xbee.tx_buffer);
}

void encode_xbee(void)
{
    vector_push(&xbee.delimiter, sizeof(uint8_t), xbee.tx_buffer);
    vector_push(&xbee.length, sizeof(uint16_t), xbee.tx_buffer);
    vector_push(&xbee.api_frame.cmdID, sizeof(uint8_t), xbee.tx_buffer);

    // push api frame
    switch(xbee.api_frame.cmdID)
    {
        case AT_COMMAND:
            encode_at_command(&xbee.api_frame.at_command);
            break;

        case ZIGBEE_TRANSMIT_REQUEST:
            break;

        case ZIGBEE_TRANSMIT_STATUS:
            break;
    }

    // calculate length, and push
    xbee.length = Swap16(xbee.tx_buffer->size - API_FRAME_HEADER_LENGTH);
    memcpy(&xbee.tx_buffer->data[API_FRAME_HEADER_IDX], &xbee.length, sizeof(uint16_t));


    // calculate checksum, and push
    xbee.checksum = 0;
    for (size_t i = API_FRAME_HEADER_LENGTH; i < xbee.tx_buffer->size; ++i)
        xbee.checksum += xbee.tx_buffer->data[i];

    xbee.checksum = 0xFF - xbee.checksum;
    vector_push(&xbee.checksum, sizeof(uint8_t), xbee.tx_buffer);
}

void decode_xbee(void)
{
    // wait for full baudrate cycle to make sure all data is delivered
    delay_us(XBEE_UART_BAUDRATE);
    LOGHEX(RX_LEVEL, "Received xbee serial data.", xbee.rx_buffer->data, xbee.rx_buffer->size);
    vector_clear(xbee.rx_buffer);
}

void configure_xbee(void)
{
    xbee.api_frame.cmdID = AT_COMMAND;
    xbee.api_frame.at_command.frame_id  = 0x01;

    xbee.api_frame.at_command.code      = VR;
    xbee.transmit();

    xbee.api_frame.at_command.code      = VL;
    xbee.transmit();

    xbee.api_frame.at_command.code      = NP;
    xbee.transmit();
}

void transmit_xbee(void)
{
    xbee.encode();
    LOGHEX(TX_LEVEL, "Transmitting serial data to xbee", xbee.tx_buffer->data, xbee.tx_buffer->size);
    usart_serial_write_packet(XBEE_UART, xbee.tx_buffer->data, xbee.tx_buffer->size);
    vector_clear(xbee.tx_buffer);
    xbee.decode();
}

void receive_xbee(void)
{
    usart_serial_getchar(XBEE_UART, &xbee.rx_buffer->data[xbee.rx_buffer->size++]);
}

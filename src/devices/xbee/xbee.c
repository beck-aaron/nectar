#include "xbee.h"
#include "logger.h"

vector_t buffer = VECTOR();

void test_xbee(void)
{
    xbee.api_frame.cmdID = AT_COMMAND;
    xbee.api_frame.at_command.frame_id = 0x01;
    xbee.api_frame.at_command.code = VL;
    xbee.api_frame.at_command.param = 0;

    xbee.encode();
    xbee.transmit();
    xbee.decode();
}

void encode_xbee(void)
{
    buffer.set(xbee.tx_data, XBEE_MAX_TX);
    buffer.push(&xbee.delimiter, sizeof(uint8_t));
    buffer.push(&xbee.length, sizeof(uint16_t));
    buffer.push(&xbee.api_frame.cmdID, sizeof(uint8_t));

    // push api frame
    switch(xbee.api_frame.cmdID)
    {
        case AT_COMMAND:
            break;

        case ZIGBEE_TRANSMIT_REQUEST:
            break;

        case ZIGBEE_TRANSMIT_STATUS:
            break;
    }

    // calculate length, and push
    xbee.length = Swap16(buffer.size - API_FRAME_HEADER_LENGTH);
    memcpy(&buffer.data[API_FRAME_HEADER_IDX], &xbee.length, sizeof(uint16_t));

    // calculate checksum, and push
    for (size_t i = API_FRAME_HEADER_LENGTH; i < buffer.size; ++i)
        xbee.checksum += buffer.data[i];

    xbee.checksum = 0xFF - xbee.checksum;
    buffer.push(&xbee.checksum, sizeof(uint8_t));
}

void decode_xbee(void)
{
    LOGHEX(RX_LEVEL, "Received xbee serial data.", xbee.rx_data, XBEE_MAX_RX);
}

void configure_xbee(void)
{
}

void transmit_xbee(void)
{
    LOGHEX(TX_LEVEL, "Transmitting serial data to xbee", xbee.tx_data, XBEE_MAX_TX);
    usart_serial_write_packet(XBEE_UART, xbee.tx_data, xbee.length);
}

void XBEE_UART_HANDLER(void)
{
    if (xbee.idx >= XBEE_MAX_RX)
    {
        LOG(ERROR_LEVEL, "Xbee rx buffer overflow. Clearing index value.");
        xbee.idx = 0;
        return;
    }

    usart_serial_getchar(XBEE_UART, &xbee.rx_data[xbee.idx++]);
}

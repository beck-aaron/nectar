#include "xbee.h"
#include "logger.h"
#include "xbee/api_frames.h"

xbee_t xbee = 
{
    .delimiter  = API_FRAME_DELIMITER,
    .test       = xbee_test,
    .encode     = xbee_encode,
    .decode     = xbee_decode,
    .transmit   = xbee_transmit,
    .receive    = xbee_receive,
    .force_receive = xbee_force_receive,
    .configure  = xbee_configure,
    .packet_length = 0,
}; 

void xbee_test(void)
{
    LOG(DEBUG_LEVEL, "Testing transmission request...");
    uint8_t test_data[] = "hello world";
    set_transmit_request(test_data, sizeof(test_data)-1, &xbee.api_frame);
    xbee.transmit();
}

void xbee_encode(void)
{
    vector_push(&xbee.delimiter, sizeof(uint8_t), &xbee.tx_buffer);
    vector_push(&xbee.length, sizeof(uint16_t), &xbee.tx_buffer);
    vector_push(&xbee.api_frame.cmdID, sizeof(uint8_t), &xbee.tx_buffer);

    // push api frame
    switch(xbee.api_frame.cmdID)
    {
        case AT_COMMAND:
            encode_at_command(&xbee.api_frame.at_command, &xbee.tx_buffer);
            break;

        case ZIGBEE_TRANSMIT_REQUEST:
            encode_transmit_request(&xbee.api_frame.transmit_request, &xbee.tx_buffer);
            break;

        case ZIGBEE_TRANSMIT_STATUS:
            break;
    }

    // calculate length, and push
    xbee.length = Swap16(xbee.tx_buffer.size - API_FRAME_HEADER_LENGTH);
    memcpy(&xbee.tx_buffer.data[API_FRAME_HEADER_IDX], &xbee.length, sizeof(uint16_t));


    // calculate checksum, and push
    xbee.checksum = 0;
    for (size_t i = API_FRAME_HEADER_LENGTH; i < xbee.tx_buffer.size; ++i)
        xbee.checksum += xbee.tx_buffer.data[i];

    xbee.checksum = 0xFF - xbee.checksum;
    vector_push(&xbee.checksum, sizeof(uint8_t), &xbee.tx_buffer);
}

void xbee_decode(void)
{
}

void xbee_configure(void)
{
    xbee.api_frame.cmdID = AT_COMMAND;
    xbee.api_frame.at_command.frame_id  = 0x01;

    xbee.api_frame.at_command.code      = VR;
    xbee.transmit();
    xbee.force_receive();

    xbee.api_frame.at_command.code      = VL;
    xbee.transmit();
    xbee.force_receive();

    xbee.api_frame.at_command.code      = NP;
    xbee.transmit();
    xbee.force_receive();
}

void xbee_transmit(void)
{
    xbee.encode();
    LOGHEX(TX_LEVEL, "Transmitting serial data to xbee", xbee.tx_buffer.data, xbee.tx_buffer.size);
    usart_serial_write_packet(XBEE_UART, xbee.tx_buffer.data, xbee.tx_buffer.size);
    vector_clear(&xbee.tx_buffer);
}

static bool xbee_is_rx_ready(void)
{
    // ensure we get the packet length, unless we already have it
    if(xbee.packet_length == 0)
    {
        while(xbee.rx_buffer.size < API_FRAME_HEADER_LENGTH); 
        xbee.packet_length = (xbee.rx_buffer.data[API_FRAME_HEADER_IDX] << 4) + xbee.rx_buffer.data[API_FRAME_HEADER_IDX+1];
    }

    return xbee.rx_buffer.size - API_FRAME_HEADER_LENGTH - 1 == xbee.packet_length;
}

void xbee_receive(void)
{
    if (xbee_is_rx_ready())
    {
        xbee.decode();
        LOGHEX(RX_LEVEL, "Received xbee serial data.", xbee.rx_buffer.data, xbee.rx_buffer.size);
        vector_clear(&xbee.rx_buffer);
        xbee.packet_length = 0;
    }
}

// force busy wait until usart finishes sending a payload.
void xbee_force_receive(void)
{
    while(!xbee_is_rx_ready());
    xbee.receive();
}

void xbee_uart_handler(void)
{
    usart_serial_getchar(XBEE_UART, &xbee.rx_buffer.data[xbee.rx_buffer.size++]);
}


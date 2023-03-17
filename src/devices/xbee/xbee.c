#include "xbee.h"
#include "dma.h"
#include "same70q21.h"
#include "xdmac.h"

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
    .state      = XBEE_IDLE,
}; 

void xbee_test(void)
{
    uint8_t test_data[] = "hello world";
//  LOG(DEBUG_LEVEL, "Testing transmission request: %s", test_data);
    set_transmit_request(test_data, sizeof(test_data)-1, &xbee.api_frame);

    // testing long response
    /*
    xbee.api_frame.cmdID = AT_COMMAND;
    xbee.api_frame.at_command.frame_id  = 0x01;
    xbee.api_frame.at_command.code      = VL;
    */
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
    SCB_CleanDCache();

    // reading the amount of bytes tranferred from dma
    xbee.rx_buffer.size = XBEE_MAX_RX - XDMAC->XDMAC_CHID[XBEE_CHANNEL_RX].XDMAC_CUBC;

    // if we don't have the length yet and the amount of data we have received is larger than the header
    if (xbee.rx_buffer.size < API_FRAME_HEADER_LENGTH)
    {
        return;
    }

    if (!(xbee.length == xbee.rx_buffer.size - 4))
    {
        // if the length specified in the header and the rx buffer size don't match, flush data from xdmac
        SCB_InvalidateDCache();
        xbee.length = (xbee.rx_buffer.data[API_FRAME_HEADER_IDX] << 4) + xbee.rx_buffer.data[API_FRAME_HEADER_IDX+1];
        xdmac_channel_software_flush_request(XDMAC, XBEE_CHANNEL_RX);
        return;
    }

    xdmac_disable_peripheral_to_memory(XBEE_CHANNEL_RX);
    LOGHEX(RX_LEVEL, "Received xbee serial data.", xbee.rx_buffer.data, xbee.rx_buffer.size);
    vector_clear(&xbee.rx_buffer);
    xbee.length = 0;
    xbee.state = XBEE_IDLE;
    return;
}

void xbee_configure(void)
{
    xbee.api_frame.cmdID = AT_COMMAND;
    xbee.api_frame.at_command.frame_id  = 0x01;

    xbee.api_frame.at_command.code      = VL;
    xbee.transmit();
//  xbee.force_receive();
}

void xbee_transmit(void)
{
    switch(xbee.state)
    {
        case XBEE_IDLE:
            xbee.encode();
            xdmac_configure_peripheral_to_memory(
                XBEE_UART,
                XBEE_HWID_RX,
                xbee.rx_buffer.data,
                XBEE_MAX_RX,
                XBEE_CHANNEL_RX
            );
            xdmac_enable_peripheral_to_memory(XBEE_CHANNEL_RX);
            LOGHEX(TX_LEVEL, "Transmitting serial data to xbee", xbee.tx_buffer.data, xbee.tx_buffer.size);
            usart_serial_write_packet(XBEE_UART, xbee.tx_buffer.data, xbee.tx_buffer.size);
            vector_clear(&xbee.tx_buffer);
            xbee.state = XBEE_RX_PENDING;
            return;

        case XBEE_RX_PENDING:
            return;
    }
}

void xbee_receive(void)
{
    switch(xbee.state)
    {
        case XBEE_IDLE:
            return;

        case XBEE_RX_PENDING:
            xbee.decode();
            return;
    }
}

// force busy wait until usart finishes sending a payload.
void xbee_force_receive(void)
{
    while(xbee.state == XBEE_RX_PENDING)
    {
        xbee.decode();
    }
}

// used for uart direct communication, not using xdmac
void xbee_uart_handler(void)
{
    usart_serial_getchar(XBEE_UART, &xbee.rx_buffer.data[xbee.rx_buffer.size++]);
}

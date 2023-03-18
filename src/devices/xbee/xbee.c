/*******************************************************************************
 * File:    xbee.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "xbee.h"

inline static void xbee_encode_packet_length(void);
inline static void xbee_decode_packet_length(void);
inline static void xbee_compute_checksum(void);
inline static bool xbee_verify_checksum(void);
inline static bool xbee_has_received_packet(void);

xbee_t xbee = 
{
    .delimiter  = API_FRAME_DELIMITER,
    .encode     = xbee_encode,
    .decode     = xbee_decode,
    .configure  = xbee_configure,
    .transmit   = xbee_transmit,
    .receive    = xbee_receive,
    .rx_state   = XBEE_IDLE,
    .tx_state   = XBEE_IDLE,
}; 

// interrupt handler used for direct uart communication. throws data into rx buffer
// todo: add check for buffer overflow
// need to enable interrupt for this to work
void xbee_uart_handler(void)
{
    usart_serial_getchar(XBEE_UART, &xbee.rx_buffer.data[xbee.rx_buffer.size++]);
}

void xbee_encode(void)
{
    vector_push(&xbee.delimiter, sizeof(uint8_t), &xbee.tx_buffer);
    vector_push(&xbee.length, sizeof(uint16_t), &xbee.tx_buffer);
    vector_push(&xbee.api_frame.cmdID, sizeof(uint8_t), &xbee.tx_buffer);

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

    xbee_encode_packet_length();
    xbee_compute_checksum();
}

void xbee_decode(void)
{
    if (!xbee_verify_checksum())
    {
        xbee.tx_state = XBEE_PENDING;
        return;
    }

    vector_pop(&xbee.api_frame.cmdID, sizeof(uint8_t), &xbee.tx_buffer);

    // push api frame
    switch(xbee.api_frame.cmdID)
    {
        case AT_COMMAND:
            decode_at_command(&xbee.api_frame.at_command, &xbee.rx_buffer);
            break;

        case ZIGBEE_TRANSMIT_REQUEST:
            decode_transmit_request(&xbee.api_frame.transmit_request, &xbee.rx_buffer);
            break;

        case ZIGBEE_TRANSMIT_STATUS:
            decode_transmit_status(&xbee.api_frame.transmit_status, &xbee.rx_buffer);
            break;
    }

    xbee.tx_state = XBEE_IDLE;
}

void xbee_configure(void)
{
    xbee.api_frame.cmdID = AT_COMMAND;
    xbee.api_frame.at_command.frame_id  = 0x01;
    xbee.api_frame.at_command.code      = VL;
//  xbee.transmit();
}

void xbee_transmit(void)
{
    switch(xbee.tx_state)
    {
        case XBEE_IDLE:
            xbee.encode();
            LOGHEX(TX_LEVEL, "Transmitting serial data to xbee", xbee.tx_buffer.data, xbee.tx_buffer.size);
            // todo: replace with dma transfer
            // configure memory to peripheral
            // enable memory to peripheral
            usart_serial_write_packet(XBEE_UART, xbee.tx_buffer.data, xbee.tx_buffer.size);
            xbee.tx_state = XBEE_PENDING;
            return;

        case XBEE_PENDING:
            if (xbee.rx_state == XBEE_IDLE)
            {
                // transmit tx buffer again
                LOGHEX(TX_LEVEL, "Retransmitting stale data to xbee", xbee.tx_buffer.data, xbee.tx_buffer.size);
                usart_serial_write_packet(XBEE_UART, xbee.tx_buffer.data, xbee.tx_buffer.size);
            }
            // clear tx buffer and return
            vector_clear(&xbee.tx_buffer);
            xbee.tx_state = XBEE_IDLE;
            return;
    }
}

void xbee_receive(void)
{
    switch(xbee.rx_state)
    {
        case XBEE_IDLE:
            xdmac_configure_peripheral_to_memory(XBEE_UART, XBEE_HWID_RX,
                    xbee.rx_buffer.data, XBEE_MAX_RX, XBEE_CHANNEL_RX);
            xdmac_channel_enable(XDMAC, XBEE_CHANNEL_RX);
            xbee.rx_state = XBEE_PENDING;
            return;

        case XBEE_PENDING:
            if (!xbee_has_received_packet())
            {
                xdmac_channel_software_flush_request(XDMAC, XBEE_CHANNEL_RX);
                return;
            }

            xdmac_channel_disable(XDMAC, XBEE_CHANNEL_RX);
            LOGHEX(RX_LEVEL, "Received serial data from xbee", xbee.rx_buffer.data, xbee.rx_buffer.size);
            xbee.decode(); // will transition tx to IDLE if transmit status is decoded and message was submitted
            xbee.rx_state = XBEE_IDLE;
            return;
    }
}

// calculate length
inline static void xbee_encode_packet_length(void)
{
    xbee.length = Swap16(xbee.tx_buffer.size - API_FRAME_HEADER_LENGTH);
    memcpy(&xbee.tx_buffer.data[API_FRAME_HEADER_IDX], &xbee.length, sizeof(uint16_t));
}

//TODO: test this function
inline static void xbee_decode_packet_length(void)
{
    xbee.length =
        (xbee.rx_buffer.data[API_FRAME_HEADER_IDX] << 4) +
        (xbee.rx_buffer.data[API_FRAME_HEADER_IDX+1]);
}

inline static void xbee_compute_checksum(void)
{
    xbee.checksum = 0;
    for (size_t i = API_FRAME_HEADER_LENGTH; i < xbee.tx_buffer.size; ++i)
        xbee.checksum += xbee.tx_buffer.data[i];
    xbee.checksum = 0xFF - xbee.checksum;
    vector_push(&xbee.checksum, sizeof(uint8_t), &xbee.tx_buffer);
}

inline static bool xbee_verify_checksum(void)
{
    xbee.checksum = 0;
    for (size_t i = API_FRAME_HEADER_LENGTH; i < xbee.rx_buffer.size; ++i)
        xbee.checksum += xbee.rx_buffer.data[i];

    return xbee.checksum == 0xFF;
}

inline static bool xbee_has_received_packet(void)
{
    // TODO: make this call into a xdmac function, with a ublock length and channel as parameters
    // reading the amount of bytes tranferred from dma
    xbee.rx_buffer.size = XBEE_MAX_RX - XDMAC->XDMAC_CHID[XBEE_CHANNEL_RX].XDMAC_CUBC;

    // we haven't received the header yet
    if (xbee.rx_buffer.size < API_FRAME_HEADER_LENGTH)
    {
        return false;
    }

    // if we haven't gotten the packet length yet, do so
    SCB_CleanInvalidateDCache();
    if (xbee.length == 0) xbee_decode_packet_length();

    // our packet length does not match our buffer size minus our checksum
    if (xbee.length  != xbee.rx_buffer.size - 4)
    {
        return false;
    }

    return true;
}

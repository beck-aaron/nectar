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
    .length     = 0,
    .transmit   = xbee_transmit,
    .receive    = xbee_receive,
    .encode     = xbee_encode,
    .decode     = xbee_decode,
    .configure  = xbee_configure,
    .rx_state   = SERIAL_IDLE,
    .tx_state   = SERIAL_IDLE,
    .state      = DEVICE_DISCONNECTED,
}; 

// interrupt handler used for direct uart communication. throws data into rx buffer
// todo: add check for buffer overflow
// need to enable interrupt for this to work
void xbee_uart_handler(void)
{
    usart_serial_getchar(XBEE_UART, &xbee.rx_buffer.data[xbee.rx_buffer.size++]);
}

// if our tx is pending, we just send the encoded buffer again.
// No need for encoding again.
void xbee_transmit(void)
{
    if (xbee.state == DEVICE_DISCONNECTED)
        return;

    switch(xbee.tx_state)
    {
        case SERIAL_IDLE:
            xbee.encode();
            LOGHEX(TX_LEVEL, "[XBEE] Transmitting serial data in tx buffer.", xbee.tx_buffer.data, xbee.tx_buffer.size);
            // todo: replace with dma transfer
            // configure memory to peripheral
            // enable memory to peripheral
            usart_serial_write_packet(XBEE_UART, xbee.tx_buffer.data, xbee.tx_buffer.size);
            xbee.tx_state = SERIAL_PENDING;
            LOG(DEBUG_LEVEL, "[XBEE] tx state change: IDLE -> PENDING");
            return;

        case SERIAL_PENDING:
            if (xbee.rx_state == SERIAL_IDLE)
            {
                // transmit tx buffer again
                LOGHEX(TX_LEVEL, "[XBEE] Retransmitting stale data in tx buffer.", xbee.tx_buffer.data, xbee.tx_buffer.size);
                usart_serial_write_packet(XBEE_UART, xbee.tx_buffer.data, xbee.tx_buffer.size);
                return;
            }
            return;
    }
}

void xbee_receive(void)
{
    if (xbee.state == DEVICE_DISCONNECTED)
        return;

    switch(xbee.rx_state)
    {
        case SERIAL_IDLE:
            xdmac_configure_peripheral_to_memory(XBEE_UART, XBEE_HWID_RX,
                    xbee.rx_buffer.data, XBEE_MAX_RX, XBEE_CHANNEL_RX);
            SCB_CleanDCache();
            xdmac_channel_enable(XDMAC, XBEE_CHANNEL_RX);
            xbee.rx_state = SERIAL_PENDING;
            LOG(DEBUG_LEVEL, "[XBEE] rx state change: IDLE -> PENDING");
            xbee.length = 0;
            return;

        case SERIAL_PENDING:
            SCB_CleanInvalidateDCache();
            if (!xbee_has_received_packet())
            {
                xdmac_flush_channel(XBEE_CHANNEL_RX);
                return;
            }

            xdmac_channel_disable(XDMAC, XBEE_CHANNEL_RX);
            LOGHEX(RX_LEVEL, "[XBEE] Received serial data from xbee", xbee.rx_buffer.data, xbee.rx_buffer.size);
            xbee.decode();
            vector_clear(&xbee.rx_buffer);
            xbee.rx_state = SERIAL_IDLE;
            LOG(DEBUG_LEVEL, "[XBEE] rx state change: PENDING -> IDLE");
            return;
    }
}

void xbee_encode(void)
{
    vector_push(&xbee.delimiter, sizeof(uint8_t), &xbee.tx_buffer);
    vector_push(&xbee.length, sizeof(uint16_t), &xbee.tx_buffer);
    vector_push(&xbee.api_frame.cmdID, sizeof(uint8_t), &xbee.tx_buffer);

    LOG(ENCODER_LEVEL, "[XBEE] %-10s := %s", "api frame", api_frame_str(xbee.api_frame.cmdID));
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

    LOG(ENCODER_LEVEL, "[XBEE] %-10s := %#0X", "length", xbee.length);
    LOG(ENCODER_LEVEL, "[XBEE] %-10s := %#0X", "checksum", xbee.checksum);
}

void xbee_decode(void)
{
    if (!xbee_verify_checksum())
    {
        LOG(ERROR_LEVEL, "[XBEE] failed to verify checksum");
        xbee.tx_state = SERIAL_PENDING;
        LOG(DEBUG_LEVEL, "[XBEE] tx state change: IDLE -> PENDING");
        return;
    }
    
    // skip over frame header
    xbee.rx_buffer.iterator += API_FRAME_HEADER_LENGTH;
    vector_pop(&xbee.api_frame.cmdID, sizeof(uint8_t), &xbee.rx_buffer);

    LOG(DECODER_LEVEL, "[XBEE] %s := %s", "api frame", api_frame_str(xbee.api_frame.cmdID));
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
            if (xbee.api_frame.transmit_status.delivery_status == SUCCESS)
            {
                vector_clear(&xbee.tx_buffer);
                xbee.tx_state = SERIAL_IDLE;
                LOG(DEBUG_LEVEL, "[XBEE] tx state change: PENDING -> IDLE");
            }
            break;
    }
}

void xbee_configure(void)
{
    xbee.api_frame.cmdID = AT_COMMAND;
    xbee.api_frame.at_command.frame_id  = 0x01;
    xbee.api_frame.at_command.code      = VL;
//  xbee.transmit();
}

// calculate length
inline static void xbee_encode_packet_length(void)
{
    xbee.length = Swap16(xbee.tx_buffer.size - API_FRAME_HEADER_LENGTH);
    memcpy(&xbee.tx_buffer.data[API_FRAME_HEADER_IDX], &xbee.length, sizeof(uint16_t));
    xbee.length = Swap16(xbee.length);
}

//TODO: test this function
inline static void xbee_decode_packet_length(void)
{
    xbee.length = (xbee.rx_buffer.data[API_FRAME_HEADER_IDX] << 4) +
        xbee.rx_buffer.data[API_FRAME_HEADER_IDX+1];
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
    xbee.rx_buffer.size = xdmac_get_bytes_transferred(XBEE_MAX_RX, XBEE_CHANNEL_RX);

    // we haven't received the header yet
    if (xbee.rx_buffer.size < API_FRAME_HEADER_LENGTH)
    {
        return false;
    }

    if (xbee.length == 0) xbee_decode_packet_length();

    // our packet length does not match our buffer size minus our checksum
    if (xbee.length  != xbee.rx_buffer.size - 4)
    {
        LOG(WARNING_LEVEL, "[XBEE] length not equal to buffer size");
        return false;
    }

    return true;
}

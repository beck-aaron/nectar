/*******************************************************************************
 * File:    xbee.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 * TODO: remove global xbee struct and fix all functions to take in an xbee
 * struct as a parameter
 *
 ******************************************************************************/
#include "xbee.h"

// private functions here
inline static void xbee_configure(xbee_t *xbee);
inline static void xbee_encode_packet_length(xbee_t* xbee);
inline static void xbee_decode_packet_length(xbee_t* xbee);
inline static void xbee_compute_checksum(xbee_t* xbee);
inline static bool xbee_verify_checksum(xbee_t* xbee);
inline static bool xbee_has_received_packet(xbee_t* xbee);

void xbee_init(xbee_t* xbee)
{
    xbee->delimiter = API_FRAME_DELIMITER;
    xbee->length = 0;
    xbee->tx_state = SERIAL_IDLE;
    xbee->rx_state = SERIAL_IDLE;
    xbee->state = DEVICE_CONNECTED;
    vector_init(XBEE_MAX_TX, &xbee->tx_buffer);
    vector_init(XBEE_MAX_RX, &xbee->rx_buffer);
    serial_uart_init(XBEE);

    xbee_configure(xbee);

    switch(xbee->state)
    {
        case DEVICE_DISCONNECTED:
            LOG(ERROR_LEVEL, "Unable to communicate with xbee, verify it is plugged in!");
            break;

        case DEVICE_CONNECTED:
            LOG(DEBUG_LEVEL, "Connected to xbee over serial interface.");
            break;
    }
}

// configure xbee here
// setup timeout for setting xbee state to DEVICE_CONNECTED
inline static void xbee_configure(xbee_t* xbee)
{
    timer xbee_timeout = timer_init(5);
    xbee->api_frame.cmdID = AT_COMMAND;
    xbee->api_frame.at_command.frame_id  = 0x01;
    xbee->api_frame.at_command.code      = VL;

    xdmac_configure_peripheral_to_memory(XBEE_UART, XBEE_HWID_RX,
                xbee->rx_buffer.data, XBEE_MAX_RX, XBEE_CHANNEL_RX);
    xdmac_enable_channel(XBEE_CHANNEL_RX);
    xbee_transmit(xbee);

    xbee->length = 0;
    xbee->rx_state = SERIAL_PENDING;

    while(!(xbee_has_received_packet(xbee) || timer_expired(&xbee_timeout)))
    {
        xdmac_flush_channel(XBEE_CHANNEL_RX);
    }

    xbee_receive(xbee);

    if (!xbee_has_received_packet(xbee))
        xbee->state = DEVICE_DISCONNECTED;

    xbee->tx_state = SERIAL_IDLE;
    xbee->rx_state = SERIAL_IDLE;
    xbee->length = 0;
    vector_clear(&xbee->tx_buffer);
    vector_clear(&xbee->rx_buffer);
}

// if our tx is pending, we just send the encoded buffer again.
// No need for encoding again.
void xbee_transmit(xbee_t* xbee)
{
    switch(xbee->tx_state)
    {
        case SERIAL_IDLE:
            xbee_encode(xbee);
            LOGHEX(TX_LEVEL, "[XBEE] Transmitting serial data in tx buffer.", xbee->tx_buffer.data, xbee->tx_buffer.size);
            // todo: replace with dma transfer
            // configure memory to peripheral
            // enable memory to peripheral
            usart_serial_write_packet(XBEE_UART, xbee->tx_buffer.data, xbee->tx_buffer.size);
            xbee->tx_state = SERIAL_PENDING;
            LOG(DEBUG_LEVEL, "[XBEE] tx state change: IDLE -> PENDING");
            break;

        case SERIAL_PENDING:
            if (xbee->rx_state == SERIAL_IDLE)
            {
                // transmit tx buffer again
                LOGHEX(TX_LEVEL, "[XBEE] Retransmitting stale data in tx buffer.", xbee->tx_buffer.data, xbee->tx_buffer.size);
                usart_serial_write_packet(XBEE_UART, xbee->tx_buffer.data, xbee->tx_buffer.size);
            }
            break;
    }
}

void xbee_receive(xbee_t* xbee)
{
    switch(xbee->rx_state)
    {
        case SERIAL_IDLE:
            xbee->length = 0;
            xbee->rx_state = SERIAL_PENDING;
            LOG(DEBUG_LEVEL, "[XBEE] rx state change: IDLE -> PENDING");
            SCB_CleanDCache();
            xdmac_configure_peripheral_to_memory(XBEE_UART, XBEE_HWID_RX,
                    xbee->rx_buffer.data, XBEE_MAX_RX, XBEE_CHANNEL_RX);
            xdmac_channel_enable(XDMAC, XBEE_CHANNEL_RX);
        // fallthrough to pending state on same cycle

        case SERIAL_PENDING:
            if (!xbee_has_received_packet(xbee))
            { 
                if (xbee->length == 0)
                    return;

                xdmac_flush_channel(XBEE_CHANNEL_RX);
                return;
            }

            xdmac_channel_disable(XDMAC, XBEE_CHANNEL_RX);
            LOGHEX(RX_LEVEL, "[XBEE] Received serial data from xbee", xbee->rx_buffer.data, xbee->rx_buffer.size);
            xbee_decode(xbee);
            vector_clear(&xbee->rx_buffer);
            xbee->rx_state = SERIAL_IDLE;
            LOG(DEBUG_LEVEL, "[XBEE] rx state change: PENDING -> IDLE");
            return;
    }
}

void xbee_encode(xbee_t* xbee)
{
    LOG(ENCODER_LEVEL, "[XBEE] Encoding api frame.");

    vector_push(&xbee->delimiter, sizeof(uint8_t), &xbee->tx_buffer);
    vector_push(&xbee->length, sizeof(uint16_t), &xbee->tx_buffer);
    vector_push(&xbee->api_frame.cmdID, sizeof(uint8_t), &xbee->tx_buffer);

    LOG(ENCODER_LEVEL, "[XBEE] %-10s := %s", "api frame", api_frame_str(xbee->api_frame.cmdID));
    switch(xbee->api_frame.cmdID)
    {
        case AT_COMMAND:
            xbee_encode_at_command(&xbee->api_frame.at_command, &xbee->tx_buffer);
            break;

        case ZIGBEE_TRANSMIT_REQUEST:
            xbee_encode_transmit_request(&xbee->api_frame.transmit_request, &xbee->tx_buffer);
            break;

        case ZIGBEE_TRANSMIT_STATUS:
            break;
    }

    xbee_encode_packet_length(xbee);
    xbee_compute_checksum(xbee);

    LOG(ENCODER_LEVEL, "[XBEE] %-10s := 0x%02X", "length", xbee->length);
    LOG(ENCODER_LEVEL, "[XBEE] %-10s := 0x%02X", "checksum", xbee->checksum);
}

void xbee_decode(xbee_t* xbee)
{
    LOG(DECODER_LEVEL, "[XBEE] Decoding api frame.");

    if (!xbee_verify_checksum(xbee))
    {
        LOG(ERROR_LEVEL, "[XBEE] failed to verify checksum");
        xbee->tx_state = SERIAL_PENDING;
        LOG(DEBUG_LEVEL, "[XBEE] tx state change: IDLE -> PENDING");
        return;
    }

    xbee->rx_buffer.iterator += API_FRAME_HEADER_LENGTH;
    vector_pop(&xbee->api_frame.cmdID, sizeof(uint8_t), &xbee->rx_buffer);

    LOG(DECODER_LEVEL, "[XBEE] %s := %s", "api frame", api_frame_str(xbee->api_frame.cmdID));

    switch(xbee->api_frame.cmdID)
    {
        case AT_COMMAND:
            xbee_decode_at_command(&xbee->api_frame.at_command, &xbee->rx_buffer);
            break;

        case ZIGBEE_TRANSMIT_REQUEST:
            xbee_decode_transmit_request(&xbee->api_frame.transmit_request, &xbee->rx_buffer);
            break;

        case ZIGBEE_TRANSMIT_STATUS:
            xbee_decode_transmit_status(&xbee->api_frame.transmit_status, &xbee->rx_buffer);
            if (xbee->api_frame.transmit_status.delivery_status != SUCCESS)
                return;
    }

    vector_clear(&xbee->tx_buffer);
    xbee->tx_state = SERIAL_IDLE;
    LOG(DEBUG_LEVEL, "[XBEE] tx state change: PENDING -> IDLE");
}

inline static void xbee_encode_packet_length(xbee_t* xbee)
{
    xbee->length = Swap16(xbee->tx_buffer.size - API_FRAME_HEADER_LENGTH);
    memcpy(&xbee->tx_buffer.data[API_FRAME_HEADER_IDX], &xbee->length, sizeof(uint16_t));
    xbee->length = Swap16(xbee->length);
}

inline static void xbee_decode_packet_length(xbee_t* xbee)
{
    xbee->length = (xbee->rx_buffer.data[API_FRAME_HEADER_IDX] << 4) +
        xbee->rx_buffer.data[API_FRAME_HEADER_IDX+1];
}

inline static void xbee_compute_checksum(xbee_t* xbee)
{
    xbee->checksum = 0;
    for (size_t i = API_FRAME_HEADER_LENGTH; i < xbee->tx_buffer.size; ++i)
        xbee->checksum += xbee->tx_buffer.data[i];
    xbee->checksum = 0xFF - xbee->checksum;
    vector_push(&xbee->checksum, sizeof(uint8_t), &xbee->tx_buffer);
}

inline static bool xbee_verify_checksum(xbee_t* xbee)
{
    xbee->checksum = 0;
    for (size_t i = API_FRAME_HEADER_LENGTH; i < xbee->rx_buffer.size; ++i)
        xbee->checksum += xbee->rx_buffer.data[i];

    return xbee->checksum == 0xFF;
}

inline static bool xbee_has_received_packet(xbee_t* xbee)
{
    SCB_CleanInvalidateDCache();
    xbee->rx_buffer.size = xdmac_get_bytes_transferred(XBEE_MAX_RX, XBEE_CHANNEL_RX);

    // we haven't received the header yet
    if (xbee->rx_buffer.size < API_FRAME_HEADER_LENGTH)
    {
        return false;
    }

    if (xbee->length == 0) xbee_decode_packet_length(xbee);

    // our packet length does not match our buffer size minus our checksum
    if (xbee->length != xbee->rx_buffer.size - 4)
    {
        LOG(WARNING_LEVEL, "[XBEE] frame size (0x%02X) not equal to received bytes (0x%02X)", xbee->length + 4, xbee->rx_buffer.size);
        return false;
    }

    return true;
}

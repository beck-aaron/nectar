#include "coz_ir.h"

// local functions
static void coz_ir_configure(coz_ir_t* coz_ir)
{
    timer coz_ir_timeout = timer_init(5);

    while (!timer_expired(&coz_ir_timeout))
    {
        // attempt to set device to polling mode
    }
}

void coz_ir_init(coz_ir_t* coz_ir)
{
    serial_uart_init(COZ_IR);
    vector_init(COZ_IR_MAX_TX, &coz_ir->tx_buffer);
    vector_init(COZ_IR_MAX_RX, &coz_ir->rx_buffer);
    coz_ir_configure(coz_ir);

    // setup dma channel for data reception

    // configure coz_ir here
    LOG(DEBUG_LEVEL, "Initialized serial interface for coz-ir.");
}

void coz_ir_transmit(coz_ir_t* coz_ir)
{
    switch(coz_ir->rx_state)
    {
        case SERIAL_IDLE:
            coz_ir_encode(coz_ir);
            usart_serial_write_packet(COZ_IR_UART, coz_ir->tx_buffer.data, coz_ir->tx_buffer.size);
            coz_ir->rx_state = SERIAL_PENDING;
            vector_clear(&coz_ir->tx_buffer);
            break;

        case SERIAL_PENDING:
            break;
    }
    // request co2, temperature, & humidity
    // we know the length of these messages

}

void coz_ir_receive(coz_ir_t* coz_ir)
{
    // use serial state to determine if we have successfully received
    // wait until DMA has received CO2, TEMPERATURE, & HUMIDITY
    // in other words, wait until COZ_IR_RESPONSE_LENGTH bytes have been transferred
    if (coz_ir->rx_state == SERIAL_PENDING)
    {
        // TODO: check to see if we have received all the necessary bytes
        coz_ir_decode(coz_ir);
    }

    // if we are in IDLE, reset DMA channel
}

void coz_ir_encode(coz_ir_t* coz_ir)
{
    coz_ir_encode_message(COZ_IR_GET_FILTERED_CO2_PPM, &coz_ir->tx_buffer);
    coz_ir_encode_message(COZ_IR_GET_TEMPERATURE, &coz_ir->tx_buffer);
    coz_ir_encode_message(COZ_IR_GET_HUMIDITY, &coz_ir->tx_buffer);
}

void coz_ir_decode(coz_ir_t* coz_ir)
{
}

void coz_ir_get_data(coz_ir_t* coz_ir, coz_ir_data_t type)
{
}

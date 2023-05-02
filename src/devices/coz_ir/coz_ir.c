/*******************************************************************************
 * File:    coz_ir.c
 * Author:  Aaron Beck
 *
 * Description: driver for the COZ-IR A sensor.  This is a very barebones
 * driver that polls the device for temperature, humidity, and co2 ppm.  No
 * error checking is performed.  The device is assumed to be plugged in. There
 * is much room for improvement on this driver. DMA is not used at the moment,
 * the only commands used on this device are very short and do not warrant DMA
 * currently although this could be implemented in the future.
 *
 ******************************************************************************/
#include "coz_ir.h"
#include "commands.h"

static void coz_ir_configure(coz_ir_t* coz_ir)
{
    // TODO
    // determine if the sensor is plugged in,
    // refer to the xbee configure method for info
}

void coz_ir_init(coz_ir_t* coz_ir)
{
    serial_uart_init(COZ_IR);
    delay_s(3);

    vector_init(COZ_IR_MAX_TX, &coz_ir->tx_buffer);
    vector_init(COZ_IR_MAX_RX, &coz_ir->rx_buffer);
    coz_ir_configure(coz_ir);
    coz_ir->rx_state = SERIAL_IDLE;

    // configure coz_ir here
    LOG(DEBUG_LEVEL, "Initialized serial interface for coz-ir.");
}

void coz_ir_transmit(coz_ir_t* coz_ir)
{
    LOGHEX(TX_LEVEL, "[COZ-IR] Transmitting serial data in tx buffer.", coz_ir->tx_buffer.data, coz_ir->tx_buffer.size);
    usart_serial_write_packet(COZ_IR_UART, coz_ir->tx_buffer.data, coz_ir->tx_buffer.size);
}

void coz_ir_receive(coz_ir_t* coz_ir)
{
    usart_serial_read_packet(COZ_IR_UART, coz_ir->rx_buffer.data, 10);
    LOGHEX(RX_LEVEL, "[COZ-IR] Received serial data in rx buffer.", coz_ir->rx_buffer.data, 10);
}

void coz_ir_decode(coz_ir_t* coz_ir)
{
    LOGHEX(DECODER_LEVEL, "coz receive buffer", coz_ir->rx_buffer.data, COZ_IR_MAX_RX);
}

uint16_t coz_ir_get_ppm(coz_ir_t* coz_ir)
{
    coz_ir_encode_message(COZ_IR_GET_FILTERED_CO2_PPM, &coz_ir->tx_buffer);
    coz_ir_transmit(coz_ir);
    coz_ir_receive(coz_ir);

    uint8_t null = '\0';
    vector_push(&null, sizeof(null), &coz_ir->rx_buffer);
    coz_ir->co2_ppm = strtol((char*)coz_ir->rx_buffer.data+4, NULL, BASE_TEN);
//  LOG(DEBUG_LEVEL, "[COZ-IR] co2 ppm: %u", coz_ir->co2_ppm);

    vector_clear(&coz_ir->tx_buffer);
    vector_clear(&coz_ir->rx_buffer);
    return coz_ir->co2_ppm;
}

float coz_ir_get_temp(coz_ir_t* coz_ir)
{
    coz_ir_encode_message(COZ_IR_GET_TEMPERATURE, &coz_ir->tx_buffer);
    coz_ir_transmit(coz_ir);
    coz_ir_receive(coz_ir);

    uint8_t null = '\0';
    vector_push(&null, sizeof(null), &coz_ir->rx_buffer);
    coz_ir->temperature = (strtol((char*)coz_ir->rx_buffer.data+4, NULL, BASE_TEN) - 1000.0)/10;
    vector_clear(&coz_ir->tx_buffer);
    vector_clear(&coz_ir->rx_buffer);
    return coz_ir->temperature;
}

float coz_ir_get_humidity(coz_ir_t* coz_ir)
{
    coz_ir_encode_message(COZ_IR_GET_HUMIDITY, &coz_ir->tx_buffer);
    coz_ir_transmit(coz_ir);
    coz_ir_receive(coz_ir);

    uint8_t null = '\0';
    vector_push(&null, sizeof(null), &coz_ir->rx_buffer);
    coz_ir->humidity = strtol((char*)coz_ir->rx_buffer.data+4, NULL, BASE_TEN)/10.0;
    vector_clear(&coz_ir->tx_buffer);
    vector_clear(&coz_ir->rx_buffer);
    return coz_ir->humidity;
}

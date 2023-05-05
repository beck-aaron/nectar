/*******************************************************************************
 * File:    nectar.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "nectar.h"

// local functions
static void nectar_payload_init(nectar_payload_t* payload);
static size_t nectar_calculate_subpayload_size(uint16_t datapoints);

void nectar_init(nectar_t* nectar)
{
    srand((unsigned) time(NULL)); // seeding rand only necessary while using fake data

#if LOG_EVERYTHING
    logger_init();
#endif

    xdmac_init();

    nectar->subpayload_index = 0;
    vector_init(MAX_PAYLOAD_SIZE, &nectar->encoded_buffer);
    queue_init(&nectar->payload_queue, nectar->payload_buffer, sizeof(nectar_payload_t), MAX_PAYLOADS);


    xbee_init(&nectar->xbee);
    coz_ir_init(&nectar->coz_ir);

    LED_On(LED0);
    LOG(DEBUG_LEVEL, "[Nectar] Successfully initialized all drivers and connected datapoints.");
    LOG(DEBUG_LEVEL, "[Nectar] Starting data collection and transmission procedure.");
}

static void nectar_payload_init(nectar_payload_t* payload)
{
    payload->full = false;
    payload->size = 2; // includes the delimiter and subpayload count to start
    payload->delimiter = 0xAF; // constant
    payload->datapoints = 0xFF00; // TODO: poll CONNECTED devices to compute this
    payload->subpayload_count = 0; // number of collected subpayloads
    payload->subpayload_size = nectar_calculate_subpayload_size(payload->datapoints);
}

static size_t nectar_calculate_subpayload_size(uint16_t datapoints)
{
    size_t size = 2; // add delimiter and payload count to size
    if (datapoints & NECTAR_TIMESTAMP)   size += sizeof(uint32_t);
    if (datapoints & NECTAR_CO2_PPM)     size += sizeof(uint16_t);
    if (datapoints & NECTAR_U_VECTOR)    size += sizeof(float);
    if (datapoints & NECTAR_V_VECTOR)    size += sizeof(float);
    if (datapoints & NECTAR_W_VECTOR)    size += sizeof(float);
    if (datapoints & NECTAR_TEMPERATURE) size += sizeof(float);
    if (datapoints & NECTAR_HUMIDITY)    size += sizeof(float);
    if (datapoints & NECTAR_PRESSURE)    size += sizeof(float);
    return size;
}

// prepare transmission for payload in front of queue
inline void nectar_transmit(nectar_t* nectar)
{
    nectar_payload_t* payload = (nectar_payload_t*)nectar->payload_queue.front;
    if (!payload->full)
    {
        switch(nectar->xbee.tx_state)
        {
            case SERIAL_IDLE:
                LOG(DEBUG_LEVEL, "[Nectar] Payload not full, skipping transmission until more data has been collected.");
                return;

            case SERIAL_PENDING: // retransmit stale data
                xbee_transmit(&nectar->xbee);
                return;
        }
    }

    // once we encode a payload, set transmit request for xbee and call
    // xbee_transmit, the payload delivery is taken care of.
    // We need to call xbee_transmit every cycle, but the xbee driver will
    // ensure that the data is sent
    if (nectar->xbee.tx_state == SERIAL_IDLE)
    {
        nectar_encode_payload(payload, &nectar->encoded_buffer);
        xbee_set_transmit_request(nectar->encoded_buffer.data, nectar->encoded_buffer.size, &nectar->xbee.api_frame);
        xbee_transmit(&nectar->xbee);
        queue_pop(&nectar->payload_queue);
        LOG(DEBUG_LEVEL, "[NECTAR] Popping first payload off queue");
        return;
    }

    // transmit fresh payload if state is SERIAL_IDLE, otherwise retransmit stale payload
    xbee_transmit(&nectar->xbee);
}

void nectar_receive(nectar_t* nectar)
{
    xbee_receive(&nectar->xbee);
}

// compile data to payload at back of queue
void nectar_compile(nectar_t* nectar)
{
    if (nectar->payload_queue.size == 0) {
        LOG(DEBUG_LEVEL, "[NECTAR] Pushing new payload onto queue");
        queue_push(&nectar->payload_queue);
        nectar_payload_init((nectar_payload_t*)nectar->payload_queue.front);
    }

    nectar_payload_t* payload = (nectar_payload_t*)nectar->payload_queue.back;
    if (payload->size + payload->subpayload_size >= MAX_PAYLOAD_SIZE)
    {
        payload->full = true;
        if (nectar->payload_queue.full) {
            LOG(DEBUG_LEVEL, "[NECTAR] payload queue full, returning");
            return;
        }
        LOG(DEBUG_LEVEL, "[NECTAR] Pushing new payload onto queue");
        queue_push(&nectar->payload_queue);
        payload = (nectar_payload_t*)nectar->payload_queue.back;
        nectar_payload_init(payload);
    }

    LOG(DEBUG_LEVEL, "[NECTAR] Compiling payload at address: %#0X", nectar->payload_queue.back);

    // determine which subpayload we need to edit
    nectar_subpayload_t* subpayload = &payload->subpayloads[payload->subpayload_count];

    // set all values in subpayload
    uint16_t datapoints = payload->datapoints;
    if (datapoints & NECTAR_TIMESTAMP) subpayload->index = nectar->subpayload_index++;
    if (datapoints & NECTAR_CO2_PPM) subpayload->co2_ppm = coz_ir_get_ppm(&nectar->coz_ir);
    if (datapoints & NECTAR_TEMPERATURE) subpayload->temperature = coz_ir_get_temp(&nectar->coz_ir);
    if (datapoints & NECTAR_HUMIDITY) subpayload->humidity = coz_ir_get_humidity(&nectar->coz_ir);
    if (datapoints & NECTAR_U_VECTOR) subpayload->u_vector = (rand() % 20) - 10;
    if (datapoints & NECTAR_V_VECTOR) subpayload->v_vector = (rand() % 20) - 10;
    if (datapoints & NECTAR_W_VECTOR) subpayload->w_vector = (rand() % 20) - 10;
    if (datapoints & NECTAR_PRESSURE) subpayload->pressure = (rand() % 20) - 10; // FIX THIS

    // increment payload size by the subpayload size
    payload->size += payload->subpayload_size;
    payload->subpayload_count++;
}

void nectar_encode_payload(nectar_payload_t* payload, vector_t* buffer)
{
    LOG(ENCODER_LEVEL, "[NECTAR] Encoding application payload.");
    vector_clear(buffer);
    vector_push(&payload->delimiter, sizeof(uint8_t), buffer);
    vector_push(&payload->subpayload_count, sizeof(uint8_t), buffer);

    LOG(ENCODER_LEVEL, "[NECTAR] %-16s:= 0x%02X", "delimiter", payload->delimiter);
    LOG(ENCODER_LEVEL, "[NECTAR] %-16s:= %u", "subpayload count", payload->subpayload_count);
    LOGBITS(ENCODER_LEVEL, "[NECTAR] device bitmask  := ", &payload->datapoints, sizeof(uint16_t));
    LOG(ENCODER_LEVEL, "[NECTAR] \tEncoding subpayloads.");

    for (uint32_t i = 0; i < payload->subpayload_count; i++) {
        nectar_encode_subpayload(&payload->subpayloads[i], payload->datapoints, buffer);
    }
}

void nectar_encode_subpayload(nectar_subpayload_t* subpayload, uint16_t datapoints, vector_t* buffer)
{
    vector_push(&datapoints, sizeof(uint16_t), buffer);

    if (datapoints & NECTAR_TIMESTAMP) {
        vector_push(&subpayload->index, sizeof(time_t), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %lu", "index", subpayload->index);
    }
    if (datapoints & NECTAR_CO2_PPM) {
        vector_push(&subpayload->co2_ppm, sizeof(uint16_t), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %u", "co2 ppm", subpayload->co2_ppm);
    }
    if (datapoints & NECTAR_U_VECTOR) {
        vector_push(&subpayload->u_vector, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "u-vector", subpayload->u_vector);
    }
    if (datapoints & NECTAR_V_VECTOR) {
        vector_push(&subpayload->v_vector, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "v-vector", subpayload->v_vector);
    }
    if (datapoints & NECTAR_W_VECTOR) {
        vector_push(&subpayload->w_vector, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "w-vector", subpayload->w_vector);
    }
    if (datapoints & NECTAR_TEMPERATURE) {
        vector_push(&subpayload->temperature, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f (°C)", "temperature", subpayload->temperature);
    }
    if (datapoints & NECTAR_HUMIDITY) {
        vector_push(&subpayload->humidity, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f (%%RH)", "humidity" , subpayload->humidity);
    }
    if (datapoints & NECTAR_PRESSURE) {
        vector_push(&subpayload->pressure, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "pressure", subpayload->pressure);
    }
}

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

inline void nectar_init(nectar_t* nectar)
{
    srand((unsigned) time(NULL));

    nectar_payload_init(&nectar->payload);
    vector_init(256, &nectar->payload_buffer); // encoded payload buffer

    logger_init();
    xdmac_init();
    devices_init(
        &nectar->xbee,
        &nectar->coz_ir,
        &nectar->telaire,
        &nectar->trisonica
    );

    LED_On(LED0);
    LOG(DEBUG_LEVEL, "[Nectar] Successfully initialized all drivers and connected datapoints.");
    LOG(DEBUG_LEVEL, "[Nectar] Starting data collection and transmission procedure.");
    delay_s(2);
}

inline void nectar_transmit(nectar_t* nectar)
{
    // TODO: replace this with payload in front of queue
    if (!nectar->payload.full)
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

    // once we encode a payload, set transmit request for xbee, and call xbee_transmit, that payload delivery is taken care of
    // and we can remove it from our payload queue.
    if (nectar->xbee.tx_state == SERIAL_IDLE)
    {
        nectar_encode_payload(&nectar->payload, &nectar->payload_buffer);
        xbee_set_transmit_request(nectar->payload_buffer.data, nectar->payload_buffer.size, &nectar->xbee.api_frame);
        // pop payload from front of queue

        // for now just reset the front payload
        nectar_payload_init(&nectar->payload);
    }

    // transmit fresh payload if SERIAL_IDLE, otherwise retransmit stale payload
    xbee_transmit(&nectar->xbee);
}

inline void nectar_receive(nectar_t* nectar)
{
    xbee_receive(&nectar->xbee);
    // todo: asynchronously collect data from sensors
    //coz_ir_receive(&nectar->coz_ir);
    //telaire_receive(&nectar->telaire);
}

void nectar_compile(nectar_t* nectar)
{
    // push to payload in the front of queue
    if (nectar->payload.size + nectar->payload.subpayload_size >= MAX_PAYLOAD_SIZE)
    {
        nectar->payload.full = true;
        // TODO: 
        // create new payload, push back to the queue.
        // we always edit the payload in the back of the queue
        // then push another payload to the back.
        // return for now, since we are just reusing a single payload for now
        return;
    }

    // determine which subpayload we need to edit
    nectar_payload_t* payload = &nectar->payload;
    uint32_t index = payload->subpayload_count;
    nectar_subpayload_t* subpayload = &payload->subpayloads[index];

    // set all values in subpayload
    uint16_t datapoints = payload->datapoints;

    /* other values that might be set based on sensor configuration */
    if (Tst_bits(datapoints, NECTAR_TIMESTAMP)) subpayload->timestamp = time(NULL);
    if (Tst_bits(datapoints, NECTAR_CO2_PPM)) subpayload->co2_ppm = (rand() % 120) + 290;
    if (Tst_bits(datapoints, NECTAR_U_VECTOR)) subpayload->u_vector = (rand() % 20) - 10;
    if (Tst_bits(datapoints, NECTAR_V_VECTOR)) subpayload->v_vector = (rand() % 20) - 10;
    if (Tst_bits(datapoints, NECTAR_W_VECTOR)) subpayload->w_vector = (rand() % 20) - 10;
    if (Tst_bits(datapoints, NECTAR_TEMPERATURE)) subpayload->temperature = (rand() % 40);
    if (Tst_bits(datapoints, NECTAR_HUMIDITY)) subpayload->humidity = rand() % 100;
    if (Tst_bits(datapoints, NECTAR_PRESSURE)) subpayload->pressure = (rand() % 100) + 970;

    payload->size += payload->subpayload_size;
    payload->subpayload_count++;
}

static void nectar_payload_init(nectar_payload_t* payload)
{
    payload->full = false;
    payload->size = 2; // include the delimiter and subpayload count
    payload->delimiter = 0xAF; // constant
    payload->datapoints = 0xC000; // TODO: poll CONNECTED devices to compute this
    payload->subpayload_count = 0; // number of collected subpayloads
    payload->subpayload_size = nectar_calculate_subpayload_size(payload->datapoints);
}

static size_t nectar_calculate_subpayload_size(uint16_t datapoints)
{
    size_t size = 2; // start with bitmask
    if (Tst_bits(datapoints, NECTAR_TIMESTAMP)) size += sizeof(uint64_t);
    if (Tst_bits(datapoints, NECTAR_CO2_PPM)) size += sizeof(uint16_t);
    if (Tst_bits(datapoints, NECTAR_U_VECTOR)) size += sizeof(float);
    if (Tst_bits(datapoints, NECTAR_V_VECTOR)) size += sizeof(float);
    if (Tst_bits(datapoints, NECTAR_W_VECTOR)) size += sizeof(float);
    if (Tst_bits(datapoints, NECTAR_TEMPERATURE)) size += sizeof(float);
    if (Tst_bits(datapoints, NECTAR_HUMIDITY)) size += sizeof(float);
    if (Tst_bits(datapoints, NECTAR_PRESSURE)) size += sizeof(float);
    return size;
}

void nectar_encode_payload(nectar_payload_t* payload, vector_t* buffer)
{
    LOG(ENCODER_LEVEL, "[NECTAR] Encoding application payload.");
    vector_clear(buffer);
    vector_push(&payload->delimiter, sizeof(uint8_t), buffer);
    vector_push(&payload->subpayload_count, sizeof(uint8_t), buffer);

    LOG(ENCODER_LEVEL, "[NECTAR] %-16s:= 0x%02X", "delimiter", payload->delimiter);
    LOG(ENCODER_LEVEL, "[NECTAR] %-16s:= %u", "payload count", payload->subpayload_count);
    LOGBITS(ENCODER_LEVEL, "[NECTAR] device bitmask  := ", &payload->datapoints, sizeof(uint16_t));
    LOG(ENCODER_LEVEL, "[NECTAR] \tEncoding subpayloads.");

    for (uint32_t i = 0; i < payload->subpayload_count; ++i)
    {
        nectar_encode_subpayload(&payload->subpayloads[i], payload->datapoints, buffer);
    }
}

void nectar_encode_subpayload(nectar_subpayload_t* subpayload, uint16_t datapoints, vector_t* buffer)
{
    vector_push(&datapoints, sizeof(uint16_t), buffer);

    if (Tst_bits(datapoints, NECTAR_TIMESTAMP)) {
        vector_push(&subpayload->timestamp, sizeof(uint64_t), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %llu", "timestamp", subpayload->timestamp);
    }
    if (Tst_bits(datapoints, NECTAR_CO2_PPM)) {
        vector_push(&subpayload->co2_ppm, sizeof(uint16_t), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %u", "co2 ppm", subpayload->co2_ppm);
    }
    if (Tst_bits(datapoints, NECTAR_U_VECTOR)) {
        vector_push(&subpayload->u_vector, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "u-vector", subpayload->u_vector);
    }
    if (Tst_bits(datapoints, NECTAR_V_VECTOR)) {
        vector_push(&subpayload->v_vector, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "v-vector", subpayload->v_vector);
    }
    if (Tst_bits(datapoints, NECTAR_W_VECTOR)) {
        vector_push(&subpayload->w_vector, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "w-vector", subpayload->w_vector);
    }
    if (Tst_bits(datapoints, NECTAR_TEMPERATURE)) {
        vector_push(&subpayload->temperature, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "temperature", subpayload->temperature);
    }
    if (Tst_bits(datapoints, NECTAR_HUMIDITY)) {
        vector_push(&subpayload->humidity, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "humidity", subpayload->humidity);
    }
    if (Tst_bits(datapoints, NECTAR_PRESSURE)) {
        vector_push(&subpayload->pressure, sizeof(float), buffer);
        LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %f", "pressure", subpayload->pressure);
    }
}

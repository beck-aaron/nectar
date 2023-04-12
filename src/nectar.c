/*******************************************************************************
 * File:    nectar.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "nectar.h"

inline void nectar_init(nectar_t* nectar)
{
    srand((unsigned) time(NULL));

    // TODO: initialize first payload in queue
    nectar->payload.full = false;
    nectar->payload.size = 2; // include the delimiter and subpayload count
    nectar->payload.delimiter = 0xAF; // constant
    nectar->payload.datapoints = 0x8000; // poll CONNECTED datapoints for this
    nectar->payload.subpayload_count = 0; // number of collected subpayloads
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
                LOG(DEBUG_LEVEL, "[Nectar] Payload not filled, skipping transmission until more data has been collected.");
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

        // for now just reset the payload
        nectar->payload.full = false;
        nectar->payload.size = 2;
        nectar->payload.delimiter = 0xAF; // constant
        nectar->payload.datapoints = 0x8000; // poll CONNECTED datapoints for this
        nectar->payload.subpayload_count = 0;        // subpayload_count based on CONNECTED datapoints
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
    if (nectar->payload.size == MAX_PAYLOAD_SIZE)
    {
        nectar->payload.full = true;
        // TODO: 
        // create new payload, push back to the queue.
        // we always edit the payload in the back of the queue
        // then push another payload to the back.
        // return for now
        return;
    }

    // determine which subpayload we need to edit
    nectar_payload_t* payload = &nectar->payload;
    uint32_t index = payload->subpayload_count;
    nectar_subpayload_t* subpayload = &payload->subpayloads[index];


    // set all values in subpayload
    // we always set the timestamp
    subpayload->timestamp = time(NULL);
    payload->size += sizeof(subpayload->timestamp);

    // record data from sensors
    /*
    switch(payload->datapoints)
    {
        case NECTAR_CO2_PPM: subpayload->co2_ppm = coz_ir_get_co2(); break;
        case NECTAR_U_VECTOR: break;
        case NECTAR_V_VECTOR: break;
        case NECTAR_W_VECTOR: break;
        case NECTAR_TEMPERATURE: break;
        case NECTAR_HUMIDITY: break;
        case NECTAR_PRESSURE: break;
    }
    */

    payload->subpayload_count++;
}

void nectar_encode_payload(nectar_payload_t* payload, vector_t* buffer)
{
    LOG(ENCODER_LEVEL, "[NECTAR] Encoding application payload.");
    vector_clear(buffer);
    vector_push(&payload->delimiter, sizeof(uint8_t), buffer);
    vector_push(&payload->subpayload_count, sizeof(uint8_t), buffer);
    vector_push(&payload->datapoints, sizeof(uint16_t), buffer);

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
    switch(datapoints)
    {
        case NECTAR_TIMESTAMP:
            vector_push(&subpayload->timestamp, sizeof(uint64_t), buffer);
            LOG(ENCODER_LEVEL, "[NECTAR] \t%-24s:= %s", "timestamp", ctime(&subpayload->timestamp));
            break;
        case NECTAR_CO2_PPM:
            vector_push(&subpayload->co2_ppm, sizeof(uint16_t), buffer);
            break;
        case NECTAR_U_VECTOR:
            vector_push(&subpayload->u_vector, sizeof(int32_t), buffer);
            break;
        case NECTAR_V_VECTOR:
            vector_push(&subpayload->v_vector, sizeof(int32_t), buffer);
            break;
        case NECTAR_W_VECTOR:
            vector_push(&subpayload->w_vector, sizeof(int32_t), buffer);
            break;
        case NECTAR_TEMPERATURE:
            vector_push(&subpayload->temperature, sizeof(int32_t), buffer);
            break;
        case NECTAR_HUMIDITY:
            vector_push(&subpayload->humidity, sizeof(int32_t), buffer);
            break;
        case NECTAR_PRESSURE:
            vector_push(&subpayload->pressure, sizeof(int32_t), buffer);
            break;
    }
}

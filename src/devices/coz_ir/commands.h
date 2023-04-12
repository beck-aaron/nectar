#ifndef _COZ_IR_COMMANDS_H_
#define _COZ_IR_COMMANDS_H_

#include <stdint.h>
#include <vector.h>

#define COZ_IR_COMMAND_MODE 0
#define COZ_IR_STREAMING_MODE 1
#define COZ_IR_POLLING_MODE 2
#define COZ_IR_RESPONSE_LENGTH 36

typedef enum
{
    COZ_IR_SET_MODE                 = 'K',
    COZ_IR_GET_FILTERED_CO2_PPM     = 'Z',
    COZ_IR_GET_UNFILTERED_CO2_PPM   = 'z',
    COZ_IR_GET_TEMPERATURE          = 'T',
    COZ_IR_GET_HUMIDITY             = 'H',

} coz_ir_command_t;

inline static void coz_ir_encode_message(coz_ir_command_t msg, vector_t* buffer)
{
    const uint16_t endl = 0x0D0A;
    vector_push(&msg, sizeof(char), buffer);
    vector_push(&endl, sizeof(endl), buffer);
}

#endif // _COZ_IR_COMMANDS_H_

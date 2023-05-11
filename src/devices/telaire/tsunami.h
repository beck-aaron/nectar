// TSUNAMI-LITE Telaire communications protocol definitions
//

#ifndef _TSUNAMI_LITE_H
#define _TSUNAMI_LITE_H

#include <stdint.h>
#include <vector.h>

typedef enum
{
    ERROR       = 0x01,
    WARMUP_MODE = 0x02,
    CALIBRATION = 0x04,
    IDLE_MODE   = 0x08,
    SELF_TEST   = 0x80,

} telaire_status_t;

typedef enum
{
    GAS_PPM        = 0x03,
    SERIAL_NUMBER  = 0x01,
    COMPILE_SUBVOL = 0x0D,
    COMPILE_DATE   = 0x0C,
    ELEVATION      = 0x0F,

} telaire_read_t;


typedef enum
{
    // READ commands
    CMD_READ                = 0x02,
    // UPDATE commands
    CMD_UPDATE              = 0x03,

    // WARMUP command
    CMD_WARMUP              = 0x84,

    // STATUS & OPERATING commands
    CMD_STATUS              = 0xB6,

    // TEST commands
    CMD_HALT                = 0x95,
    CMD_LOOPBACK            = 0x00,
    CMD_SELF_TEST_START     = 0xC000,
    CMD_SELF_TEST_RESULTS   = 0xC001,

    // STREAM command
    CMD_STREAM_DATA         = 0xBD,

} tsunami_cmd_t;

typedef struct
{
    uint8_t         data_id;
    uint16_t        co2_ppm;
    unsigned char   serial_number[15];
    unsigned char   compile_subvol[3];
    unsigned char   compile_date[6];
    uint16_t        elevation;

} cmd_read_t;

typedef struct
{
    uint16_t elevation;

} cmd_update_t;

typedef struct
{
    uint8_t status;

} cmd_status_t;

typedef struct
{
    uint8_t loopback[16];
    struct
    {
        uint8_t flag;
        uint8_t pga_status;
        uint8_t good_dsp;
        uint8_t total_dsp;

    } results;

} cmd_test_t;

void telaire_encode_read_cmd(const cmd_read_t* cmd, vector_t* vector);
void telaire_encode_update_cmd(const cmd_update_t* cmd, vector_t* vector);
void telaire_encode_test_cmd(const cmd_test_t* cmd, vector_t* vector);

#endif /* _TSUNAMI_LITE_H */

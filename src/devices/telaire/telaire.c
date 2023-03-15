#include "telaire.h"
#include "telaire/tsunami.h"

static void encode_read_cmd(void);
static void encode_update_cmd(void); //TODO
static void encode_loopback_cmd(void); //TODO

telaire_t telaire =
{
    .flag       = TELAIRE_HEADER_FLAG,
    .address    = TELAIRE_HEADER_ADDR,
    .encode     = telaire_encode,
    .decode     = telaire_decode,
    .transmit   = telaire_transmit,
    .configure  = telaire_configure,
    .push       = telaire_push,
};

void telaire_encode(void)
{
    telaire.push(&telaire.flag, sizeof(uint8_t));
    telaire.push(&telaire.address, sizeof(uint8_t));

    switch(telaire.command)
    {
        case CMD_READ:
            encode_read_cmd();
            break;

        case CMD_WARMUP:
        case CMD_STATUS:
        case CMD_HALT:
        case CMD_STREAM_DATA:
            telaire.push(&telaire.command, sizeof(uint8_t));
            break;

        case CMD_UPDATE:
            encode_update_cmd();
            break;

        case CMD_LOOPBACK:
            encode_loopback_cmd();
            break;

        case CMD_SELF_TEST_START:
            break;

        case CMD_SELF_TEST_RESULTS:
            break;
    }
}

void telaire_decode(void)
{
}

void telaire_transmit(void)
{
}

void telaire_configure(void)
{
}

void telaire_push(void* value, size_t size)
{
    vector_push(value, size, &telaire.tx_buffer);
}

static void encode_read_cmd(void)
{
    telaire.push(&telaire.command, sizeof(uint8_t));
    telaire.push(&telaire.cmd_read.data_id, sizeof(uint8_t));
}

static void encode_update_cmd(void)
{
    //TODO
}

static void encode_loopback_cmd(void)
{
    //TODO
}

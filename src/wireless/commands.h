/*
 *
 * Setup for AT Commands to configure Xbee device according to our network
 * parameters.
 *
 * To enter command mode, send "+++" within one second to the xbee.  The device
 * will return OK\r to confirm that it has entered command mode successfully.
 *
 * Every AT command starts with AT followed by the two characters that identify
 * the command and then by some optional configuration values.
 *
 * When the device is in command mode, it will wait for the CT time 
 * (default 10 seconds) without any user input before leaving Command mode.
 *
 * CN exits command mode.
 *
 * Send AT Commands:
 *
 * AT +  Command + Space(optional) + Parameter(optional, HEX) + Carriage Return
 *
 * Example: ATNI 2<CR>
 * This changes the NI (Node Identifier) to My XBee
 *
 *
 * When using AT commands to set parameters, the xbee responds with:
 *      OK<CR> when successful
 *      ERROR<CR> when a failure occurs
 *
 * Any changes made to the configuration command registers using AT commands
 * do not take effect unti the changes are applied.
 * To apply changes:
 *  1. Send AC (Apply Changes)
 *  2. Send WR (Write)          - writes to non-volatile memory
 *
 * RE (restore defaults) wipes settings saved by WR to factory defaults.
 *
 */

#ifndef _AT_COMMANDS_H_
#define _AT_COMMANDS_H_

#include <asf.h>

//const char* AT_PREFIX                       = "AT";
//const char* AT_SUFFIX                       = "\r";

/* Network commands */
/*
const char* EXTENDED_PAN_ID                 = "ID";
const char* SCAN_CHANNELS                   = "SC";
const char* SCAN_DURATION                   = "SD";
const char* ZIGBEE_STACK_PROFILE            = "ZS";
const char* NODE_JOIN_TIME                  = "NJ";
const char* NETWORK_WATCHDOG_TIMEOUT        = "NW";
const char* COORDINATOR_JOIN_VERIFICATION   = "JV";
const char* JOIN_NOTIFICATION               = "JN";
const char* OPERATING_EXTENDED_PAN_ID       = "OP";
const char* OPERATING_16_BIT_PAN_PID        = "OI";
const char* OPERATING_CHANNEL               = "CH";
const char* NUMBER_OF_REMAINING_CHILDREN    = "NC";
const char* COORDINATOR_ENABLE              = "CE";
const char* MISC_DEVICE_OPTIONS             = "DO";
const char* JOINING_DEVICE_CONTROLS         = "DC";
const char* INITIAL_16_BIT_PAN_ID           = "II";
const char* ENERGY_DETECT                   = "ED";
*/

/* Addressing commands */
/*
const char* SERIAL_NUMBER_HIGH              = "SH";
const char* SERIAL_NUMBER_LOW               = "SL";
const char* NETWORK_ADDRESS                 = "MY";
const char* PARENT_NETWORK_ADDRESS          = "MP";
const char* DESTINATION_ADDRESS_HIGH        = "DH";
const char* DESTINATION_ADDRESS_LOW         = "DL";
const char* NODE_IDENTIFIER                 = "NI";
const char* MAXIMUM_UNICAST_HOPS            = "NH";
const char* BROADCAST_HOPS                  = "BH";
const char* AGGREGATE_ROUTING_NOTIFICATION  = "AR";
const char* DEVICE_TYPE_IDENTIFIER          = "DD";
const char* NODE_DISCOVER_TIMEOUT           = "NT";
const char* NETWORK_DISCOVERY_OPTIONS       = "NO";
const char* MAXIMUM_PACKET_PAYLOAD_BYTES    = "NP";
const char* CONFLICT_REPORT                 = "CR";
*/

/* Zigbee Addressing commands */
/*
const char* SOURCE_ENDPOINT                 = "SE";
const char* DESTINATION_ENDPOINT            = "DE";
const char* CLUSTER_ID                      = "CI";
const char* TRANSMIT_OPTIONS                = "TO";
*/

/* RF interfacing commands */
/*
const char* TX_POWER_LEVEL                  = "PL";
const char* POWER_AT_PL4                    = "PP";
const char* POWER_MODE                      = "PM";
*/

/* Security commands */
/*
const char* ENCRYPTION_ENABLE               = "EE";
const char* ENCRYPTION_OPTIONS              = "EO";
const char* LINK_KEY                        = "KY";
const char* TRUST_CENTER_NETWORK_KEY        = "NK";
*/

/* Serial interfacing commands */
/*
const char* INTERFACE_DATA_RATE             = "BD";
const char* PARITY                          = "NB";
const char* STOP_BITS                       = "SB";
const char* PACKETIZATION_TIMEOUT           = "RO";
const char* DIO6_RTS                        = "D6";
const char* DIO7_CTS                        = "D7";
const char* API_ENABLE                      = "AP";
const char* API_OPTIONS                     = "AO";
*/

/* Command mode options */
/*
const char* COMMAND_MODE_TIMEOUT            = "CT";
const char* GUARD_TIMES                     = "GT";
const char* COMMAND_CHARACTER               = "CC";
const char* EXIT_COMMAND_MODE               = "CN";
*/

/* Sleep commands */
/*
const char* SLEEP_PERIOD                    = "SP";
const char* NUM_CYCLES_BETWEEN_ON_SLEEP     = "SN";
const char* SLEEP_MODE                      = "SM";
const char* TIME_BEFORE_SLEEP               = "ST";
const char* SLEEP_OPTIONS                   = "SO";
const char* WAKE_HOST_DELAY                 = "WH";
const char* POLLING_RATE                    = "PO";
*/

/* I/O settings commands */

/* I/O sampling commands */
/*
const char* IO_SAMPLE_RATE                  = "IR";
const char* DIGITAL_CHANGE_DIRECTION        = "IC";
const char* SET_VOLTAGE_SUPPLY_THRESHOLD    = "V+";
*/

/* Diagnostic commands */
/*
const char* FIRMWARE_VERSION                = "VR";
const char* HARDWARE_VERSION                = "HV";
const char* ASSOCIATION_INDICATION          = "AI";
const char* VOLTAGE_SUPPLY_MONITORING       = "%V";
const char* RECEIVED_SIGNAL_STRENGTH        = "DB";
const char* TEMPERATURE                     = "TP";
const char* VERSION_LONG                    = "VL";
*/

/* Execution commands */
/*
const char* APPLY_CHANGES                   = "AC";
const char* ACTIVE_SCAN                     = "AS";
const char* WRITE                           = "WR";
const char* RESTORE_DEFAULTS                = "RE";
const char* SOFTWARE_RESET                  = "FR";
const char* NETWORK_RESET                   = "NR";
const char* SLEEP_IMMEDIATELY               = "SI";
const char* COMMISSIONING_PUSHBUTTON        = "CB";
const char* CLEAR_BINDING_AND_GROUP_TABLES  = "&X";
const char* NODE_DISCOVERY                  = "ND";
const char* DESTINATION_NODE                = "DN";
const char* DISABLE_JOINING                 = "DJ";
const char* FORCE_SAMPLE                    = "IS";
*/

typedef enum
{
    /* network commands */
    ID,     // extended pan ID
    SC,     // scan channels
    SD,     // scan duration,
    ZS,     // zigbee stack profile,
    NJ,     // node join time,
    NW,     // network watchdog timeout,
    JV,     // coordinator join verification,
    JN,     // join notification,
    OP,     // operating extended pan id,
    OI,     // operating 16-bit pan id,
    CH,     // operating channel,
    NC,     // number of remaining children,
    CE,     // coordinator enable,
    DO,     // miscellaneous device options,
    DC,     // joining device controls,
    II,     // initial 16-bit pan id,
    ED,     // energy detect,

    /* addressing commands */
    SH,     // serial number high
    SL,     // serial number low
    MY,     // 16-bit network address
    MP,     // 16-bit parent network address
    DH,     // destination address high
    DL,     // destination address low
    NI,     // node identifier
    NH,     // maximum unicast hops
    BH,     // broadcast hops
    AR,     // aggregate routing notification
    DD,     // device type identifier
    NT,     // node discover timeout
    NO,     // network discovery options
    NP,     // maximum packet payload bytes
    CR,     // conflict report

} xbee_cmd_t;

typedef struct
{
    char name[2];
    uint16_t parameter;

} xbee_at_cmd;


#endif /* _AT_COMMANDS_H_ */

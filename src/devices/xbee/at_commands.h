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

typedef struct
{
    uint16_t code;
    uint16_t parameter;

} xbee_at_cmd_t;

typedef enum
{
    /* networking commands */
    ID = 0x4944,                // set or read extended pan id
    SC = 0x5343,                // set or read list of channels to scan
    SD = 0x5344,                // set or display scan duration exponent
    ZS = 0x5A53,
    NJ = 0x4E4A,
    NW = 0x4E57,
    JV = 0x4A56,
    JN = 0x4A4E,
    OP = 0x4F50,
    OI = 0x4F49,
    CH = 0x4348,
    NC = 0x4E43,
    CE = 0x4345,
    DO = 0x444F,
    DC = 0x4443,
    II = 0x4949,
    ED = 0x4544,

    /* addressing commands */
    SH = 0,
    SL = 0,
    MY = 0,
    MP = 0,
    DH = 0,
    DL = 0,
    NI = 0,
    NH = 0,
    BH = 0,
    AR = 0,
    DD = 0,
    NT = 0,
    NO = 0,
    NP = 0x4E50,
    CR = 0,

    /* zigbee addressing commands */
    SE = 0,
    DE = 0,
    CI = 0,
    TO = 0,

    /* RF interfacing commands */
    PL = 0,
    PP = 0,
    PM = 0,

    /* Security commands */
    EE = 0,
    EO = 0,
    KY = 0,
    NK = 0,

    /* Serial interfacing commands */
    CT = 0,
    GT = 0,
    CC = 0,
    CN = 0,

    /* Sleep commands */
    SP = 0,
    SN = 0,
    SM = 0,
    ST = 0,
    SO = 0,
    WH = 0,
    PO = 0,

    /* IO settings commands */
    D0 = 0,
    D1 = 0,
    D2 = 0,
    D3 = 0,
    D4 = 0,
    D5 = 0,
    D6 = 0,
    D7 = 0,
    D8 = 0,
    D9 = 0,
    P0 = 0,
    P1 = 0,
    P2 = 0,
    P3 = 0,
    P4 = 0,
    P5 = 0,
    P6 = 0,
    P7 = 0,
    P8 = 0,
    P9 = 0,
    PR = 0,
    PD = 0,
    LT = 0,
    RP = 0,

    /* IO sampling commands */
    IR = 0,
    IC = 0,
    VP = 0,

    /* Diagnostic commands */
    VR = 0x5652,
    HV = 0,
    AI = 0,
    PV = 0,
    DB = 0,
    TP = 0,
    VL = 0x564C,

    /* Execution commands */
    AC = 0x4143,
    AS = 0,
    WR = 0,
    RE = 0,
    FR = 0,
    NR = 0,
    SI = 0,
    CB = 0,
    AX = 0,
    ND = 0,
    DN = 0,
    DJ = 0,
    IS = 0,

} xbee_cmd_t;


#endif /* _AT_COMMANDS_H_ */


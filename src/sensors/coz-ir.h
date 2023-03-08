/*******************************************************************************
 * File:    coz-ir.h
 * Author:  Aaron Beck
 *
 * Currently using UART0 port for coz-ira sensor on evaluation board.
 * SAME70 Xplained.  Code should be easily ported to PTLS board.
 *
 ******************************************************************************/
#include <asf.h>
#include <logger.h>

#ifndef _COZ_IR_H_
#define _COZ_IR_H_

typedef struct
{

} coz_ir_t;

/**
 * @brief
 */
void coz_ir_init(void);

/**
 * @brief
 */
void coz_ir_read(void);

#endif // _COZ_IR_H_


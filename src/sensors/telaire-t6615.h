/*******************************************************************************
 * File:    telaire-t6615.h
 * Author:  Aaron Beck
 *
 * Currently using UART0 port for trisonica-mini sensor on evaluation board.
 * SAME70 Xplained.  Code should be easily ported to PTLS board.
 *
 ******************************************************************************/
#include <asf.h>
#include <logger.h>

#ifndef _TELAIRE_H_
#define _TELAIRE_H_

typedef struct
{

} telaire_t6615_t;

/**
 * @brief
 */
void telaire_init(void);

/**
 * @brief
 */
void telaire_read(void);

#endif // _TELAIRE_H_

/*******************************************************************************
 * File:    trisonica-mini.h
 * Author:  Aaron Beck
 *
 * Currently using UART1 port for trisonica-mini sensor.
 * This corresponds to board label UTXD1, URXD1, GND
 *
 ******************************************************************************/
#ifndef _TRISONICA_MINI_H_
#define _TRISONICA_MINI_H_

#include <asf.h>
#include <logger.h>

/**
 * @brief 
 */
void tsm_init(void);

/**
 * @brief 
 */
void tsm_read(void);

#endif //_TRISONICA_MINI_H_ 

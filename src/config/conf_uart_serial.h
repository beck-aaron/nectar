/**
 * \file
 *
 * \brief USART Serial Configuration
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_USART_SERIAL_H_INCLUDED
#define CONF_USART_SERIAL_H_INCLUDED

/* Logger interface */
#define LOGGER_UART                 USART1                  // serial interface
#define LOGGER_UART_ID              ID_USART1               // peripheral ID
#define LOGGER_UART_BAUDRATE        (115200UL)              // baudrate
#define LOGGER_UART_CHAR_LENGTH     US_MR_CHRL_8_BIT        // character length
#define LOGGER_UART_PARITY          US_MR_PAR_NO            // Parity setting
#define LOGGER_UART_STOP_BITS       US_MR_NBSTOP_1_BIT      // Stop bits setting

/* Trisonica-mini anemometer interface */
#define TRISONICA_UART              USART0                  // serial interface
#define TRISONICA_UART_ID           ID_USART0               // peripheral ID
#define TRISONICA_UART_BAUDRATE     (115200UL)              // baudrate
#define TRISONICA_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT        // character length
#define TRISONICA_UART_PARITY       US_MR_PAR_NO            // Parity setting
#define TRISONICA_UART_STOP_BITS    US_MR_NBSTOP_1_BIT      // Stop bits setting

/* Telaire-T6615 CO2 sensor interface */
#define TELAIRE_UART                USART0                  // serial interface
#define TELAIRE_UART_ID             ID_USART0               // peripheral ID
#define TELAIRE_UART_BAUDRATE       (19200UL)               // baudrate
#define TELAIRE_UART_CHAR_LENGTH    US_MR_CHRL_8_BIT        // character length
#define TELAIRE_UART_PARITY         US_MR_PAR_NO            // Parity setting
#define TELAIRE_UART_STOP_BITS      US_MR_NBSTOP_1_BIT      // Stop bits setting

/* CozIR-A CO2 sensor interface */
#define COZ_IR_UART                 USART0                  // serial interface
#define COZ_IR_UART_ID              ID_USART0               // peripheral ID
#define COZ_IR_UART_BAUDRATE        (9600UL)                // baudrate
#define COZ_IR_UART_CHAR_LENGTH     US_MR_CHRL_8_BIT        // character length
#define COZ_IR_UART_PARITY          US_MR_PAR_NO            // Parity setting
#define COZ_IR_UART_STOP_BITS       US_MR_NBSTOP_1_BIT      // Stop bits setting

#endif/* CONF_USART_SERIAL_H_INCLUDED */

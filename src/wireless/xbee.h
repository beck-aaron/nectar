/*
 *
 * xbee driver for SAME70
 *
 * note: ignoring xdmac for rapid prototyping, plan to integrate this later
 *
 */

#ifndef _XBEE_H_
#define _XBEE_H_

#include <asf.h>
#include <logger.h>

#include "commands.h"

/**
 * @brief Sets up UART interface and configures xbee with needed parameters to
 * join the network.
 */
void xbee_init(void);

/**
 * @brief Enter xbee command mode and set all necessary AT commands
 * After all commands have been written, apply changes and write to non-volatile memory
 */
void xbee_configure(void);

/**
 * @brief Sends an API frame or AT commands to the xbee. The API frame can be
 * generated from the encoder functions within the codec directory.
 */
void xbee_write(const void* data, const size_t size);

void xbee_set_cmd(const char* cmd, uint16_t parameter);
void xbee_read_cmd(const char* cmd);

/**
 * @brief Read response from xbee after writing an API frame
 * Log output for now.
 *
 * @param size the amount of bytes to read
 */
void xbee_return_code(void);

/**
 * @brief 
 *
 * @param size
 */
void xbee_read(void);

#endif /* _XBEE_H_ */

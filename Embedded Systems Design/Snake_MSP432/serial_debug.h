/*
 * serial_debug.h
 *
 *  Created on: Aug 25, 2020
 *      Author: Joe Krachey
 */
#ifndef SERIAL_DEBUG_H_
#define SERIAL_DEBUG_H_
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "msp.h"

#define RX_BUFFER_MAX_SIZE 80
#define CLEAR_TERMINAL "\x1b[2J"

extern volatile char buffer[RX_BUFFER_MAX_SIZE];
extern volatile int buffer_size;
extern volatile bool rx_done;
extern volatile bool accepting_user_input;

//****************************************************************************
// Initializes the circular buffers used to transmit and receive data from the
// serial debug interface. It will also initialize the UART interface to enable
// interrupts.
// ****************************************************************************/
void serial_debug_init(void);
//****************************************************************************
// Prints a string to the serial debug UART
// ****************************************************************************/
void serial_debug_put_string(char * s);

// Sets whether UART stores data read or not
void allow_user_input(bool permitted);

// Resets UART receive buffer
void clear_rx_buffer(void);
#endif /* SERIAL_DEBUG_H_ */

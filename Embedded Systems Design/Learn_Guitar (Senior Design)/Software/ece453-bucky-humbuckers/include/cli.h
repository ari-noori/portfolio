/*
 * cli.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

#ifndef CLI_H_
#define CLI_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define MAX_UART_PRINTF_LEN 512
#define ARGUMENT_MAXIMUM 	10
#define CLI_BUFFER_LENGTH 	0xFF
#define CLI_BAUD_RATE 		1000000
//#define CLI_BAUD_RATE 		256000
//#define CLI_BAUD_RATE 		115200
#define DATA_BITS_8     	8
#define STOP_BITS_1     	1
//#define CLI_UART_RX			P9_0
//#define CLI_UART_TX			P9_1
#define CLI_UART_RX P10_0
#define CLI_UART_TX P10_1
#define CLI_INT_PRIORITY	5

/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/
typedef void (*cliCallback)(uint8_t nargs, uint8_t** args);

typedef struct cliCommand {
    uint8_t name[10];
    uint8_t description[80];
    cliCallback callback;
} cliCommand;

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/

/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/
void vCommandLineTask( void * pvParameters );

void commandLineInit(void);

void cliPrint(const uint8_t *_format, ...);

void cliPrintln(const uint8_t *_format, ...);

void commandLineInit(void);

#endif /* CLI_H_ */

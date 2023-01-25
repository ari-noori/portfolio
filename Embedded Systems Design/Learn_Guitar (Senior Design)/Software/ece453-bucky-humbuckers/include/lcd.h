/*
 * lcd.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

#ifndef LCD_H_
#define LCD_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <time.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "main.h"
#include "images.h"
#include "task.h"
#include "touchscreen.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
// Coordinates for modes buttons
#define DELTA_X		20
#define DELTA_Y		35
#define MODE_X0		92
#define MODE_X1		145
#define MODE_Y0		85
#define MODE_Y1		240

#define LCD_RESET_PIN P5_3
#define LCD_MOSI_PIN P9_0
#define LCD_MISO_PIN P9_1
#define LCD_SCLK_PIN P9_2
#define LCD_NCS_PIN  P9_3
#define LCD_WR_PIN	 P12_6
#define LCD_BL_PIN	 P0_4
#define LCD_BITS_PER_FRAME 8
//#define LCD_SPI_FREQUENCY 6350000
#define LCD_SPI_FREQUENCY 25000000
#define LCD_BUFFER_SIZE 0xFF
#define LCD_POINT_BITMASK ((1 << 12) - 1)
#define LCD_BACK_BUTTON_SIZE 40

/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/
typedef enum screen{
	START,
	MODES,
	LEARN,
	ROCK,
	TUNE,
	ABOUT,
	COLORTEST
} screen_t;

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/
extern cyhal_spi_t spiDevice;
/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/
void vLcdTask( void * pvParameters );

void lcdSpiInit(void);

void lcdSetPosition(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);

void lcdSpiInit(void);

void touchscreenInit(void);

#endif /* LCD_H_ */

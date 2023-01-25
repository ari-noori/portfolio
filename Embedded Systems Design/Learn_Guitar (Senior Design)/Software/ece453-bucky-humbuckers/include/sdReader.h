/*
 * sdReader.h
 *
 *  Created on: Nov 9, 2022
 *      Author: tanow
 */

#ifndef INCLUDE_SDREADER_H_
#define INCLUDE_SDREADER_H_

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
#include "lcd.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define SD_NCS_PIN P9_4
/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/

/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/
void sdLoadImage(uint8_t* filename, uint8_t* data, uint32_t bytesToRead);

void sdInit(void);

#endif /* INCLUDE_SDREADER_H_ */

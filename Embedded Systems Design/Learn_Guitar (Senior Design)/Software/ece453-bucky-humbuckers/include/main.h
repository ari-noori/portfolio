/*
 * signalProcessing.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

#ifndef INCLUDE_MAIN_H_
#define INCLUDE_MAIN_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdlib.h>
#include "cy_syspm.h"
#include "cy_sysclk.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cli.h"
#include "gpioButton.h"
#include "lcd.h"
#include "microphoneAdc.h"
#include "piezo.h"
#include "signalProcessing.h"
#include "touchscreen.h"
#include "sdReader.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define BACKGROUND_IMAGE_LENGTH (240*320*2)
#define NOTE_IMAGE_LENGTH 		 16000
#define TUNING_IMAGE_LENGTH		  (70*70*2)
#define COLORTEST_IMAGE_LENGTH	(40*40*2)
/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/
extern TaskHandle_t cliTaskHandle;
extern TaskHandle_t buttonTaskHandle;
extern TaskHandle_t adcTaskHandle;
extern TaskHandle_t lcdTaskHandle;
extern TaskHandle_t piezoTaskHandle;
extern TaskHandle_t signalProcessingTaskHandle;

extern SemaphoreHandle_t cliSemaphore;
extern SemaphoreHandle_t adcSemaphore;
extern SemaphoreHandle_t spiSemaphore;
extern SemaphoreHandle_t signalProcessingSemaphore;

extern FATFS FatFs;		/* FatFs work area needed for each volume */
extern FIL Fil;			/* File object needed for each open file */

extern uint8_t sdBuffer[BACKGROUND_IMAGE_LENGTH];
/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/

#endif /* INCLUDE_MAIN_H_ */

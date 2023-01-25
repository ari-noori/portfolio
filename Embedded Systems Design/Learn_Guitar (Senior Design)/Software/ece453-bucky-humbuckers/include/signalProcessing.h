/*
 * signalProcessing.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

#ifndef SIGNALPROCESSING_H_
#define SIGNALPROCESSING_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>
#include "arm_math.h"
#include <math.h>
#include "microphoneAdc.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
//#define FFT_LENGTH 128
//#define FFT_LENGTH 256
//#define FFT_LENGTH 512
#define FFT_LENGTH 1024
#define FFT_BUCKET_WIDTH 7.629395
#define BUCKET_THRESHOLD 75000
/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/
extern bool signalProcessingPrint;
/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/
void vSignalProcessingTask( void * pvParameters );

bool getNoteStatus(uint16_t noteInHertz);

#endif /* SIGNALPROCESSING_H_ */

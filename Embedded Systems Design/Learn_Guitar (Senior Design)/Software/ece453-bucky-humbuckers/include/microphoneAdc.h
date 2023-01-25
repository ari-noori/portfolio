/*
 * microphoneAdc.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

#ifndef MICROPHONEADC_H_
#define MICROPHONEADC_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "FreeRTOS.h"
#include <stdbool.h>
#include "task.h"
#include "signalProcessing.h"
#include "cli.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define ADC_PIN P10_6
/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/
extern int32_t adcData[];
extern bool adcPrint;
/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/
void vAdcTask( void * pvParameters );

#endif /* MICROPHONEADC_H_ */

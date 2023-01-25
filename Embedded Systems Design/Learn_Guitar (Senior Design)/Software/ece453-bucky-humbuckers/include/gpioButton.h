/*
 * gpioButton.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

#ifndef GPIOBUTTON_H_
#define GPIOBUTTON_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define COURSE_DEV_KIT

#ifndef COURSE_DEV_KIT
#define BUTTON_PIN P0_4
#define LED_PIN P6_3
#else
#define LED_PIN P5_5
#define BUTTON_ONE_PIN P5_6
#define BUTTON_TWO_PIN P6_2
#endif
/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/

/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/
void vButtonTask( void * pvParameters);

void buttonInterruptHandler(void* handler_arg, cyhal_gpio_event_t event);

#endif /* GPIOBUTTON_H_ */

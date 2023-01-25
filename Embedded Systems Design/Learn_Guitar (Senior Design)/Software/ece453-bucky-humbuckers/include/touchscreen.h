/*
 * touchscreen.h
 *
 *  Created on: Nov 7, 2022
 *      Author: tanow
 */

#ifndef INCLUDE_TOUCHSCREEN_H_
#define INCLUDE_TOUCHSCREEN_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <time.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "images.h"
#include "task.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define TOUCHSCREEN_SCLK_PIN P5_0
#define TOUCHSCREEN_SDA_PIN	 P5_1
#define TOUCHSCREEN_INTERRUPT_PIN P5_2
#define TOUCHSCREEN_FREQUENCY 200000
#define TOUCHSCREEN_SLAVE_ADDRESS 0x38
#define TOUCHSCREEN_INTERRUPT_PRIORITY 5
#define TOUCHSCREEN_REFRESH_RATE 95

/*******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/
typedef enum touchscreenRegister {
       TOUCHSCREEN_DEVICE_MODE_REG = 0x00,
       TOUCHSCREEN_GESTURE_ID_REG = 0x01,
       TOUCHSCREEN_TD_STATUS_REG = 0x02,
       TOUCHSCREEN_P1_XH_REG = 0x03,
       TOUCHSCREEN_P1_XL_REG = 0x04,
       TOUCHSCREEN_P1_YH_REG = 0x05,
       TOUCHSCREEN_P1_YL_REG = 0x06,
       TOUCHSCREEN_P1_WEIGHT_REG = 0x07,
       TOUCHSCREEN_P1_MISC_REG = 0x08,
       TOUCHSCREEN_P2_XH_REG = 0x09,
       TOUCHSCREEN_P2_XL_REG = 0x0A,
       TOUCHSCREEN_P2_YH_REG = 0x0B,
       TOUCHSCREEN_P2_YL_REG = 0x0C,
       TOUCHSCREEN_P2_WEIGHT_REG = 0x0D,
       TOUCHSCREEN_P2_MISC_REG = 0x0E,
       TOUCHSCREEN_TH_GROUP_REG = 0x80,
       TOUCHSCREEN_TH_DIFF_REG = 0x85,
       TOUCHSCREEN_CTRL_REG = 0x86,
       TOUCHSCREEN_TIMEENTERMONITOR_REG = 0x87,
       TOUCHSCREEN_PERIODACTIVE_REG = 0x88,
       TOUCHSCREEN_PERIODMONITOR_REG = 0x89,
       TOUCHSCREEN_RADIAN_VALUE_REG = 0x91,
       TOUCHSCREEN_OFFSET_LEFT_RIGHT_REG = 0x92,
       TOUCHSCREEN_OFFSET_UP_DOWN_REG = 0x93,
       TOUCHSCREEN_DISTANCE_LEFT_RIGHT_REG = 0x94,
       TOUCHSCREEN_DISTANCE_UP_DOWN_REG = 0x95,
       TOUCHSCREEN_DISTANCE_ZOOM_REG = 0x96,
       TOUCHSCREEN_LIB_VER_H_REG = 0xA1,
       TOUCHSCREEN_LIB_VER_L_REG = 0xA2,
       TOUCHSCREEN_CIPHER_REG = 0xA3,
       TOUCHSCREEN_G_MODE_REG = 0xA4,
       TOUCHSCREEN_PWR_MODE_REG = 0xA5,
       TOUCHSCREEN_FIRMID_REG = 0xA6,
       TOUCHSCREEN_FOCALTECH_ID_REG = 0xA8,
       TOUCHSCREEN_RELEASE_CODE_ID_REG = 0xAF,
       TOUCHSCREEN_STATE_REG = 0xBC
} touchscreenRegister;

typedef enum touchscreenEvent{
	PRESS_DOWN = 0,
	LIFT_UP = 1,
	CONTACT = 2,
	NO_EVENT = 3,
} touchscreenEvent;

/*******************************************************************************
 * Global Variable Declarations
 ******************************************************************************/

extern const uint16 TOUCHSCREEN_NO_NEW_INPUT[2];

/*******************************************************************************
 * Global Function Declarations
 ******************************************************************************/
void touchscreenInit(void);

void touchScreenInterruptHandler(void *handler_arg, cyhal_gpio_event_t event);

uint16* touchscreenGetTouchPoint();

#endif /* INCLUDE_TOUCHSCREEN_H_ */

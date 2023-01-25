/*
 * task_joystick.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#ifndef TASK_JOYSTICK_H_
#define TASK_JOYSTICK_H_

#include "main.h"

#define VOLT_MIN  (jerk ? 1800 : 1300)      // 0.85 /(3.3/4096)
#define VOLT_MAX  (jerk ? 2200 : 2900)      // 2.50 /(3.3/4096)
#define ACCEL_BASE 2900
#define ACCEL_MIN  ACCEL_BASE - 500
#define ACCEL_MAX  ACCEL_BASE + 500

extern TaskHandle_t Task_Joystick_Handle;
extern TaskHandle_t Task_Joystick_Timer_Handle;
extern volatile int timer_ms;
extern volatile bool game_in_progress;
extern volatile bool jerk;
extern volatile uint32_t JOYSTICK_X_DIR ;
extern volatile uint32_t JOYSTICK_Y_DIR;
extern volatile uint32_t ACCEL_X_DIR;
extern volatile uint32_t ACCEL_Y_DIR;
extern volatile uint32_t ACCEL_Z_DIR;

typedef enum {
    JOYSTICK_DIR_CENTER,
    JOYSTICK_DIR_LEFT,
    JOYSTICK_DIR_RIGHT,
    JOYSTICK_DIR_UP,
    JOYSTICK_DIR_DOWN,
} JOYSTICK_DIR_t;



/******************************************************************************
* Examines the ADC data from the joystick on the MKII
******************************************************************************/
void Task_Joystick_Timer(void *pvParameters);
void Task_Joystick_Bottom_Half(void *pvParameters);
void init_game(void);

#endif /* TASK_JOYSTICK_H_ */

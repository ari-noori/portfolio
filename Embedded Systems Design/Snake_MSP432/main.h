/*
 * main.h
 *
 *  Created on: Oct 19, 2020
 *      Author: Joe Krachey
 *      Edited by: Aaron Cohen; Arian Noori - May 05 2022
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* ECE353 Includes */
#include "task_joystick.h"
#include "task_paint.h"
#include "task_erase.h"
#include "task_optical.h"
#include "task_darkmode.h"
#include "peripherals.h"
#include "lcd.h"
#include "music.h"
#include "opt3001.h"
#include "i2c.h"
#include "snake.h"



#endif /* MAIN_H_ */

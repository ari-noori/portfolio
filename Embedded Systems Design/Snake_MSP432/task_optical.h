/*
 * task_optical.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#ifndef TASK_OPTICAL_H_
#define TASK_OPTICAL_H_

extern unsigned color_fg;
extern unsigned color_bg;

TaskHandle_t Task_Optical_Handle;
void Task_Optical(void *pvParameters);

#endif /* TASK_OPTICAL_H_ */

/*
 * task_erase.h
 *
 *  Created on: May 5, 2022
 *      Author: AC
 */

#ifndef TASK_ERASE_H_
#define TASK_ERASE_H_

extern QueueHandle_t Queue_Erase;
extern TaskHandle_t Task_Erase_Handle;
void Task_Erase_Square(void *pvParameters);

#endif /* TASK_ERASE_H_ */

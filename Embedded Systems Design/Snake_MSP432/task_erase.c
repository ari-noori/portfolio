/*
 * task_erase.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */
#include "main.h"
#include "grid.h"

QueueHandle_t Queue_Erase;
TaskHandle_t Task_Erase_Handle;

/******************************************************************************
* Task used to erase end of snake
******************************************************************************/
void Task_Erase_Square(void *pvParameters)
{
    grid_box * cell;
    while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /**
         * Magic Detail: Only want to erase from the queue if
         * and only if the queue is sufficiently large
         *
         * This will only dequeue one element at a time, and
         * logically requires paint to have another motion
         * before going again.
         *
         * Allows for all current snake cells which
         * were painted to stay painted, only covering
         * up cell where snake was before
         */
        if(uxQueueMessagesWaiting(Queue_Erase) >= snake_size+1){
            xQueueReceive(Queue_Erase, &cell, portMAX_DELAY);
            cell->is = Background;
            lcd_draw_rectangle(
                    cell->x,
                    cell->y,
                    UNIT_SIZE,
                    UNIT_SIZE,
                    color_bg
            );
        }
        xTaskNotify(
            Task_Optical_Handle,
            0,
            eNoAction
        );
    }
}


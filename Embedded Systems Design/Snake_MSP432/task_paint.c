/*
 * task_paint.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include "main.h"
#include "grid.h"

TaskHandle_t Task_Paint_Handle;

/******************************************************************************
* Task used to paint snake's position
******************************************************************************/
void Task_Paint_Square(void *pvParameters)
{
    while(1){
        // Kill LED from last blip
        ece353_rgb(false, false, false);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // On food encountered, consume food will
        // elongate snake and move food to new location
        // also will flash green and make quick beep
        if(grid[head_x][head_y].is == Food){
            consume_food();
            ece353_rgb(false, true, false);
            playTone();
        }

        // Move snake head to new location
        grid[head_x][head_y].is = Snake;
        lcd_draw_rectangle(
                grid[head_x][head_y].x,
                grid[head_x][head_y].y,
                UNIT_SIZE,
                UNIT_SIZE,
                color_fg
        );

        // Queue_Erase keeps track of all previous
        // head positions to erase them when snake
        // has moved far enough beyond current spot
        grid_box * dptr = &grid[head_x][head_y];
        xQueueSend(Queue_Erase, &dptr, portMAX_DELAY);
        xTaskNotify(
            Task_Erase_Handle,
            0,
            eNoAction
        );
    }
}


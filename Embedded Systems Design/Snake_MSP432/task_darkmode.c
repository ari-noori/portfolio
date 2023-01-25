/*
 * task_darkmode.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */
#include "main.h"
#include "grid.h"

// Examine queue of what needs to be errased (i.e. tiles to be redrawn)
QueueHandle_t Queue_Erase;
TaskHandle_t Task_Darkmode_Handle;

void flush_queue();

/******************************************************************************
* Task used to trigger redraw of game board
******************************************************************************/
void Task_Darkmode(void *pvParameters)
{
    while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        // Want to run this atomically to prevent distortion of Erase Queue
        taskENTER_CRITICAL();
        if(uxQueueMessagesWaiting(Queue_Erase)){
            lcd_draw_rectangle(66, 66, LCD_HORIZONTAL_MAX, LCD_VERTICAL_MAX, color_bg);
            draw_food();
        }
        flush_queue();
        taskEXIT_CRITICAL();
    }
}

/*
 * Recursive function to redraw everything in erase queue and put it back
 *
 * Removes item from erase, redraws it, then recurses until erase queue is empty
 * This allows one to reinsert items in queue by storing on stack as one iterates
 * through the queue, instead of having to use a secondary data structure
 */
void flush_queue(){
    if(!uxQueueMessagesWaiting(Queue_Erase))
        return;

    grid_box * cell;
    xQueueReceive(Queue_Erase, &cell, portMAX_DELAY);
    lcd_draw_rectangle(
            cell->x,
            cell->y,
            UNIT_SIZE,
            UNIT_SIZE,
            color_fg
    );
    flush_queue();

    // Executes backwards off recursive stack so sending to front preserves
    // original ordering
    xQueueSendToFront(Queue_Erase, &cell, portMAX_DELAY);
}

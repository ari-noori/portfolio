/*
 * task_optical.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */
#include "main.h"
#include "grid.h"

TaskHandle_t Task_Optical_Handle;

unsigned color_fg;
unsigned color_bg;

/******************************************************************************
* Task used to detect changes in light levels
******************************************************************************/
void Task_Optical(void *pvParameters)
{
    int old_color_bg = color_bg;
    while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Read light levels
        const float lux = opt3001_read_lux();
        if(lux < 500){
            color_fg = OUROBOROUS_BLUE;
            color_bg = LCD_COLOR_BLACK;
        }
        else {
            color_fg = OUROBOROUS_BLUE;
            color_bg = OUROBOROUS_BEIGE;
        }

        // Change color scheme if change necessitated
        if(color_bg != old_color_bg)
            xTaskNotify(
                Task_Darkmode_Handle,
                0,
                eNoAction
            );
        old_color_bg = color_bg;
    }
}


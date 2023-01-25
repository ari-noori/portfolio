/*
 * main.c
 *
 *  Created on: May 2, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */

/* External files */
#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>

#include "main.h"
/* Student-made header files*/
#include "lcd.h"
#include "grid.h"
#include "serial_debug.h"
#include "leaderboard.h"
#include "peripherals.h"

/* FreeRTOS tasks*/
#include <FreeRTOS.h>
#include <task.h>
#include "task_paint.h"
#include "task_joystick.h"
#include "task_music.h"


/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // Init peripherals and global variables
    jerk = 0;
    Crystalfontz128x128_Init();
    i2c_init();
    opt3001_init();
    grid_init();
    serial_debug_init();
    adc_comparator_init();
    MKII_buttons_init();
    __enable_irq();
    game_in_progress = 0;
    timer_ms = 40;


    // Initialize the two global queues
    Queue_Erase = xQueueCreate(GRID_LEN*GRID_LEN, sizeof(grid_box * ));
    Queue_Music = xQueueCreate(2, sizeof(song_t));

    // Kickoff theme song and show home splash screen
    lcd_splash_screen(Ouroboros);
    song_t intro = Intro;
    xQueueSend(Queue_Music, &intro, portMAX_DELAY);

    // Task: Joystick bottom half to handle movement
    xTaskCreate
    (   Task_Joystick_Bottom_Half,
        "Task_Joystick",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &Task_Joystick_Handle
    );

    // Task: Paint box where snake moves to
    xTaskCreate
    (   Task_Paint_Square,
        "Task_Paint_Square",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &Task_Paint_Handle
    );

    // Task: Erase last spot where snake was
    xTaskCreate
    (   Task_Erase_Square,
        "Task_Erase_Square",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &Task_Erase_Handle
    );

    // Task: Play music - has higher priority than other tasks
    // so as to ursurp them on priority
    xTaskCreate
    (   Task_Music,
        "Task_Music",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_Music_Handle
    );

    // Task: Monitor timing for top half of ADC ISR kickoffs
    xTaskCreate
    (   Task_Joystick_Timer,
        "Task_Joystick_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &Task_Joystick_Timer_Handle
    );

    // Task: Monitor light levels to detect if dark mode should
    // be used
    xTaskCreate
    (  Task_Optical,
       "Task_Optical",
       configMINIMAL_STACK_SIZE,
       NULL,
       1,
       &Task_Optical_Handle
    );

    // Task: Updates game board for dark mode
    xTaskCreate
    (  Task_Darkmode,
       "Task_Darkmode",
       configMINIMAL_STACK_SIZE,
       NULL,
       1,
       &Task_Darkmode_Handle
    );


    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while(1){};
    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}

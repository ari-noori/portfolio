/*
 * task_joystick.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include "main.h"
#include "grid.h"

 TaskHandle_t Task_Joystick_Handle;
 TaskHandle_t Task_Joystick_Timer_Handle;

volatile uint32_t JOYSTICK_X_DIR = 0;
volatile uint32_t JOYSTICK_Y_DIR = 0;
volatile uint32_t ACCEL_X_DIR = 0;
volatile uint32_t ACCEL_Y_DIR = 0;
volatile uint32_t ACCEL_Z_DIR = 0;
volatile bool game_in_progress;
volatile bool jerk;
volatile int timer_ms;
int handle_joystick_dir(JOYSTICK_DIR_t dir, JOYSTICK_DIR_t direction_before_center);

/******************************************************************************
* Bottom Half Task.  Examines the ADC data from the joystick on the MKII
******************************************************************************/
void Task_Joystick_Bottom_Half(void *pvParameters)
{
    JOYSTICK_DIR_t dir;
    JOYSTICK_DIR_t dir_before_center = JOYSTICK_DIR_UP;

    while(1)
    {
        // Wait until called from top half
        // When ADC conversion is ready
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Assign dir enumeration based on joystick voltage level
        if(JOYSTICK_X_DIR < VOLT_MIN)
           dir = JOYSTICK_DIR_LEFT;
        else if(JOYSTICK_X_DIR > VOLT_MAX)
            dir = JOYSTICK_DIR_RIGHT;
        else if(JOYSTICK_Y_DIR < VOLT_MIN)
            dir = JOYSTICK_DIR_DOWN;
        else if(JOYSTICK_Y_DIR > VOLT_MAX)
            dir = JOYSTICK_DIR_UP;
        else
            dir = JOYSTICK_DIR_CENTER;

        // Manually override to same direction if user tries to go directly backwards
        if ((dir == JOYSTICK_DIR_LEFT && dir_before_center == JOYSTICK_DIR_RIGHT)   ||
            (dir == JOYSTICK_DIR_RIGHT && dir_before_center == JOYSTICK_DIR_LEFT)  ||
            (dir == JOYSTICK_DIR_UP && dir_before_center == JOYSTICK_DIR_DOWN)      ||
            (dir == JOYSTICK_DIR_DOWN && dir_before_center == JOYSTICK_DIR_UP))
                dir = dir_before_center;

        // Update snake head and check that snake not overlapping with self, or edges
        handle_joystick_dir(dir, dir_before_center);
        validate_new_position();

        // Keep track of last valid noncentered joystick on behalf of
        // manual override above
        if(dir != JOYSTICK_DIR_CENTER)
            dir_before_center = dir;

        xTaskNotify(
            Task_Paint_Handle,
            0,
            eNoAction
        );
    }
}

/******************************************************************************
* Top Half of ADC14 Handler.
******************************************************************************/
void ADC14_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Joystick ADC
    uint32_t jx = ADC14->MEM[0];
    uint32_t jy = ADC14->MEM[1];

    // Accelerometer ADC
    uint32_t ax = ADC14->MEM[2];
    uint32_t ay = ADC14->MEM[3];
    uint32_t az = ADC14->MEM[4];

    // Read the value and clear the interrupt
    if(jerk){
        JOYSTICK_X_DIR = ax;
        JOYSTICK_Y_DIR = ay;
    } else {
        JOYSTICK_X_DIR = jx;
        JOYSTICK_Y_DIR = jy;
    }

    // Used for seeding where food spawns
    ACCEL_X_DIR = ax;
    ACCEL_Y_DIR = ay;
    ACCEL_Z_DIR = az;

    // Kickoff bottom half or initializes game
    if(game_in_progress){
        vTaskNotifyGiveFromISR(
                Task_Joystick_Handle,
                &xHigherPriorityTaskWoken
        );

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    else
        init_game();

}

// Helper switch statement for incrementing head position based upon direction
int handle_joystick_dir(JOYSTICK_DIR_t dir, JOYSTICK_DIR_t direction_before_center){
    switch(dir){
           case JOYSTICK_DIR_CENTER:
               return handle_joystick_dir(direction_before_center, direction_before_center);
           case JOYSTICK_DIR_LEFT:
               return head_x--;
           case JOYSTICK_DIR_RIGHT:
               return head_x++;
           case JOYSTICK_DIR_UP:
               return head_y--;
           case JOYSTICK_DIR_DOWN:
               return head_y++;
           }
    return 0;
}

TaskHandle_t Task_Joystick_Handle;
TaskHandle_t Task_Joystick_Timer_Handle;

 /******************************************************************************
 * Used to start an ADC14 Conversion
 ******************************************************************************/
 void Task_Joystick_Timer(void *pvParameters)
 {
     while(1)
     {
         /*
          * Start the ADC conversion
          */
         ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

         /*
          * Delay 50mS
          */
         vTaskDelay(pdMS_TO_TICKS(timer_ms));

     }
 }


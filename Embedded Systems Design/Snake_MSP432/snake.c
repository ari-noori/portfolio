/*
 * task_joystick.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include "snake.h"

// Store location for redraws too
grid_box * food_location;

// Elongates snake and redraws food in new spot
void consume_food(){
    snake_size++;
    srand(ACCEL_Z_DIR * ACCEL_Y_DIR / ACCEL_X_DIR);

    int x,y;
    do {
        x = rand() % (GRID_LEN - 3) + 3;
        y = rand() % (GRID_LEN - 3) + 3;
    } while (grid[x][y].is != Background);

    grid[x][y].is = Food;
    food_location = &grid[x][y];
    draw_food();
}

// Redraws food on grid
void draw_food(){
    lcd_draw_rectangle(
            food_location->x,
            food_location->y,
            UNIT_SIZE,
            UNIT_SIZE,
            LCD_COLOR_RED
    );
}

// Initializes game settings
void init_game(){
    lcd_splash_screen(Select_Controls);

    while(1){
        // Note that game clock (timer_ms) is slower for
        // accelerometer as it is much more difficult
        if(S1_pressed()){
            jerk = 0;
            timer_ms = 40;
            break;
        }
        else if(S2_pressed()){
            jerk = 1;
            timer_ms = 60;
            break;
        }
        taskYIELD();
    }

    // Initialize our screen only after the game has started
    Crystalfontz128x128_Init();
    snake_size = 0;
    consume_food(); // Begin with size of 1, from incrementing on 0
    game_in_progress = 1;
}

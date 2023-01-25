/*
 * leaderboard.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include "splash_screens.h"
#include "leaderboard.h"
#include "serial_debug.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "music.h"
#include "task_joystick.h"
#include "msp.h"

 void add_leaderboard_entry(int score){
    ece353_rgb(false, true, false);
    song_t song = Outro;
    xQueueSend(Queue_Music, &song, portMAX_DELAY);
    taskENTER_CRITICAL();
    lcd_splash_screen(Leaderboard);
    allow_user_input(1);
    while(!rx_done);

    char buf[RX_BUFFER_MAX_SIZE * 2];
    char* mode = jerk ? "Accelerometer" : "Joystick";
    snprintf(buf, sizeof(buf), "\r%s:\t%d\t(%s)\r\n", buffer, score, mode);
    printf(buf);

    clear_rx_buffer();
    lcd_splash_screen(Play_Again);
    while(!S2_pressed());
//    taskEXIT_CRITICAL();
    RSTCTL->HARDRESET_SET = 1;
}



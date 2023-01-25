/*
 * task_music.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */
#include "main.h"
#include "task_music.h"

QueueHandle_t Queue_Music;
TaskHandle_t Task_Music_Handle;

/******************************************************************************
* Task used play songs as soon as received elsewhere
******************************************************************************/
void Task_Music(void *pvParameters)
{
    song_t track_name;
    while(1){
        xQueueReceive(Queue_Music, &track_name, portMAX_DELAY);
        music_play_song(track_name);
    }
}

/*
 * task_music.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#ifndef TASK_MUSIC_H_
#define TASK_MUSIC_H_
#include "music.h"

extern TaskHandle_t Task_Music_Handle;
void Task_Music(void *pvParameters);

#endif /* TASK_MUSIC_H_ */

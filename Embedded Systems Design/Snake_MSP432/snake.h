/*
 * Snake.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include "grid.h"
#include "task_joystick.h"

#ifndef SNAKE_H_
#define SNAKE_H_

int head_x;
int head_y;
int snake_size;

void draw_food(void);
void consume_food(void);
void init_game(void);

#endif /* SNAKE_H_ */

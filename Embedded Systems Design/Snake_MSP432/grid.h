/*
 * grid.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#ifndef GRID_H_
#define GRID_H_

#include "msp.h"
#include "lcd.h"
#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>

typedef enum {Background, Snake, Food} contents;

typedef struct grid_box {
    contents is;
    int x;
    int y;
} grid_box;

grid_box grid[GRID_LEN][GRID_LEN];

void grid_init(void);
void validate_new_position(void);

#endif /* GRID_H_ */

/*
 * grid.c
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include "grid.h"
#include "leaderboard.h"

void grid_init(){
    int i, j;
    for(i = 0; i < GRID_LEN; ++i)
        for(j = 0; j < GRID_LEN; ++j){
            grid[i][j].x = (i+1)*UNIT_SIZE;
            grid[i][j].y = (j+1)*UNIT_SIZE;
        }
    head_x = 7;
    head_y = 7;
}

void validate_new_position(){
    if( head_x < 0          ||
        head_x == GRID_LEN  ||
        head_y < 0          ||
        head_y == GRID_LEN  ||
        grid[head_x][head_y].is == Snake)
            add_leaderboard_entry(snake_size);
}

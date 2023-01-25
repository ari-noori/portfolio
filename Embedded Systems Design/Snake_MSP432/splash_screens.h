/*
 * splash_screens.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include <stdint.h>

#ifndef SPLASHSCREENS_H_
#define SPLASHSCREENS_H_

typedef enum image{Ouroboros, Select_Controls, Leaderboard, Play_Again} image;
extern const uint8_t ouroborousBitmaps[];
extern const uint8_t lossBitmaps[];
extern const uint8_t controlsBitmaps[];
extern const uint8_t play_againBitmaps[];

#endif /* SPLASHSCREENS_H_ */

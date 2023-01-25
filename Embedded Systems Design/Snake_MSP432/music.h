/*
 * music.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "peripherals.h"
#include "main.h"

//*****************************************************************************
// You will need to determine the number of SMCLKs it will
// take for the following notes and their associated frequencies.
//
// Assume that your MCU is running at 24MHz.
// Note: As clk freq is doubled, values here are multiplied by 2
//*****************************************************************************
#define NOTE_A5        27273*2 // 880 Hz
#define NOTE_A5S       25751*2 // 932 Hz
#define NOTE_B5        24291*2 // 988 Hz
#define NOTE_C6        22945*2 // 1046 Hz
#define NOTE_C6S       21641*2 // 1109 Hz
#define NOTE_D6        20426*2 // 1175 Hz
#define NOTE_D6S       19277*2 // 1245 Hz
#define NOTE_E6        18196*2 // 1319 Hz
#define NOTE_F6        17180*2 // 1397 Hz
#define NOTE_F6S       16216*2 // 1480 Hz
#define NOTE_G6        15306*2 // 1568 Hz
#define NOTE_G6S       14449*2 // 1661 Hz
#define NOTE_A6        13636*2 // 1760 Hz
#define NOTE_A6S       12869*2 // 1865 Hz
#define NOTE_B6        12146*2 // 1976 Hz
#define NOTE_C7        11467*2 // 2093 Hz
#define NOTE_C7S       10825*2 // 2217 Hz
#define NOTE_D7        10217*2 // 2349 Hz
#define NOTE_D7S       9642*2 // 2489 Hz
#define NOTE_E7        9101*2 // 2637 Hz
#define NOTE_F7        8590*2 // 2794 Hz
#define NOTE_F7S       8108*2 // 2960 Hz
#define NOTE_G7        7653*2 // 3136 Hz
#define NOTE_G7S       7225*2 // 3322 Hz
#define REST           0000 //

#define MEASURE_DURATION 12000000*2    // 500 milliseconds
#define MEASURE_RATIO           2    // 2/4 time
#define DELAY_AMOUNT       240000*2    // 10  milliseconds

extern QueueHandle_t Queue_Music;

typedef enum song_t {Intro, Outro} song_t;

//***************************************************************
//***************************************************************
void music_init(void);

//***************************************************************
//***************************************************************
void music_play_song(song_t);
void playTone();

#endif /* MUSIC_H_ */

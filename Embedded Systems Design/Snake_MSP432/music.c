/*
 * music.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */
#include "music.h"

typedef enum measure_time_t {
    ONE_HALF,
    ONE_QUARTER,
    ONE_EIGTH
} measure_time_t;

typedef struct Note_t{
    uint32_t period;
    measure_time_t time;
    bool delay;
} Note_t;

typedef struct Song_t{
    Note_t* song;
    unsigned length;
} Song_t;


//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time)
{
    uint32_t time_return = 0;

    time_return  = MEASURE_DURATION * MEASURE_RATIO;

    switch(time)
    {
        case ONE_HALF:
        {
            time_return  = time_return / 2;
            break;
        }
        case ONE_QUARTER:
        {
            time_return  = time_return / 4;
            break;
        }
        case ONE_EIGTH:
        {
            time_return  = time_return / 8;
            break;
        }
        default:
        {
            break;
        }
    }

    return time_return - DELAY_AMOUNT;

}

const Song_t track_list[];
//***************************************************************
// Plays the song (loop through, playing each note) 
// and then returns
//***************************************************************
void music_play_song(song_t track_name)
{
    // For debugging in the library, enable LED instead of sound
    const bool silent_mode = false;
    if(silent_mode){
        ece353_rgb_init();
        ece353_rgb(true, false, false);
    }

    ece353_rgb_init();
    ece353_rgb(true, false, false);

    const Note_t * Song = track_list[track_name].song;
    int i;
    for (i = 0; i < track_list[track_name].length / sizeof(Note_t); i++){
        // Initialize the buzzer with the period of the PWM Pulse
        ece353_MKII_Buzzer_Init(Song[i].period);

        // Turn the buzzer ON
        if(!silent_mode)
            ece353_MKII_Buzzer_On();
        // Let the buzzer run for the length of the note
        ece353_T32_2_wait(music_get_time_delay(Song[i].time));
        // Turn OFF the buzzer when the note is done
        ece353_MKII_Buzzer_Off();

        // If we have a delay, then wait for the delay amount
        if(Song[i].delay)
            ece353_T32_2_wait(DELAY_AMOUNT);
    }
    if(silent_mode)
        ece353_rgb(false, true, false);
}

// Plays quick note for food consumption
void playTone(){
    ece353_MKII_Buzzer_Init(NOTE_D6S);
    ece353_MKII_Buzzer_On();
    ece353_T32_2_wait(MEASURE_DURATION/36);
    ece353_MKII_Buzzer_Off();
}

// Guile's Theme from Street Fighter
const Note_t intro_song[] =
{
    {NOTE_D6S,ONE_QUARTER,false},
    {NOTE_D6S,ONE_EIGTH,false},
    {NOTE_D6,ONE_EIGTH,false},
    {REST,ONE_EIGTH,true},
    {NOTE_D6,ONE_EIGTH,false},
    {NOTE_D6S,ONE_HALF,false},
    {REST,ONE_QUARTER,false},
    {NOTE_D6,ONE_QUARTER,false},
    {NOTE_D6S,ONE_QUARTER,false},
    {NOTE_D6S,ONE_EIGTH,false},
    {NOTE_D6,ONE_EIGTH,false},
    {REST,ONE_EIGTH,true},
    {NOTE_D6,ONE_EIGTH,false},
    {NOTE_D6S,ONE_HALF,false},
    {REST,ONE_QUARTER,false},
    {NOTE_D6,ONE_QUARTER,true},
    {NOTE_D6S,ONE_EIGTH,true},
    {NOTE_D6,ONE_QUARTER,true},
    {NOTE_D6S,ONE_QUARTER,true},
    {NOTE_D6,ONE_QUARTER,true},
    {NOTE_F6,ONE_QUARTER,true},
    {NOTE_F6,ONE_QUARTER,true},
    {NOTE_D6S,ONE_EIGTH,true},
    {NOTE_D6,ONE_QUARTER,true},
    {NOTE_A5S,ONE_QUARTER,true},
    {NOTE_C6,4,false},
};

// Never Meant by American Football
Note_t outro_song[] =
{
    {NOTE_C6,ONE_QUARTER,true},  // Tone, Time, Delay
    {NOTE_E7,ONE_EIGTH,true},
    {NOTE_D7,ONE_EIGTH,true},
    {NOTE_C7,ONE_QUARTER,true},
    {NOTE_G6,ONE_QUARTER,true},
    {NOTE_A6,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},

    {NOTE_E7,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},
    {NOTE_B6,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},
    {NOTE_E7,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},

    {NOTE_D7,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},
    {NOTE_E7,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},
    {NOTE_E7,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},

    {NOTE_E7,ONE_QUARTER,true},
    {NOTE_F7,ONE_QUARTER,true},
    {NOTE_G7,ONE_QUARTER,true},
    {NOTE_C7,ONE_QUARTER,true},
};

// Songs accessible via their enum label in this array
const Song_t track_list[] = {{&intro_song, sizeof(intro_song)}, {&outro_song, sizeof(outro_song)}};

#ifndef NOTES_H_
#define NOTES_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

// SILENCE_THRESHOLD is a multiple of vTaskDelay found in the lcd_draw_foreground fn
#define SILENCE_THRESHOLD (5)
#define NOTE_HEIGHT 100
#define NOTE_WIDTH 80
#define TUNING_NOTE_HEIGHT 70
#define TUNING_NOTE_WIDTH 70
// b : flat | n : natural | s : sharp
#define Eb2 77
#define Eb2_Bucket 12.5
#define En2 82
#define En2_Bucket 12.5
#define Fn2 87
#define Fn2_Bucket 13.5
#define Fs2 93
#define Fs2_Bucket 14.5
#define Gb2 93
#define Gb2_Bucket 14.5
#define Gn2 98
#define Gn2_Bucket 14.5
#define Gs2 104
#define Gs2_Bucket 16.5
#define Ab2 104
#define Ab2_Bucket 16.5
#define An2 110
#define An2_Bucket 16.5
#define As2 116
#define As2_Bucket 17.5
#define Bb2 116
#define Bb2_Bucket 17.5
#define Cn3 130
#define Cn3_Bucket 18.5
#define Cs3 138
#define Cs3_Bucket 20.5
#define Db3 138
#define Db3_Bucket 20.5
#define Dn3 146
#define Dn3_Bucket 22.5
#define Ds3 155
#define Ds3_Bucket 23.5
#define Eb3 155
#define Ds3_Bucket 23.5
#define En3 164
#define En3_Bucket 24.5
#define Fn3 174
#define Fn3_Bucket 26.5
#define Fs3 185
#define Fs3_Bucket 28.5
#define Gb3 185
#define Gb3_Bucket 28.5
#define Gn3 196
#define Gn3_Bucket 28.5
#define Gs3 207
#define Gs3_Bucket 29.5
#define Ab3 207
#define Ab3_Bucket 29.5
#define An3 220
#define An3_Bucket 32.5
#define As3 233
#define As3_Bucket 34.5
#define Bb3 233
#define Bb3_Bucket 34.5
#define Bn3 246
#define Bn3_Bucket 36.5
#define Cn4 261
#define Cn4_Bucket 38.5
#define Cs4 277
#define Cs4_Bucket 40.5
#define Db4 277
#define Db4_Bucket 40.5
#define Dn4 293
#define Dn4_Bucket 42.5
#define Ds4 311
#define Ds4_Bucket 46.5
#define Eb4 311
#define Eb4_Bucket 46.5
#define En4 329
#define En4_Bucket 48.5
#define Fn4 349
#define Fn4_Bucket 50.5
#define Fs4 370
#define Fs4_Bucket 54.5
#define Gb4 370
#define Gb4_Bucket 54.5
#define Gn4 392
#define Gn4_Bucket 58.5
#define Gs4 415
#define Gs4_Bucket 60.5
#define Ab4 415
#define Ab4_Bucket 60.5
#define An4 440
#define An4_Bucket 64.5
#define As4 466
#define As4_Bucket 68.5
#define Bb4 466
#define Bb4_Bucket 68.5
#define Bn4 493
#define Bn4_Bucket 72.5
#define Cn5 523
#define Cn5_Bucket 76.5
#define Cs5 554
#define Cs5_Bucket 82.5
#define Db5 554
#define Db5_Bucket 82.5
#define Dn5 587
#define Dn5_Bucket 86.5
#define Ds5 622
#define Ds5_Bucket 92.5
#define Eb5 622
#define Eb5_Bucket 92.5
#define En5 659
#define En5_Bucket 96.5
#define Fn5 698
#define Fn5_Bucket 102.5
#define Fs5 740
#define Fs5_Bucket 108.5
#define Gb5 740
#define Gb5_Bucket 108.5
#define Gn5 790
#define Gn5_Bucket 116.5
#define Gs5 830
#define Gs5_Bucket 122.5
#define Ab5 830
#define Ab5_Bucket 122.5
#define An5 880
#define An5_Bucket 129.5
#define As5 932
#define As5_Bucket 136.5
#define Bb5 932
#define Bb5_Bucket 136.5
#define Bn5 988
#define Bn5_Bucket 144.5
#define Cn6 1046
#define Cn6_Bucket 154.5
#define Cs6 1108
#define Cs6_Bucket 162.5
#define Db6 1108
#define Db6_Bucket 162.5
#define Dn6 1174
#define Dn6_Bucket 172.5
#define Ds6 1244
#define Ds6_Bucket 182.5
#define Eb6 1244
#define Eb6_Bucket 182.5
#define En6 1318
#define En6_Bucket 194.5
#define REST 0.

#define FLAT_HI_E ((Eb4 + En4)/2)
#define SHRP_HI_E ((En4 + Fn4)/2)
#define FLAT_B ((Bb3 + Bn3)/2)
#define SHRP_B ((Bn3 + Cn4)/2)
#define FLAT_G ((Gb3 + Gn3)/2)
#define SHRP_G ((Gn3 + Gs3)/2)
#define FLAT_D ((Db3 + Dn3)/2)
#define SHRP_D ((Dn3 + Ds3)/2)
#define FLAT_A ((Ab2 + An2)/2)
#define SHRP_A ((An2 + As2)/2)
#define FLAT_LO_E ((Eb2 + En2)/2)
#define SHRP_LO_E ((En2 + Fn2)/2)

#define FLAT_HI_E_Bucket ((Eb4_Bucket + En4_Bucket)/2)
#define SHRP_HI_E_Bucket ((En4_Bucket + Fn4_Bucket)/2)
#define FLAT_B_Bucket ((Bb3_Bucket + Bn3_Bucket)/2)
#define SHRP_B_Bucket ((Bn3_Bucket + Cn4_Bucket)/2)
#define FLAT_G_Bucket ((Gb3_Bucket + Gn3_Bucket)/2)
#define SHRP_G_Bucket ((Gn3_Bucket + Gs3_Bucket)/2)
#define FLAT_D_Bucket ((Db3_Bucket + Dn3_Bucket)/2)
#define SHRP_D_Bucket ((Dn3_Bucket + Ds3_Bucket)/2)
#define FLAT_A_Bucket ((Ab2_Bucket + An2_Bucket)/2)
#define SHRP_A_Bucket ((An2_Bucket + As2_Bucket)/2)
#define FLAT_LO_E_Bucket ((Eb2_Bucket + En2_Bucket)/2)
#define SHRP_LO_E_Bucket ((En2_Bucket + Fn2_Bucket)/2)

typedef enum note_label {
	a_natural,
	a_sharp,
	a_flat,
	b_natural,
	b_flat,
	c_natural,
	c_sharp,
	d_flat,
	d_natural,
	d_sharp,
	e_flat,
	e_natural,
	f_natural,
	f_sharp,
	g_flat,
	g_natural,
	g_sharp,
	rest, // Note: only used for play back and has no associated image
	END_OF_SONG
} note_label_t;


extern const uint16_t * tuningImages[6];

typedef struct note {
	double frequency;
	note_label_t label;
} note_t;

typedef struct tuning_frequency{
	uint8_t string;
	double prev_note;
	double target_note;
	double next_note;
} tuning_frequency_t;
extern const tuning_frequency_t tuning[6];

extern tuning_frequency_t * get_closest_note(uint32_t);
extern const uint16_t * notes[];
extern const uint16_t aSharp[];
extern const uint16_t aNatural[];
extern const uint16_t aFlat[];
extern const uint16_t bNatural[];
extern const uint16_t bFlat[];
extern const uint16_t cSharp[];
extern const uint16_t cNatural[];
extern const uint16_t dSharp[];
extern const uint16_t dNatural[];
extern const uint16_t dFlat[];
extern const uint16_t eNatural[];
extern const uint16_t eFlat[];
extern const uint16_t fSharp[];
extern const uint16_t fNatural[];
extern const uint16_t gSharp[];
extern const uint16_t gNatural[];
extern const uint16_t gFlat[];



extern const uint16_t b3Tune[];
extern const uint16_t a2Tune[];
extern const uint16_t d3Tune[];
extern const uint16_t g3Tune[];
extern const uint16_t e2Tune[];
extern const uint16_t e4Tune[];

#endif

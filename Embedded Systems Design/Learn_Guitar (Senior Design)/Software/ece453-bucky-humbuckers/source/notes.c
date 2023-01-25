/*
 * notes.c
 *
 *  Created on: Nov 03, 2022
 *      Author: cohen
 */
#include "notes.h"

const uint16_t * notes[] = {
		aNatural,
		aSharp,
		aFlat,
		bNatural,
		bFlat,
		cNatural,
		cSharp,
		dFlat,
		dNatural,
		dSharp,
		eFlat,
		eNatural,
		fNatural,
		fSharp,
		gFlat,
		gNatural,
		gSharp
};

const uint16_t * tuningImages[] = {
		e4Tune,
		b3Tune,
		g3Tune,
		d3Tune,
		a2Tune,
		e2Tune
};

const tuning_frequency_t tuning[] = {
		{
			1,
			Eb4,
			En4,
			Fn4
		},
		{
			2,
			Bb3,
			Bn3,
			Cn4
		},
		{
			3,
			Gb3,
			Gn3,
			Gs3
		},
		{
			4,
			Db3,
			Dn3,
			Ds3
		},
		{
			5,
			Ab2,
			An2,
			As2,
		},
		{
			6,
			Eb2,
			En2,
			Fn2
		}
};
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
tuning_frequency_t * get_closest_note(uint32_t frequency){
	uint16_t min_diff = 1024;
	tuning_frequency_t * tuning_frequency;
	for(uint8_t note = 0; note < 6; ++note) {
		const uint16_t diff = abs(tuning[note].target_note - frequency);
		if(diff < min_diff){
			min_diff = diff;
			tuning_frequency = (tuning_frequency_t*)&tuning[note];
		}
	}
	return tuning_frequency;
}

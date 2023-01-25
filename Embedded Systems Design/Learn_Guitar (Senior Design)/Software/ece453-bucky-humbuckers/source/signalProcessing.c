/*
 * signalProcessing.c
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

/*******************************************************************************
 * Equation to find where the bucket should be
 * y = -8.31e-11x^4 + 1.61e-7x^3 - 1.044e-4x^2 + .179x - .832
 * Size: 1024 samples
 * Nyquist: 7812.5 Hz
 * Bucket Range: 7.6 Hz
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "signalProcessing.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Static Variable Definitions
 ******************************************************************************/
static arm_rfft_fast_instance_f32 fftInstance;
/*******************************************************************************
 * Global Variable Definitions
 ******************************************************************************/
float fftOutput[FFT_LENGTH] = {0};
bool signalProcessingPrint = false;
/*******************************************************************************
 * Static Function Definitions
 ******************************************************************************/

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/
void vSignalProcessingTask(void *pvParameters) {
	configASSERT(((uint32_t ) pvParameters) == 5);

	arm_rfft_fast_init_f32 (&fftInstance, FFT_LENGTH);

	float fftInput[FFT_LENGTH];

	while (1) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		xSemaphoreTake(adcSemaphore, portMAX_DELAY);
		for(uint16_t i = 0; i < FFT_LENGTH; i ++){
			fftInput[i] = (float)adcData[i];
		}
		xSemaphoreGive(adcSemaphore);
		xSemaphoreTake(signalProcessingSemaphore, portMAX_DELAY);
		arm_rfft_fast_f32(&fftInstance, fftInput, fftOutput, 0);
		xSemaphoreGive(signalProcessingSemaphore);
		fftOutput[0] = 0;
		if(signalProcessingPrint){
//			for(uint16_t i = 0; i < FFT_LENGTH; i ++){
//				cliPrintln((uint8_t*)"%.4f", fftOutput[i]);
//				vTaskDelay(1);
//			}
			cliPrintln((uint8_t*)"\r\nMAXIMUMS");
			for(uint16_t i = 1; i < FFT_LENGTH; i ++){
				if(abs(fftOutput[i]) > BUCKET_THRESHOLD){
//					float frequency = i * FFT_BUCKET_WIDTH;
					cliPrintln((uint8_t*)"Index: %-9i %-21.02f", i, fftOutput[i]);
					vTaskDelay(1);
				}
			}
			vTaskDelay(1);
		}
	}
}

bool getNoteStatus(uint16_t noteInHertz){
//	cliPrintln((uint8_t*)"Called with arg %d", noteInHertz);
	xSemaphoreTake(signalProcessingSemaphore, portMAX_DELAY);

	bool thresholdReached = false;
	float noteBucket = 0.0;
	switch (noteInHertz){
		case (Eb2):
			noteBucket = Eb2_Bucket;
			break;
		case (En2):
			noteBucket = En2_Bucket;
			break;
		case (Fn2):
			noteBucket = Fn2_Bucket;
			break;
		case (Fs2):
			noteBucket = Fs2_Bucket;
			break;
//		case (Gb2):
//			noteBucket = Gb2_Bucket;
//			break;
		case (Gn2):
			noteBucket = Gn2_Bucket;
			break;
		case (Gs2):
			noteBucket = Gs2_Bucket;
			break;
//		case (Ab2):
//			noteBucket = Ab2_Bucket;
//			break;
		case (An2):
			noteBucket = An2_Bucket;
			break;
		case (As2):
			noteBucket = As2_Bucket;
			break;
//		case (Bb2):
//			noteBucket = Bb2_Bucket;
//			break;
		case (Cn3):
			noteBucket = Cn3_Bucket;
			break;
		case (Cs3):
			noteBucket = Cs3_Bucket;
			break;
//		case (Db3):
//			noteBucket = Db3_Bucket;
//			break;
		case (Dn3):
			noteBucket = Dn3_Bucket;
			break;
		case (Ds3):
			noteBucket = Ds3_Bucket;
			break;
//		case (Eb3):
//			noteBucket = Eb3_Bucket;
//			break;
		case (En3):
			noteBucket = En3_Bucket;
			break;
		case (Fn3):
			noteBucket = Fn3_Bucket;
			break;
		case (Fs3):
			noteBucket = Fs3_Bucket;
			break;
//		case (Gb3):
//			noteBucket = Gb3_Bucket;
//			break;
		case (Gn3):
			noteBucket = Gn3_Bucket;
			break;
		case (Gs3):
			noteBucket = Gs3_Bucket;
			break;
//		case (Ab3):
//			noteBucket = Ab3_Bucket;
//			break;
		case (An3):
			noteBucket = An3_Bucket;
			break;
		case (As3):
			noteBucket = As3_Bucket;
			break;
//		case (Bb3):
//			noteBucket = Bb3_Bucket;
//			break;
		case (Bn3):
			noteBucket = Bn3_Bucket;
			break;
		case (Cn4):
			noteBucket = Cn4_Bucket;
			break;
		case (Cs4):
			noteBucket = Cs4_Bucket;
			break;
//		case (Db4):
//			noteBucket = Db4_Bucket;
//			break;
		case (Dn4):
			noteBucket = Dn4_Bucket;
			break;
		case (Ds4):
			noteBucket = Ds4_Bucket;
			break;
//		case (Eb4):
//			noteBucket = Eb4_Bucket;
//			break;
		case (En4):
			noteBucket = En4_Bucket;
			break;
		case (Fn4):
			noteBucket = Fn4_Bucket;
			break;
		case (Fs4):
			noteBucket = Fs4_Bucket;
			break;
//		case (Gb4):
//			noteBucket = Gb4_Bucket;
//			break;
		case (Gn4):
			noteBucket = Gn4_Bucket;
			break;
		case (Gs4):
			noteBucket = Gs4_Bucket;
			break;
//		case (Ab4):
//			noteBucket = Ab4_Bucket;
//			break;
		case (An4):
			noteBucket = An4_Bucket;
			break;
		case (As4):
			noteBucket = As4_Bucket;
			break;
//		case (Bb4):
//			noteBucket = Bb4_Bucket;
//			break;
		case (Bn4):
			noteBucket = Bn4_Bucket;
			break;
		case (Cn5):
			noteBucket = Cn5_Bucket;
			break;
		case (Cs5):
			noteBucket = Cs5_Bucket;
			break;
//		case (Db5):
//			noteBucket = Db5_Bucket;
//			break;
		case (Dn5):
			noteBucket = Dn5_Bucket;
			break;
		case (Ds5):
			noteBucket = Ds5_Bucket;
			break;
//		case (Eb5):
//			noteBucket = Eb5_Bucket;
//			break;
		case (En5):
			noteBucket = En5_Bucket;
			break;
		case (Fn5):
			noteBucket = Fn5_Bucket;
			break;
		case (Fs5):
			noteBucket = Fs5_Bucket;
			break;
//		case (Gb5):
//			noteBucket = Gb5_Bucket;
//			break;
		case (Gn5):
			noteBucket = Gn5_Bucket;
			break;
		case (Gs5):
			noteBucket = Gs5_Bucket;
			break;
//		case (Ab5):
//			noteBucket = Ab5_Bucket;
//			break;
		case (An5):
			noteBucket = An5_Bucket;
			break;
		case (As5):
			noteBucket = As5_Bucket;
			break;
//		case (Bb5):
//			noteBucket = Bb5_Bucket;
//			break;
		case (Bn5):
			noteBucket = Bn5_Bucket;
			break;
		case (Cn6):
			noteBucket = Cn6_Bucket;
			break;
		case (Cs6):
			noteBucket = Cs6_Bucket;
			break;
//		case (Db6):
//			noteBucket = Db6_Bucket;
//			break;
		case (Dn6):
			noteBucket = Dn6_Bucket;
			break;
		case (Ds6):
			noteBucket = Ds6_Bucket;
			break;
//		case (Eb6):
//			noteBucket = Eb6_Bucket;
//			break;
		case (En6):
			noteBucket = En6_Bucket;
			break;
		case (FLAT_HI_E):
			noteBucket = FLAT_HI_E_Bucket;
			break;
		case (SHRP_HI_E):
			noteBucket = SHRP_HI_E_Bucket;
			break;
		case (FLAT_B):
			noteBucket = FLAT_B_Bucket;
			break;
		case (SHRP_B):
			noteBucket = SHRP_B_Bucket;
			break;
		case (FLAT_G):
			noteBucket = FLAT_G_Bucket;
			break;
		case (SHRP_G):
			noteBucket = SHRP_G_Bucket;
			break;
		case (FLAT_D):
			noteBucket = FLAT_D_Bucket;
			break;
		case (SHRP_D):
			noteBucket = SHRP_D_Bucket;
			break;
		case (FLAT_A):
			noteBucket = FLAT_A_Bucket;
			break;
		case (SHRP_A):
			noteBucket = SHRP_A_Bucket;
			break;
		case (FLAT_LO_E):
			noteBucket = FLAT_LO_E_Bucket;
			break;
		case (SHRP_LO_E):
			noteBucket = SHRP_LO_E_Bucket;
			break;
		default:
			cliPrintln((uint8_t*) "No match for above frequency");
	}
	if((abs((uint32_t)fftOutput[(uint32_t)floor(noteBucket)]) >= BUCKET_THRESHOLD) ||
			(abs((uint32_t)fftOutput[(uint32_t)ceil(noteBucket)]) >= BUCKET_THRESHOLD)){
		thresholdReached = true;
	}
//	cliPrint("Bucket amplitude is %lu but threshold is %lu", (uint32_t)fftOutput[(uint32_t)floor(noteBucket)], BUCKET_THRESHOLD);
//	vTaskDelay(100);
	xSemaphoreGive(signalProcessingSemaphore);
	return thresholdReached;
}

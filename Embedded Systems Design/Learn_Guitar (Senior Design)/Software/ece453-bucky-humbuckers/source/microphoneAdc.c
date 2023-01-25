/*
 * adc.c
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "microphoneAdc.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Static Variable Definitions
 ******************************************************************************/
static cyhal_adc_t adcDevice;
static cyhal_adc_channel_t adcChannelOne;
/*******************************************************************************
 * Global Variable Definitions
 ******************************************************************************/
int32_t adcData[FFT_LENGTH] = {0};
bool adcPrint = false;
/*******************************************************************************
 * Static Function Definitions
 ******************************************************************************/
static void adcInterruptHandler(void* arg, cyhal_adc_event_t event){
	xSemaphoreGiveFromISR(adcSemaphore, NULL);
	xTaskNotifyFromISR(signalProcessingTaskHandle, 5, eNoAction, NULL);
	xTaskNotifyFromISR(adcTaskHandle, 5, eNoAction, NULL);
}
/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/
void vAdcTask(void *pvParameters) {
	configASSERT(((uint32_t ) pvParameters) == 2);
	cy_rslt_t status = CY_RSLT_SUCCESS;

	status = cyhal_adc_init(&adcDevice, ADC_PIN, NULL);
	CY_ASSERT(status == CY_RSLT_SUCCESS);

    const cyhal_adc_channel_config_t adcChannelConfiguration = {
    		.enable_averaging = false,
			.min_acquisition_ns = 200000000,
			.enabled = true
    };

    status = cyhal_adc_channel_init_diff(&adcChannelOne, &adcDevice, ADC_PIN, CYHAL_ADC_VNEG, &adcChannelConfiguration);
	CY_ASSERT(status == CY_RSLT_SUCCESS);

	uint32_t samplingRate = 0;
	status = cyhal_adc_set_sample_rate(&adcDevice, 10000, &samplingRate);
	CY_ASSERT(status == CY_RSLT_SUCCESS);

	status = cyhal_adc_set_power(&adcDevice, CYHAL_POWER_LEVEL_HIGH);
	CY_ASSERT(status == CY_RSLT_SUCCESS);

    cyhal_adc_register_callback(&adcDevice, &adcInterruptHandler, adcData);

    cyhal_adc_enable_event(&adcDevice, CYHAL_ADC_ASYNC_READ_COMPLETE, 4, true);

	while (1) {
		xSemaphoreTake(adcSemaphore, portMAX_DELAY);
		cyhal_adc_read_async(&adcDevice, FFT_LENGTH, adcData);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(adcPrint){
			for(uint16_t i = 0; i < FFT_LENGTH; i ++){
				cliPrintln((uint8_t*)"%i", adcData[i]);
				vTaskDelay(1);
			}
		}
	}
}

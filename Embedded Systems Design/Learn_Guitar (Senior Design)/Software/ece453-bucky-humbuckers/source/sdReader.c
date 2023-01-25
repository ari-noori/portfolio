/*
 * sdReader.c
 *
 *  Created on: Nov 9, 2022
 *      Author: tanow
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdReader.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Static Variable Definitions
 ******************************************************************************/

/*******************************************************************************
 * Global Variable Definitions
 ******************************************************************************/

/*******************************************************************************
 * Static Function Definitions
 ******************************************************************************/

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/
void sdLoadImage(uint8_t* filename, uint8_t* data, uint32_t bytesToRead){
	xSemaphoreTake(spiSemaphore, portMAX_DELAY);
	memset(data, '\0', BACKGROUND_IMAGE_LENGTH);
	FRESULT fr = FR_OK;

	cyhal_gpio_write(SD_NCS_PIN, 0);
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	CY_ASSERT(fr == FR_OK);
	cyhal_gpio_write(SD_NCS_PIN, 1);

	cyhal_gpio_write(SD_NCS_PIN, 0);
	fr = f_mkfs("", 0, data, sizeof(data));
//	CY_ASSERT(fr == FR_OK);
	cyhal_gpio_write(SD_NCS_PIN, 1);
	cyhal_gpio_write(SD_NCS_PIN, 0);
	fr = f_open(&Fil, (const TCHAR*)filename, FA_READ | FA_OPEN_EXISTING);	/* Open a song file */
	cyhal_gpio_write(SD_NCS_PIN, 1);
	if (fr == FR_OK) {
		cyhal_gpio_write(SD_NCS_PIN, 0);
		UINT bytesRead = 0;
		// Start reading from the start
		fr = f_lseek(&Fil, 0);
		CY_ASSERT(fr == FR_OK);
		// Read the data from the file
		fr = f_read(&Fil, data, bytesToRead, &bytesRead);
		CY_ASSERT(fr == FR_OK);
		cyhal_gpio_write(SD_NCS_PIN, 1);
		cyhal_gpio_write(SD_NCS_PIN, 0);
		fr = f_close(&Fil);							/* Close the file */
		CY_ASSERT(fr == FR_OK);
		cyhal_gpio_write(SD_NCS_PIN, 1);
	}
	xSemaphoreGive(spiSemaphore);
}

void sdInit(){
	cy_rslt_t result = CY_RSLT_SUCCESS;

	result = cyhal_gpio_init(SD_NCS_PIN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);
}

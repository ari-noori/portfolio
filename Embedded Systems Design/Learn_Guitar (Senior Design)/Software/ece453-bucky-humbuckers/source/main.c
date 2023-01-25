
#include "main.h"

TaskHandle_t cliTaskHandle;
TaskHandle_t buttonTaskHandle;
TaskHandle_t adcTaskHandle;
TaskHandle_t lcdTaskHandle;
TaskHandle_t piezoTaskHandle;
TaskHandle_t signalProcessingTaskHandle;

SemaphoreHandle_t cliSemaphore;
SemaphoreHandle_t adcSemaphore;
SemaphoreHandle_t spiSemaphore;
SemaphoreHandle_t signalProcessingSemaphore;

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */

uint8_t sdBuffer[BACKGROUND_IMAGE_LENGTH] = {0};


int main(void){
	// cyhal_gpio_init(LCD_RESET_PIN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
	// cyhal_gpio_write(LCD_RESET_PIN, 1);

	cy_rslt_t result;
	BaseType_t taskReturn = pdPASS;

	result = cybsp_init();
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// enable NVIC
	__enable_irq();

	cliSemaphore = xSemaphoreCreateBinary();
	CY_ASSERT(cliSemaphore != NULL);
    xSemaphoreGive(cliSemaphore);

	adcSemaphore = xSemaphoreCreateBinary();
	CY_ASSERT(adcSemaphore != NULL);
    xSemaphoreGive(adcSemaphore);

	spiSemaphore = xSemaphoreCreateBinary();
	CY_ASSERT(spiSemaphore != NULL);
    xSemaphoreGive(spiSemaphore);

	signalProcessingSemaphore = xSemaphoreCreateBinary();
	CY_ASSERT(signalProcessingSemaphore != NULL);
    xSemaphoreGive(signalProcessingSemaphore);

	commandLineInit();
	touchscreenInit();
	sdInit();

	//    taskReturn = xTaskCreate(vTaskCode, "NAME", STACK_SIZE, parameter, priority, &xHandle );
	taskReturn = xTaskCreate(vCommandLineTask, 		"Command Line Interface Task", 	1024, (void*) 0, 0, &cliTaskHandle);
	CY_ASSERT(taskReturn == pdPASS);
	taskReturn = xTaskCreate(vButtonTask, 			"GPIO Button Task", 			 128, (void*) 1, 1, &buttonTaskHandle);
	CY_ASSERT(taskReturn == pdPASS);
	taskReturn = xTaskCreate(vAdcTask, 				"Analog Digital Converter Task",4096, (void*) 2, 2, &adcTaskHandle);
	CY_ASSERT(taskReturn == pdPASS);
	taskReturn = xTaskCreate(vLcdTask, 				"LCD Task", 					 2048, (void*) 3, 3, &lcdTaskHandle);
	CY_ASSERT(taskReturn == pdPASS);
//	taskReturn = xTaskCreate(vPiezoTask, 			"Piezo Task", 					 128, (void*) 4, 4, &piezoTaskHandle);
//	CY_ASSERT(taskReturn == pdPASS);
	taskReturn = xTaskCreate(vSignalProcessingTask, "Signal Processing Task", 		4096, (void*) 5, 5, &signalProcessingTaskHandle);
	CY_ASSERT(taskReturn == pdPASS);

	// Start scheduler for FreeRTOS
	vTaskStartScheduler();

	/* Should never get here! */
	/* Halt the CPU if scheduler exits */
	CY_ASSERT(0);
//	while(1);

}

/* [] END OF FILE */

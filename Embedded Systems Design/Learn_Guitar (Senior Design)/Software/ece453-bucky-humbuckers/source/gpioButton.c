/*
 * gpioButton.c
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "gpioButton.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Static Variable Definitions
 ******************************************************************************/
static cyhal_gpio_callback_data_t buttonCallbackData = {
    .callback     = buttonInterruptHandler,
    .callback_arg = NULL
};
/*******************************************************************************
 * Global Variable Definitions
 ******************************************************************************/

/*******************************************************************************
 * Static Function Definitions
 ******************************************************************************/

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/
/**
 * @brief FreeRTOS task that governs the GPIO button input. Waits asynchronously
 * 		  for the button pressed notification
 * @brief pvParameters [in] This is task 1 so the value should be 1
 */
void vButtonTask(void *pvParameters) {
	configASSERT(((uint32_t ) pvParameters) == 1);

	cy_rslt_t result = CY_RSLT_SUCCESS;

	result = cyhal_gpio_init(LED_PIN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_gpio_init(BUTTON_ONE_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

    cyhal_gpio_register_callback(BUTTON_ONE_PIN, &buttonCallbackData);

    cyhal_gpio_enable_event(BUTTON_ONE_PIN, CYHAL_GPIO_IRQ_FALL | CYHAL_GPIO_IRQ_RISE, 2, true);

	while (1) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		cyhal_gpio_toggle(LED_PIN);
	}
}

/**
 * @brief Handles the GPIO button interrupt
 * @param handler_arg Not used
 * @param event	The event/interrupt that is being signaled
 */
void buttonInterruptHandler(void* handler_arg, cyhal_gpio_event_t event){
	if((event & CYHAL_GPIO_IRQ_RISE) == CYHAL_GPIO_IRQ_RISE){

	}
	// Active low buttons
	if((event & CYHAL_GPIO_IRQ_FALL) == CYHAL_GPIO_IRQ_FALL){
    	xTaskNotifyFromISR(buttonTaskHandle, 0, eNoAction, NULL);
	}
}

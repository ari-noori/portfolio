/*
 * touchscreen.c
 *
 *  Created on: Nov 7, 2022
 *      Author: tanow
 */
#include "touchscreen.h"

static cyhal_i2c_t touchscreenDevice;

static cyhal_gpio_callback_data_t touchScreenCallbackData = {
		.callback = touchScreenInterruptHandler,
		.callback_arg = NULL
};

static uint16_t point[2] = {0};

static void touchscreenEventHandler(void *callback_arg, cyhal_i2c_event_t event) {
	// To remove unused variable warning
	(void) callback_arg;

	// Check write complete event
	if (0UL != (CYHAL_I2C_MASTER_WR_CMPLT_EVENT & event)) {
		// Perform the required functions
	}
	// Check read complete event
	if (0UL != (CYHAL_I2C_MASTER_RD_CMPLT_EVENT & event)) {
		// Perform the required functions
	}
	if (0UL != (CYHAL_I2C_MASTER_ERR_EVENT & event)) {
		// In case of error abort transfer
		cyhal_i2c_abort_async(&touchscreenDevice);
	}
}

static void touchscreenSetRefreshRate(void){
	cy_rslt_t result = CY_RSLT_SUCCESS;

	// Update the Active refresh rate
	// Write operation to send active refresh rate
	uint8_t txBuffer[] = {TOUCHSCREEN_PERIODACTIVE_REG,
			TOUCHSCREEN_REFRESH_RATE };
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 2, 1000, false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Confirm the write operation result
	uint8_t rxBuffer[2] = { 0 };
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, rxBuffer, 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);
	CY_ASSERT(rxBuffer[0] = TOUCHSCREEN_REFRESH_RATE);

	// Update the monitor refresh rate
	// Write operation to send monitor refresh rate
	txBuffer[0] = TOUCHSCREEN_PERIODMONITOR_REG;
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 2, 1000, false);

	// Confirm the write operation result
	memset(rxBuffer, 0, 2);
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, rxBuffer, 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);
	CY_ASSERT(rxBuffer[0] == TOUCHSCREEN_REFRESH_RATE);
}

static void touchscreenSetActiveMode(void){
	cy_rslt_t result = CY_RSLT_SUCCESS;

	// Update the control register to always stay active
	uint8_t txBuffer[] = {TOUCHSCREEN_CTRL_REG, 0x00};
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 2, 1000, false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Confirm the write operation result
	uint8_t rxBuffer[1] = {0};
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, rxBuffer, 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);
	CY_ASSERT(rxBuffer[0] == 0x00);
}

static void touchscreenSetThreshold(void){
	cy_rslt_t result = CY_RSLT_SUCCESS;

	// Update the touch register to set touch threshold
	uint8_t txBuffer[] = {TOUCHSCREEN_TH_GROUP_REG, 0x04};
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 2, 1000, false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Confirm the write operation result
	uint8_t rxBuffer[1] = {0};
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, rxBuffer, 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);
	CY_ASSERT(rxBuffer[0] == 0x04);
}

void touchscreenInit(void){
	cy_rslt_t result = CY_RSLT_SUCCESS;

		// Initialize the interrupt pin
		result = cyhal_gpio_init(TOUCHSCREEN_INTERRUPT_PIN, CYHAL_GPIO_DIR_INPUT,
				CYHAL_GPIO_DRIVE_NONE, false);
		CY_ASSERT(result == CY_RSLT_SUCCESS);

		cyhal_gpio_register_callback(TOUCHSCREEN_INTERRUPT_PIN,
				&touchScreenCallbackData);

		cyhal_gpio_enable_event(TOUCHSCREEN_INTERRUPT_PIN,
				CYHAL_GPIO_IRQ_FALL | CYHAL_GPIO_IRQ_RISE, 2, true);

		// Initialize the I2C interface
		cyhal_i2c_cfg_t touchscreenConfiguration = { .is_slave =
				CYHAL_I2C_MODE_MASTER, .address = 0, .frequencyhal_hz =
						TOUCHSCREEN_FREQUENCY };

		result = cyhal_i2c_init(&touchscreenDevice, TOUCHSCREEN_SDA_PIN,
				TOUCHSCREEN_SCLK_PIN, NULL);
		CY_ASSERT(result == CY_RSLT_SUCCESS);

		result = cyhal_i2c_configure(&touchscreenDevice, &touchscreenConfiguration);
		CY_ASSERT(result == CY_RSLT_SUCCESS);

		cyhal_i2c_register_callback(&touchscreenDevice, touchscreenEventHandler,
				NULL);

		cyhal_i2c_enable_event(&touchscreenDevice,
				CYHAL_I2C_MASTER_WR_CMPLT_EVENT | CYHAL_I2C_MASTER_RD_CMPLT_EVENT
				| CYHAL_I2C_MASTER_ERR_EVENT,
				TOUCHSCREEN_INTERRUPT_PRIORITY, true);

		touchscreenSetRefreshRate();
		touchscreenSetActiveMode();
		touchscreenSetThreshold();
}

void touchScreenInterruptHandler(void *handler_arg, cyhal_gpio_event_t event) {
	if ((event & CYHAL_GPIO_IRQ_RISE) == CYHAL_GPIO_IRQ_RISE) {

	}
	// Active low interrupt
	if((event & CYHAL_GPIO_IRQ_FALL) == CYHAL_GPIO_IRQ_FALL){
		xTaskNotifyFromISR(lcdTaskHandle, 0, eNoAction, NULL);
	}
}

uint16* touchscreenGetTouchPoint(void){
	cy_rslt_t result = CY_RSLT_SUCCESS;

	uint8_t rxBuffer[4] = {0};
	// Get the upper byte of the point x
	uint8_t txBuffer[1] = {TOUCHSCREEN_P1_XH_REG};
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	// TODO - this assert seems to occasionally fail, but
	// board behaves as expected... Potential race condition?
	 CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, &rxBuffer[0], 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Get the lower byte of the point x
	txBuffer[0] = TOUCHSCREEN_P1_XL_REG;
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, &rxBuffer[1], 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Get the upper byte of the point y
	txBuffer[0] = TOUCHSCREEN_P1_YH_REG;
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, &rxBuffer[2], 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Get the lower byte of the point y
	txBuffer[0] = TOUCHSCREEN_P1_YL_REG;
	result = cyhal_i2c_master_write(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, txBuffer, 1, 1000,
			false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	result = cyhal_i2c_master_read(&touchscreenDevice,
			TOUCHSCREEN_SLAVE_ADDRESS, &rxBuffer[3], 1, 1000,
			true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	point[0] = (rxBuffer[0] << 8) | rxBuffer[1];
	point[1] = (rxBuffer[2] << 8) | rxBuffer[3];
	return point;
}

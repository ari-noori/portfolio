/*
 * cli.c
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cli.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * CLI Callback Function Declarations
 ******************************************************************************/
static void cliHelp(uint8_t nargs, uint8_t** args);
static void cliRerun(uint8_t nargs, uint8_t** args);
static void cliAdc(uint8_t nargs, uint8_t** args);
static void cliSignalProcessing(uint8_t nargs, uint8_t** args);
static void cliClear(uint8_t nargs, uint8_t** args);
static void cliSd(uint8_t nargs, uint8_t** args);
static void cliTestHertz(uint8_t nargs, uint8_t** args);

/*******************************************************************************
 * Static Variable Definitions
 ******************************************************************************/
static uint8_t printfBuf[MAX_UART_PRINTF_LEN];
static uint8_t lastCommandBuffer[CLI_BUFFER_LENGTH];
static uint8_t cliBuffer[CLI_BUFFER_LENGTH];
static uint8_t uartBuffer[1];
static uint8_t numberOfCharacters;
static cyhal_uart_t uartDevice;

static const cliCommand consoleCommands[] = {
		{"help",	"Displays list of available commands", 					cliHelp},
		{"/", 		"Reruns previous valid command", 						cliRerun},
		{"adc", 	"Controls whether the ADC values are printed to CLI",	cliAdc},
		{"signal",	"Controls whether the FFT values are printed to CLI", 	cliSignalProcessing},
		{"clear",	"Clears the screen", 									cliClear},
		{"sd",		"Reads or writes to the SD card",						cliSd},
		{"hertz", 	"Tests if a given hertz value is high", 				cliTestHertz},
};
/*******************************************************************************
 * Global Variable Definitions
 ******************************************************************************/

/*******************************************************************************
 * Static Function Definitions
 ******************************************************************************/
/**
 * @brief Handles the UART interrupt events. In the event of
 * 		  a RX the notification is sent to the while loop in
 * 		  vCommandLineTask
 * @param handler_arg Not used
 * @param event	The event/interrupt that is being signaled
 */
static void cliInterruptHandler(void* handler_arg, cyhal_uart_event_t event){
	(void)handler_arg;

	if ((event & CYHAL_UART_IRQ_TX_ERROR) == CYHAL_UART_IRQ_TX_ERROR){
		// An error occurred in Tx
	}
	else if ((event & CYHAL_UART_IRQ_TX_DONE) == CYHAL_UART_IRQ_TX_DONE){
		// All Tx data has been transmitted
	}
	else if ((event & CYHAL_UART_IRQ_RX_DONE) == CYHAL_UART_IRQ_RX_DONE){
		// All Rx data has been received
		xTaskNotifyFromISR(cliTaskHandle, 5, eNoAction, NULL);
	}
}


/**
 * @brief Initializes the CLI UART and configures it to 8n1.
 *  	  Also turns on interrupts for the TX/RX done events
 */
void commandLineInit(void){
	cy_rslt_t result;

	const cyhal_uart_cfg_t uartConfig = {
			.data_bits      = DATA_BITS_8,
			.stop_bits      = STOP_BITS_1,
			.parity         = CYHAL_UART_PARITY_NONE,
			.rx_buffer      = uartBuffer,
			.rx_buffer_size = 1
	};

	// Set up the UART with correct pins, RTS/CTS are not used
	result = cyhal_uart_init(&uartDevice, CLI_UART_TX, CLI_UART_RX, NC, NC, NULL, &uartConfig);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Set the baud rate
	uint32_t actualBaud = 0;
	result = cyhal_uart_set_baud(&uartDevice, CLI_BAUD_RATE, &actualBaud);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Register the interrupt callback
	cyhal_uart_register_callback(&uartDevice, cliInterruptHandler, NULL);

	// Enable the TX/RX Done interrupts and TX Error interrupt
	cyhal_uart_enable_event(&uartDevice, (cyhal_uart_event_t)
			(CYHAL_UART_IRQ_TX_DONE | CYHAL_UART_IRQ_TX_ERROR | CYHAL_UART_IRQ_RX_DONE),
			CLI_INT_PRIORITY, true);

	// Reset all buffers
	memset(printfBuf, '\0', MAX_UART_PRINTF_LEN);
	memset(lastCommandBuffer, '\0', CLI_BUFFER_LENGTH);
	memset(cliBuffer, '\0', CLI_BUFFER_LENGTH);
	memset(uartBuffer, '\0', 1);
	// Restart the character counter
	numberOfCharacters = 0;

	uint8_t escapeSequence[] = {0x1b, '[', '2', 'J', 0x1b, '[', 'H'};
	size_t escapeSequenceSize = 7;
	cyhal_uart_write(&uartDevice, escapeSequence, &escapeSequenceSize);

	cliPrintln((uint8_t*)"\r========================================");
	cliPrintln((uint8_t*)"======      Bucky Humbuckers      ======");
	cliPrintln((uint8_t*)"========================================");

	(void)result;
}

/**
 * @brief Separates the command buffer into an argument array and a number of
 * 		  of arguments.
 * @param commandBuffer [in] Buffer that contains the command to be parsed
 */
static void cliHandleCommand(uint8_t commandBuffer[]){
	uint8_t nargs = 0;
	uint8_t* ptr;
	uint8_t* args[ARGUMENT_MAXIMUM];

	// Parse arguments (space delimited)
	args[nargs++] = commandBuffer;
	ptr           = (uint8_t*)strpbrk((char*)commandBuffer, " ");
	while (ptr != NULL) {
		args[nargs] = ptr + 1;
		*ptr        = '\0';
		ptr         = (uint8_t*)strpbrk((char*)args[nargs], " ");
		nargs++;
		// don't go out of bounds
		if(nargs == ARGUMENT_MAXIMUM){
			break;
		}
	}
	int8_t found = -1;
	// find command
	for(uint8_t i = 0; i < sizeof(consoleCommands)/sizeof(cliCommand); i++){
		if (0 == strcmp((char*)consoleCommands[i].name, (char*)args[0])) {
			consoleCommands[i].callback(nargs, args);
			found = i+1;
			break;
		}
	}
	if(found == -1){
		cliPrint((uint8_t*)"'%s' is not a command.\r\nTo get available commands type 'help'\r\n", args[0]);
		return;
	}
	else{
		if(found != 2){
			strncpy((char*)lastCommandBuffer, (char*)commandBuffer, CLI_BUFFER_LENGTH);
			uint8_t insertedSpaces = 0;
			for(uint16_t j = 0; j < CLI_BUFFER_LENGTH; j ++){
				if(commandBuffer[j] == '\0'){
					if(insertedSpaces < (nargs - 1)){
						lastCommandBuffer[j] = ' ';
						insertedSpaces++;
					}
				}
			}
		}
	}

	return;
}

/**
 * @brief Takes the byte that was received over the UART and processes
 * 		  the byte into the command line buffer
 */
static void cliProcessByte(void){
	// newline or return character
	if((uartBuffer[0] == '\r') || (uartBuffer[0] == '\n')){
		cliBuffer[numberOfCharacters] = '\0';
		cliPrint((uint8_t*)"\r\n");
		if(strlen((char*)cliBuffer) > 0)
			cliHandleCommand(cliBuffer);
		memset(cliBuffer, '\0', CLI_BUFFER_LENGTH);
		numberOfCharacters = 0;
		cliPrint((uint8_t*)"$> ");
	}
	// backspace character
	else if((uartBuffer[0] == 0x08 || uartBuffer[0] == 0x7f) && (numberOfCharacters > 0)){
		numberOfCharacters--;
		memset(&cliBuffer[numberOfCharacters], '\0', 1);
		cliPrint(uartBuffer);
	}
	// invalid character or maxed out on characters cliBuffer[0xfe] reserved for '\0'
	else if((uartBuffer[0] > 0x7f) || (uartBuffer[0] < 0x20) || (numberOfCharacters == 0xff)){}
	else if(((numberOfCharacters+1) > 0) && (numberOfCharacters < CLI_BUFFER_LENGTH) && (uartBuffer[0] != 0x7f)){
		cliBuffer[numberOfCharacters] = uartBuffer[0];
		numberOfCharacters++;
		cliPrint(uartBuffer);
	}
}

/**
 * @brief Handles the 'help' command from the CLI
 */
static void cliHelp(uint8_t nargs, uint8_t** args){
	if(nargs == 1){
		for(uint8_t i = 0; i < (sizeof(consoleCommands)/sizeof(cliCommand)); i++){
			cliPrintln((uint8_t*)"\t%-9s- %-21s", consoleCommands[i].name, consoleCommands[i].description);
			vTaskDelay(1);
		}
	}
	else{
		cliPrintln((uint8_t*)"Invalid number of arguments. Given %i, Expected 1", nargs);
	}
	return;
}

/**
 * @brief Handles the '/' command from the CLI
 */
static void cliRerun(uint8_t nargs, uint8_t** args){
	if(nargs != 1){
		cliPrintln((uint8_t*)"Invalid number of arguments. Given %i, Expected 1", nargs);
		return;
	}
	else{
		cliHandleCommand(lastCommandBuffer);
	}
}

/**
 * @brief Handles the 'adc' command from the CLI
 */
static void cliAdc(uint8_t nargs, uint8_t** args){
	if(nargs != 2){
		cliPrintln((uint8_t*)"Invalid number of arguments. Given %i, Expected 1", nargs);
		return;
	}
	else{
		if(strcmp((char*)args[1], "start") == 0){
			adcPrint = true;
		}
		else if(strcmp((char*)args[1], "stop") == 0){
			adcPrint = false;
		}
	}
}

/**
 * @brief Handles the 'adc' command from the CLI
 */
static void cliSignalProcessing(uint8_t nargs, uint8_t** args){
	if(nargs != 2){
		cliPrintln((uint8_t*)"Invalid number of arguments. Given %i, Expected 1", nargs);
		return;
	}
	else{
		if(strcmp((char*)args[1], "start") == 0){
			signalProcessingPrint = true;
		}
		else if(strcmp((char*)args[1], "stop") == 0){
			signalProcessingPrint = false;
		}
	}
}

static void cliClear(uint8_t nargs, uint8_t** args){
	if(nargs != 1){
		cliPrintln((uint8_t*)"Invalid number of arguments. Given %i, Expected 1", nargs);
		return;
	}
	else{
		uint8_t escapeSequence[] = {0x1b, '[', '2', 'J', 0x1b, '[', 'H'};
		size_t escapeSequenceSize = 7;

		xSemaphoreTake(cliSemaphore, portMAX_DELAY);
		cyhal_uart_write(&uartDevice, escapeSequence, &escapeSequenceSize);
	    xSemaphoreGive(cliSemaphore);

		return;
	}
}

static void cliSd(uint8_t nargs, uint8_t** args){
	if(nargs != 2){
		cliPrintln((uint8_t*)"Invalid number of arguments. Given %i, Expected 2", nargs);
		return;
	}
	uint8_t* filename = args[1];

	xSemaphoreTake(spiSemaphore, portMAX_DELAY);
	FRESULT fr = FR_OK;

	cyhal_gpio_write(SD_NCS_PIN, 0);
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	CY_ASSERT(fr == FR_OK);
	cyhal_gpio_write(SD_NCS_PIN, 1);

	cyhal_gpio_write(SD_NCS_PIN, 0);
	fr = f_mkfs("", 0, sdBuffer, sizeof(sdBuffer));
//	CY_ASSERT(fr == FR_OK);
	cyhal_gpio_write(SD_NCS_PIN, 1);
	cyhal_gpio_write(SD_NCS_PIN, 0);
	fr = f_open(&Fil, (const TCHAR*)filename, FA_WRITE | FA_OPEN_ALWAYS);	/* Open a file */
	cyhal_gpio_write(SD_NCS_PIN, 1);
	if (fr == FR_OK) {
		cyhal_gpio_write(SD_NCS_PIN, 0);
		UINT bytesWritten = 0;
		// Start reading from the start
		fr = f_lseek(&Fil, 0);
		CY_ASSERT(fr == FR_OK);
		// Write the data from the file
		fr = f_write(&Fil, sdBuffer, BACKGROUND_IMAGE_LENGTH, &bytesWritten);
		CY_ASSERT(fr == FR_OK);
		cyhal_gpio_write(SD_NCS_PIN, 1);
		cyhal_gpio_write(SD_NCS_PIN, 0);
		fr = f_close(&Fil);							/* Close the file */
		CY_ASSERT(fr == FR_OK);
		cyhal_gpio_write(SD_NCS_PIN, 1);
	}
	xSemaphoreGive(spiSemaphore);

}

static void cliTestHertz(uint8_t nargs, uint8_t** args){
	if(nargs != 2){
		cliPrintln((uint8_t*)"Invalid number of arguments. Given %i, Expected 2", nargs);
		return;
	}
//	uint8_t upperByte = strtoul((char*)args[1], NULL, 16);
//	uint8_t lowerByte = strtoul((char*)args[2], NULL, 16);
	uint16_t bytes = strtoul((char*)args[1], NULL, 10);
	bool test = getNoteStatus(bytes);
	cliPrintln((uint8_t*)"0x%02x%02x %i Hertz: %i",bytes >> 8, bytes & 0xff, bytes, test);
}

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/
/**
 * @brief FreeRTOS task that governs the CLI. Waits asynchronously
 * 		  for the RX interrupt notification and then processes the byte
 * @brief pvParameters [in] This is task 0 so the value should be 0
 */
void vCommandLineTask(void *pvParameters) {
	configASSERT(((uint32_t ) pvParameters) == 0);
	vTaskDelay(10);
	cliPrint((uint8_t*)"$> ");
	while (1) {
		// Wait forever for a byte
		cyhal_uart_read_async(&uartDevice, uartBuffer, 1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		cliProcessByte();
	}
}

/**
 * @brief Prints out the given buffer to the CLI UART TX line
 * @param _format [in] The string format the data is placed in
 */
void cliPrint(const uint8_t *_format, ...){
	xSemaphoreTake(cliSemaphore, portMAX_DELAY);
	va_list _valist;

	va_start(_valist, _format);
	vsnprintf((char*)printfBuf, MAX_UART_PRINTF_LEN,(char*) _format, _valist);
	va_end(_valist);

	// Write buffer to UART
	uint8_t* ch = printfBuf;
	while(*ch != '\0') {
		cyhal_uart_putc(&uartDevice, *ch);
		ch++;
	}
    xSemaphoreGive(cliSemaphore);
}

/**
 * @brief Prints out the given buffer to the CLI UART TX line with a
 * 		  newline character printed afterwards
 * @param _format [in] The string format the data is placed in
 */
void cliPrintln(const uint8_t *_format, ...){
	xSemaphoreTake(cliSemaphore, portMAX_DELAY);
	va_list _valist;

	va_start(_valist, _format);
	vsnprintf((char*)printfBuf, MAX_UART_PRINTF_LEN,(char*) _format, _valist);
	va_end(_valist);

	// Write buffer to UART
	uint8_t* ch = printfBuf;
	while(*ch != '\0') {
		cyhal_uart_putc(&uartDevice, *ch);
		ch++;
	}
	uint8_t newline[] = "\r\n";
	size_t newlineSize = strlen((char*)newline);
	cyhal_uart_write(&uartDevice, newline, &newlineSize);
    xSemaphoreGive(cliSemaphore);
}

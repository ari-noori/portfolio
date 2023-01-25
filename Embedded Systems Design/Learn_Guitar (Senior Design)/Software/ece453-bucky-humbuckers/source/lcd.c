/*
 * lcd.c
 *
 *  Created on: Sep 30, 2022
 *      Author: Thomas Nowak
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lcd.h"
#include "needle.h"
#include "songs.h"
#include "images.h"
#include <stdbool.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define SONG bmv578

/*******************************************************************************
 * Static Variable Definitions
 ******************************************************************************/
static bool noop = false;
static note_t * current_note = SONG;
static uint8_t tune_string = 0;
static bool nightMode = false;

cyhal_spi_t spiDevice;

// Ordering is important as it bijects to screen_t enum
static const char * screens_sd[] = {
		"ascii.start.hex",
		"ascii.modes.hex",
		"ascii.learn.hex",
		"ascii.rock.hex",
		"ascii.tune.hex",
		"ascii.about.hex",
		"ascii.colortest.hex"
};

/*******************************************************************************
 * Global Variable Definitions
 ******************************************************************************/

screen_t currentScreen = START;
screen_t nextScreen = START;
cyhal_spi_t lcdSpiDevice;

/*******************************************************************************
 * Static Function Definitions
 ******************************************************************************/
static void writeCommand(uint8_t command) {
	xSemaphoreTake(spiSemaphore, portMAX_DELAY);
	cy_rslt_t result;
	// set the WR to be in command mode (low)
	cyhal_gpio_write(LCD_WR_PIN, false);

	// Instantiate buffers
	size_t txAmount = 1;
	uint8_t rxBuffer[1];
	size_t rxAmount = 1;

	cyhal_gpio_write(LCD_NCS_PIN, 0);

	result = cyhal_spi_transfer(&spiDevice, &command, txAmount, rxBuffer,
			rxAmount, 0x00);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	cyhal_gpio_write(LCD_NCS_PIN, 1);

	xSemaphoreGive(spiSemaphore);
}

static void writeData(uint8_t dataHigh, uint8_t dataLow) {
	xSemaphoreTake(spiSemaphore, portMAX_DELAY);
	cy_rslt_t result;
	// set the WR to be in data mode (high)
	cyhal_gpio_write(LCD_WR_PIN, true);

	// Instantiate buffers
	uint8_t txBuffer[2] = { dataHigh, dataLow };
	size_t txAmount = 2;
	uint8_t rxBuffer[2];
	size_t rxAmount = 2;

	cyhal_gpio_write(LCD_NCS_PIN, 0);

	result = cyhal_spi_transfer(&spiDevice, txBuffer, txAmount, rxBuffer,
			rxAmount, 0x00);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	cyhal_gpio_write(LCD_NCS_PIN, 1);

	xSemaphoreGive(spiSemaphore);
}

static void writeDataU16(uint16_t command) {
	uint8_t upperByte = command >> 8;
	uint8_t lowerByte = command & 0xff;
	writeData(upperByte, lowerByte);
}

static void writeCommandData(uint8_t command) {
	xSemaphoreTake(spiSemaphore, portMAX_DELAY);
	cy_rslt_t result;
	// set the WR to be in data mode (high)
	cyhal_gpio_write(LCD_WR_PIN, true);

	// Instantiate buffers
	size_t txAmount = 1;
	uint8_t rxBuffer[1];
	size_t rxAmount = 1;

	cyhal_gpio_write(LCD_NCS_PIN, 0);

	result = cyhal_spi_transfer(&spiDevice, &command, txAmount, rxBuffer,
			rxAmount, 0x00);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	cyhal_gpio_write(LCD_NCS_PIN, 1);

	xSemaphoreGive(spiSemaphore);
}

static void lcdInit(void) {
	writeCommand(0x11);
	vTaskDelay(120.0 / configTICK_RATE_HZ);

	writeCommand(0xCF);
	writeCommandData(0x00);
	writeCommandData(0xc3);
	writeCommandData(0X30);

	writeCommand(0xED);
	writeCommandData(0x64);
	writeCommandData(0x03);
	writeCommandData(0X12);
	writeCommandData(0X81);

	writeCommand(0xE8);
	writeCommandData(0x85);
	writeCommandData(0x10);
	writeCommandData(0x79);

	writeCommand(0xCB);
	writeCommandData(0x39);
	writeCommandData(0x2C);
	writeCommandData(0x00);
	writeCommandData(0x34);
	writeCommandData(0x02);

	writeCommand(0xF7);
	writeCommandData(0x20);

	writeCommand(0xEA);
	writeCommandData(0x00);
	writeCommandData(0x00);

	writeCommand(0xC0);	  // Power control
	writeCommandData(0x22); // VRH[5:0]

	writeCommand(0xC1);	  // Power control
	writeCommandData(0x11); // SAP[2:0];BT[3:0]

	writeCommand(0xC5); // VCM control
	writeCommandData(0x3d);
	// LCD_DataWrite_ILI9341(0x30);
	writeCommandData(0x20);

	writeCommand(0xC7); // VCM control2
	// LCD_DataWrite_ILI9341(0xBD);
	writeCommandData(0xAA); // 0xB0

	writeCommand(0x36); // Memory Access Control
	writeCommandData(0x08);

	writeCommand(0x3A);
	writeCommandData(0x55);

	writeCommand(0xB1);
	writeCommandData(0x00);
	writeCommandData(0x13);

	writeCommand(0xB6); // Display Function Control
	writeCommandData(0x0A);
	writeCommandData(0xA2);

	writeCommand(0xF6);
	writeCommandData(0x01);
	writeCommandData(0x30);

	writeCommand(0xF2); // 3Gamma Function Disable
	writeCommandData(0x00);

	writeCommand(0x26); // Gamma curve selected
	writeCommandData(0x01);

	writeCommand(0xE0); // Set Gamma
	writeCommandData(0x0F);
	writeCommandData(0x3F);
	writeCommandData(0x2F);
	writeCommandData(0x0C);
	writeCommandData(0x10);
	writeCommandData(0x0A);
	writeCommandData(0x53);
	writeCommandData(0XD5);
	writeCommandData(0x40);
	writeCommandData(0x0A);
	writeCommandData(0x13);
	writeCommandData(0x03);
	writeCommandData(0x08);
	writeCommandData(0x03);
	writeCommandData(0x00);

	writeCommand(0XE1); // Set Gamma
	writeCommandData(0x00);
	writeCommandData(0x00);
	writeCommandData(0x10);
	writeCommandData(0x03);
	writeCommandData(0x0F);
	writeCommandData(0x05);
	writeCommandData(0x2C);
	writeCommandData(0xA2);
	writeCommandData(0x3F);
	writeCommandData(0x05);
	writeCommandData(0x0E);
	writeCommandData(0x0C);
	writeCommandData(0x37);
	writeCommandData(0x3C);
	writeCommandData(0x0F);

	writeCommand(0x11); // Exit Sleep
	vTaskDelay(120.0 / configTICK_RATE_HZ);
	writeCommand(0x29); // Display on
	vTaskDelay(50.0 / configTICK_RATE_HZ);
}

static void lcd_update_background() {
	const uint16_t * image = (uint16_t*)sdBuffer;
	lcdSetPosition(0, BACKGROUND_WIDTH - 1, 0, BACKGROUND_HEIGHT - 1); // 320x240
	for (uint32_t i = 0; i < BACKGROUND_WIDTH * BACKGROUND_HEIGHT; ++i)
		writeDataU16(nightMode ? ~image[i] : image[i]);
}

static void lcd_draw_note(const note_label_t note) {
	const int16_t ydiff = -(BACKGROUND_HEIGHT - NOTE_HEIGHT)/3;
	const int16_t xdiff = -(BACKGROUND_WIDTH - NOTE_WIDTH) /1.43;
	lcdSetPosition(0 - ydiff, NOTE_WIDTH - 1 - ydiff , 0 - xdiff, NOTE_HEIGHT - 1 - xdiff); // 320x240
	const uint16_t * image = notes[note];
	for (uint32_t i = 0; i < NOTE_WIDTH * NOTE_HEIGHT; ++i)
		writeDataU16(nightMode ? ~image[i] : image[i]);
}

static void lcd_draw_tuning_note(const uint16_t * image){
	const int16_t ydiff = -10;
	const int16_t xdiff = -BACKGROUND_WIDTH - 7;
	lcdSetPosition(0 - ydiff, TUNING_NOTE_WIDTH - 1 - ydiff , 0 - xdiff, TUNING_NOTE_HEIGHT - 1 - xdiff); // 320x240

	for (uint32_t i = 0; i < TUNING_NOTE_WIDTH * TUNING_NOTE_HEIGHT; ++i)
		writeDataU16(nightMode ? ~image[i] : image[i]);
}

static void erase_needle(uint16_t erase_position){
	lcdSetPosition(TUNING_NEEDLE_BOTTOM - TUNING_NEEDLE_HEIGHT, TUNING_NEEDLE_BOTTOM, erase_position, erase_position + TUNING_NEEDLE_WIDTH); // 320x240
	for (uint32_t i = 0; i < TUNING_NEEDLE_WIDTH * TUNING_NEEDLE_HEIGHT + 100; ++i)
		writeDataU16(nightMode ? BLACK : WHITE);
}

bool lastErased = false;
static void erase_needles(){
	if(lastErased) return;

	erase_needle(TUNING_NEEDLE_LEFT);
	erase_needle(TUNING_NEEDLE_CENTER);
	erase_needle(TUNING_NEEDLE_RIGHT);

	lastErased = true;
}


uint8_t note_on_screen  = -1;
static inline void draw_tuning_note(){
	if(note_on_screen != tune_string)
		lcd_draw_tuning_note(tuningImages[tune_string]);
	note_on_screen = tune_string;
}

uint32_t silent_count = 1;
static void lcd_draw_needle_at(uint16_t x, uint16_t color){
	silent_count = 1;
	lastErased = false;
	lcdSetPosition(TUNING_NEEDLE_BOTTOM - TUNING_NEEDLE_HEIGHT, TUNING_NEEDLE_BOTTOM, x, x + TUNING_NEEDLE_WIDTH); // 320x240
	for (uint32_t i = 0; i < TUNING_NEEDLE_WIDTH * TUNING_NEEDLE_HEIGHT + 100; ++i)
		writeDataU16(color);
}

void lcdSpiInit(void){
	cy_rslt_t result;

	memset(&spiDevice, 0, sizeof(spiDevice));

	// Initialize the Command/Data WR GPIO
	result = cyhal_gpio_init(LCD_WR_PIN, CYHAL_GPIO_DIR_OUTPUT,
			CYHAL_GPIO_DRIVE_STRONG, true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Initialize the LCD nReset pin
	result = cyhal_gpio_init(LCD_RESET_PIN, CYHAL_GPIO_DIR_OUTPUT,
			CYHAL_GPIO_DRIVE_STRONG, true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Initialize the LCD nCS pin
	result = cyhal_gpio_init(LCD_NCS_PIN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Initialize the SPI Interface
	result = cyhal_spi_init(&spiDevice, LCD_MOSI_PIN, LCD_MISO_PIN, LCD_SCLK_PIN,
			NC, NULL, LCD_BITS_PER_FRAME, CYHAL_SPI_MODE_00_MSB, false);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	// Set the SPI Frequency
	result = cyhal_spi_set_frequency(&spiDevice, LCD_SPI_FREQUENCY);
	CY_ASSERT(result == CY_RSLT_SUCCESS);

	lcdInit();
}

static void lcd_draw_from_sd(uint8_t data[]){
	lcdSetPosition(0, BACKGROUND_WIDTH - 1, 0, BACKGROUND_HEIGHT - 1); // 320x240
	for (uint32_t i = 0; i < BACKGROUND_WIDTH * BACKGROUND_HEIGHT * 2; i+=2)
		writeDataU16(nightMode ? ~((data[i+1] << 8) | (data[i])) : (data[i+1] << 8) | (data[i]));
//	lcdSetPosition(0, 1, 0, 1);
}

static inline void lcd_enable_back_button_to(screen_t screen, uint16_t x, uint16_t y){
	// Top right
	if(y < LCD_BACK_BUTTON_SIZE && x < LCD_BACK_BUTTON_SIZE)
		nextScreen = screen;
}

// Trust me.
static inline void lcd_hide_white_sweeping_transition(){
	lcdSetPosition(0, 0, 0, 0);
	writeDataU16(0);
}

uint16_t prev_x = -1;
uint16_t prev_y = -1;
static void lcd_analyze_point(const uint16_t point[2]){
	const uint16_t x = point[0] & LCD_POINT_BITMASK;
	const uint16_t y = point[1] & LCD_POINT_BITMASK;
	if(x == prev_x && y == prev_y)
		return;
	prev_x = x;
	prev_y = y;
	screen_t backScreen = MODES;
	// uncomment if we use ROCK mode
//	nightMode = false;
	switch(currentScreen){
		case COLORTEST:
			nextScreen = START;
			break;
		case START:
			nextScreen = MODES;
			break;
		case MODES:
			if(x > (MODE_X0 - DELTA_X) && x < (MODE_X0 + DELTA_X)){
				  if(y > (MODE_Y0 - DELTA_Y) && y < (MODE_Y0 + DELTA_Y)){
					  nightMode = !nightMode;
					  noop = false;
					  return;
					  // uncomment if we use ROCK mode
//					  nightMode = true;
//					  nextScreen = ROCK;
				  }
				  else if(y > (MODE_Y1 - DELTA_Y) && y < (MODE_Y1 + DELTA_Y)){
					  nextScreen = LEARN;
					  current_note = SONG;
				  }
			}
			else if(x > (MODE_X1 - DELTA_X) && x < (MODE_X1 + DELTA_X)){
				  if(y > (MODE_Y0 - DELTA_Y) && y < (MODE_Y0 + DELTA_Y))
					  nextScreen = ABOUT;
				  else if(y > (MODE_Y1 - DELTA_Y) && y < (MODE_Y1 + DELTA_Y)){
					  nextScreen = TUNE;
					  tune_string = 0;
					  note_on_screen = -1;
				  }
			}
			backScreen = START;
			break;
		case LEARN:
			break;
		case ROCK:
			break;
		case TUNE:
			tune_string = ++tune_string == 6 ? 0 : tune_string;
			erase_needles();
			draw_tuning_note();

			// Debounce
			vTaskDelay(100);
			break;
		case ABOUT:
			break;
		default:
			cliPrintln((uint8_t*)"Unimplemented Screen State 0x%x", currentScreen);
	}
	lcd_enable_back_button_to(backScreen, x, y);
	noop = nextScreen == currentScreen;
}

static void lcd_draw_background_from_sd(){
	if (noop) return;

	cliPrintln("Drawing from sd");
	sdLoadImage((uint8_t*)screens_sd[currentScreen], sdBuffer, BACKGROUND_IMAGE_LENGTH);
	lcd_draw_from_sd(sdBuffer);
}

static void lcd_draw_foreground_from_flash(){
	const tuning_frequency_t * note = &tuning[tune_string];
	switch(currentScreen){
		case LEARN:
			while (current_note->label == rest && current_note->label != END_OF_SONG)
				current_note++;

			if(current_note->label == END_OF_SONG)
				return;

			lcd_draw_note(current_note->label);
			cliPrintln((uint8_t*)"Expecting note %d to be played: %f", current_note->label, current_note->frequency);
			while (!getNoteStatus((uint16_t)current_note->frequency)){
				cliPrintln("Next");
				ulTaskNotifyTake(pdTRUE, 10);
				uint16_t* point = touchscreenGetTouchPoint();
				lcd_analyze_point(point);
				if (currentScreen != nextScreen) break;
			} // Spin

			// TODO Right here - flash GPIO LED
			current_note++;
			break;
		case START:
			break;
		case MODES:
			break;
		case ROCK:
			break;
		case TUNE:
			draw_tuning_note();
			const bool lo = getNoteStatus((uint16_t) (note->prev_note)); // + note->target_note)/2);
			const bool md = getNoteStatus((uint16_t) (note->target_note));
			const bool hi = getNoteStatus((uint16_t) (note->next_note));

			if(lo + md + hi >= 2) erase_needles();

			if (lo && md && hi){
				lcd_draw_needle_at(TUNING_NEEDLE_CENTER, YELLOW);
				cliPrintln("Just right %d",(uint16_t)note->target_note);
			}
			else if (lo && md){
				lcd_draw_needle_at(TUNING_NEEDLE_LEFT, YELLOW);
				cliPrintln("Too low %d",(uint16_t)note->prev_note);
			}
			else if (hi && md){
				lcd_draw_needle_at(TUNING_NEEDLE_RIGHT, YELLOW);
				cliPrintln("Too high %d",(uint16_t)note->next_note);
			}
			if (md){
				lcd_draw_needle_at(TUNING_NEEDLE_CENTER, GREEN);
				cliPrintln("Just right %d",(uint16_t)note->target_note);
			}
			else if (silent_count && ++silent_count == SILENCE_THRESHOLD) {
				silent_count = 0;
				cliPrintln("Silent erasing");
				erase_needles();
			}
			else {
				cliPrintln("%d %d %d", lo, md, hi);
			}

			vTaskDelay(75);
			break;
		case ABOUT:
			break;
		case COLORTEST:
			break;
		default:
			cliPrintln((uint8_t*)"Unimplemented Screen State 0x%x", currentScreen);
	}
}

bool awaitTouch(){
	uint32_t ignore = 1;
	BaseType_t res;
	switch(currentScreen){
			case TUNE:
				return xTaskNotifyWait(ULONG_MAX,1,&ignore, 10);
			case LEARN:
				break;
			case START:
			case MODES:
			case ROCK:
			case ABOUT:
			case COLORTEST:
				ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
				break;
			default:
				cliPrintln((uint8_t*)"Unimplemented Screen State (touch) 0x%x", currentScreen);
		}
	return true;
}

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/
void vLcdTask(void *pvParameters) {
	configASSERT(((uint32_t ) pvParameters) == 3);

	lcdSpiInit();

	bool limiter = true;



	while (1) {
		currentScreen = nextScreen;
		lcd_hide_white_sweeping_transition();
		lcd_draw_background_from_sd();
		lcd_draw_foreground_from_flash();

		if (awaitTouch() && limiter) {
			uint16_t* point = touchscreenGetTouchPoint();
			lcd_analyze_point(point);
		}
		else noop = true;
		limiter = !limiter;
	}
}

// The x and y are flip-flopped as we use display sideways
void lcdSetPosition(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1) {
	uint8_t YSH, YSL, YEH, YEL;

	YSH = y0 >> 8;
	YSL = y0;

	YEH = y1 >> 8;
	YEL = y1;

	writeCommand(0x2A);
	writeCommandData(0x00);
	writeCommandData(x0);
	writeCommandData(0x00);
	writeCommandData(x1);
	writeCommand(0x2B);
	writeCommandData(YSH);
	writeCommandData(YSL);
	writeCommandData(YEH);
	writeCommandData(YEL);
	writeCommand(0x2C); // LCD_WriteCMD(GRAMWR);
}

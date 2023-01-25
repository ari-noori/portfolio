/*
 * peripherals.h
 *
 *  Created on: May 4, 2022
 *      Author: Arian Noori and Aaron Cohen
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_
#include "msp.h"
#include <stdbool.h>

void adc_comparator_init(void);
void T32_1_Interrupt_Ms(unsigned milliseconds);
bool S1_pressed(void);
void MKII_buttons_init(void);
bool S2_pressed(void);
void ece353_T32_2_wait(uint32_t ticks);
void ece353_MKII_Buzzer_Init(uint16_t ticks_period);
void ece353_MKII_Buzzer_On(void);
void ece353_MKII_Buzzer_Off(void);
void MKII_RGB_IO_Init(bool en_primary_function);
void MKII_RGB_PWM(uint16_t ticks_period, uint16_t red_num, uint16_t green_num, uint16_t blue_num);
void MKII_RGB_LED(bool red, bool green, bool blue);
void ece353_rgb(bool red_on, bool green_on, bool blue_on);
void ece353_rgb_init(void);
void ece353_rgb(bool red_on, bool green_on, bool blue_on);
void ece353_T32_1_wait(uint32_t ticks);
#endif /* PERIPHERALS_H_ */

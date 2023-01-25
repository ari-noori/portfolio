/*
 * peripherals.h
 *
 *  Created on: May 5, 2022
 *      Author: Aaron Cohen; Arian Noori
 */

#include "peripherals.h"

void adc_comparator_init(void)
{
    // Configure the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;
    // Configure the Y direction as an analog input pin.
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;

    // Configure the X direction of the ACCELEROMETER
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // Configure the Y direction of the ACCELEROMETER
    P4->SEL0 |= BIT0;
    P4->SEL1 |= BIT0;

    // Configure the Z direction of the ACCELEROMETER
    P4->SEL0 |= BIT2;
    P4->SEL1 |= BIT2;

    // Configure CTL0 to sample 16-times in pulsed sample mode and indicate that this is a sequence-of-channels.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02 | ADC14_CTL0_CONSEQ_1;

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the X accelerometer with MEM[2]
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_14;

    // Associate the Y accelerometer with MEM[3]
    ADC14->MCTL[3] = ADC14_MCTLN_INCH_13;

    // Associate the Z accelerometer with MEM[Z]
    ADC14->MCTL[4] = ADC14_MCTLN_INCH_11;

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

    // Associate the Y direction analog signal with MEM[1]
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;

    // Make sure to indicate this is the end of a sequence.
    ADC14->MCTL[4] |= ADC14_MCTLN_EOS;

    // Enable interrupts in the ADC AFTER a value is written into MEM[1].
    ADC14->IER0 = ADC14_IER0_IE1;

    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_SetPriority(ADC14_IRQn, 2);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}

// Init boosterpack buttons
void MKII_buttons_init(){
    P5->DIR &= ~BIT1; // Initialize S1
    P3->DIR &= ~BIT5; // Initialze S2
}

// Read s1 pressed
bool S1_pressed(void)
{
    if((P5->IN & BIT1) == 0){
        return true;
    } else {
        return false;
    }
}

// Read s2 pressed
bool S2_pressed(void)
{
    if((P3->IN & BIT5) == 0){
        return true;
    } else {
        return false;
    }
}

/*****************************************************
 * Busy waits for a given number of clock cycles
 *
 * Parameters: ticks - number of ticks to wait
 *
 * Returns: NONE
 *****************************************************/
void ece353_T32_1_wait(uint32_t ticks)
{
  // Stop timer by zeroing control register
  TIMER32_1->CONTROL = 0;

  // Set the timer to be a 32-bit, one shot
  TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE;

  // Set the load register with the desired number of ticks
  TIMER32_1->LOAD = ticks;

  // Start the timer
  TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

  // Wait until it reaches 0
  while(TIMER32_1->VALUE != 0);
}

/*****************************************************
 * Busy waits for a given number of clock cycles
 *
 * Parameters: ticks - number of ticks to wait
 *
 * Returns: NONE
 *****************************************************/
void ece353_T32_2_wait(uint32_t ticks)
{
  // Stop timer by zeroing control register
  TIMER32_2->CONTROL = 0;

  // Set the timer to be a 32-bit, one shot
  TIMER32_2->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE;

  // Set the load register with the desired number of ticks
  TIMER32_2->LOAD = ticks;

  // Start the timer
  TIMER32_2->CONTROL |= TIMER32_CONTROL_ENABLE;

  // Wait until it reaches 0
  while(TIMER32_2->VALUE != 0);
}

/*****************************************************
 * Initializes the buzzer on the MKII with the desired
 * period.
 *
 * Parameters: ticks_period - the number of ticks
 * (period of operation).
 *
 * Returns: NONE
 *****************************************************/
void ece353_MKII_Buzzer_Init(uint16_t ticks_period){
    // Set P2.7 to be a GPIO OUTPUT Pin
    P2->DIR |= BIT7;

    // The TimerA PWM controller will control the buzzer on the MKII
    // P2.7 <--> TA0.4
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;

    // Turn off TA0
    TIMER_A0->CTL = 0;

    // Set the period of the timer.
    TIMER_A0->CCR[0] = ticks_period - 1;

    // Configure the BUZZER Duty cycle to 50%
    TIMER_A0->CCR[4] = (ticks_period/2)-1;

    // Configure TA0.4 for RESET/SET Mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    // Select the master clock as the timer source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
}

/*****************************************************
 * Turns on the MKII Buzzer.
 *
 * Parameters: NONE
 *
 * Returns: NONE
 *****************************************************/
void ece353_MKII_Buzzer_On(void){
    // Clear the current mode control bits
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // Set mode control to up and clear the current count
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

/*****************************************************
 * Turns off the MKII Buzzer.
 *
 * Parameters: NONE
 *
 * Returns: NONE
 *****************************************************/
void ece353_MKII_Buzzer_Off(void){
    // Turn off timer by clearing mode control bits
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

/*****************************************************
 * Initialize hardware resources used to control RGBLED
 *****************************************************/
void ece353_rgb_init(void)
{
    P2 -> DIR = 0x07;
}

/*****************************************************
 * Turn RGBLED ON/Off.
 *
 * Parameters
 *  red_on      :   if true,  turn RGBLED.RED on
 *                  if false, turn RGBLED.RED off
 *  green_on    :   if true,  turn RGBLED.GREEN on
 *                  if false, turn RGBLED.GREEN off
 *  blue_on     :   if true,  turn RGBLED.BLUE on
 *                  if false, turn RGBLED.BLUE off
 *****************************************************/
void ece353_rgb(bool red_on, bool green_on, bool blue_on)
{
    //Check if red is supposed to be on, else turn it off.
    if(red_on){
        P2 -> OUT |= BIT0;
    } else {
        P2 -> OUT &= ~BIT0;
    }

    if(green_on){
        P2 -> OUT |= BIT1;
    } else {
        P2 -> OUT &= ~BIT1;
    }

    if(blue_on){
        P2 -> OUT |= BIT2;
    } else {
        P2 -> OUT &= ~BIT2;
    }
}

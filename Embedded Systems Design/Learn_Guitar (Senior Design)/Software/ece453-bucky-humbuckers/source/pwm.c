/*
 * pwm.c
 *
 *  Created on: Nov 21, 2022
 *      Author: Cohen
 *		Course: ECE453
 *		Project: Bucky Humbuckers
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
//#include "pwm.h"
#include "main.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

#define MY_TCPWM_PWM_NUM   (0UL)
#define MY_TCPWM_PWM_MASK  (1UL << MY_TCPWM_PWM_NUM)

/*******************************************************************************
 * Static Variable Definitions
 ******************************************************************************/
static cy_stc_tcpwm_pwm_config_t config =
{
    /* .pwmMode           = */ CY_TCPWM_PWM_MODE_PWM, /* Simple PWM Mode */
    /* .clockPrescaler    = */ CY_TCPWM_PWM_PRESCALER_DIVBY_4, /* Clk_counter = Clk_input / 4 */
    /* .pwmAlignment      = */ CY_TCPWM_PWM_LEFT_ALIGN, /* PWM signal is left aligned, meaning it starts high */
    /* .deadTimeClocks    = */ 0UL, /* dead time feature is disabled */
    /* .runMode           = */ CY_TCPWM_PWM_CONTINUOUS, /* Wrap around at terminal count. */
    /* .period0           = */ 99UL, /* Period of 100 (0-99). Terminal count event when rolls over back to 0. */
    /* .period1           = */ 199UL, /* Period of 200 (0-199). Terminal count event when rolls over back to 0. */
    /* .enablePeriodSwap  = */ true, /* period swapping feature is enabled */
    /* .compare0          = */ 33UL, /* duty cycle 33% (with period 100) */
    /* .compare1          = */ 66UL, /* duty cycle 33% (with period 200) */
    /* .enableCompareSwap = */ true, /* compare swapping feature is enabled */
    /* .interruptSources  = */ CY_TCPWM_INT_ON_TC, /* interrupt will rise on terminal count */
    /* .invertPWMOut      = */ 0UL, /* Not invert */
    /* .invertPWMOutN     = */ 0UL, /* Not invert */
    /* .killMode          = */ CY_TCPWM_PWM_STOP_ON_KILL, /* PWM stops counting on kill */
    /* .swapInputMode     = */ CY_TCPWM_INPUT_RISINGEDGE, /* This input is NOT used, leave it in default state (CY_TCPWM_INPUT_RISINGEDGE = 0UL) */
    /* .swapInput         = */ CY_TCPWM_INPUT_0,
    /* .reloadInputMode   = */ CY_TCPWM_INPUT_RISINGEDGE, /* This input is NOT used, leave it in default state (CY_TCPWM_INPUT_RISINGEDGE = 0UL) */
    /* .reloadInput       = */ CY_TCPWM_INPUT_0,
    /* .startInputMode    = */ CY_TCPWM_INPUT_RISINGEDGE, /* This input is NOT used, leave it in default state (CY_TCPWM_INPUT_RISINGEDGE = 0UL) */
    /* .startInput        = */ CY_TCPWM_INPUT_0,
    /* .killInputMode     = */ CY_TCPWM_INPUT_RISINGEDGE, /* This input is NOT used, leave it in default state (CY_TCPWM_INPUT_RISINGEDGE = 0UL) */
    /* .killInput         = */ CY_TCPWM_INPUT_0,
    /* .countInputMode    = */ CY_TCPWM_INPUT_LEVEL, /* Set this input to LEVEL and 1 (high logic level) */
    /* .countInput        = */ CY_TCPWM_INPUT_1 /* So the PWM will be enabled forever */
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

void pwmInit(){
	cy_rslt_t result;
	result =  Cy_TCPWM_PWM_Init(TCPWM0, MY_TCPWM_PWM_NUM, &config);
    CY_ASSERT(result == CY_TCPWM_SUCCESS);

    /* Enable the initialized PWM */
    Cy_TCPWM_PWM_Enable(TCPWM0, MY_TCPWM_PWM_NUM);

    /* Then start the PWM */
    Cy_TCPWM_TriggerReloadOrIndex(TCPWM0, MY_TCPWM_PWM_MASK);
}

// Probably need
uint32_t pwmWait(){
	return  Cy_TCPWM_Counter_GetCompare0(TCPWM0, MY_TCPWM_PWM_NUM);
}

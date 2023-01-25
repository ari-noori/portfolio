/*
 * opt3001.c
 *
 *  Created on: Mar 14, 2021
 *      Author: younghyunkim
 *      Edited: Aaron Cohen
 */

#include "opt3001.h"
#include "math.h"

/******************************************************************************
 * Initialize the opt3001 ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/

void opt3001_init(void)
{
    // Initialize OPT3001
    /*
    Conversion time: 100 ms
    Mode of conversion: Continuous conversion
    Leave everything as the default values at reset.
    */
    const uint16_t configuration = (OPT3001_RST &   ~BITB)  |   // 100ms conversion
                                                     BITA   | BIT9;   // Continuous
    i2c_write_16(OPT3001_SLAVE_ADDRESS, CONFIG_REG, configuration);
}

/******************************************************************************
 * Returns the current ambient light in lux
 *  ******************************************************************************/
float opt3001_read_lux(void)
{
    // Read the Result register of OPT3001 and convert into Lux, then return.
    uint16_t raw_data = i2c_read_16(OPT3001_SLAVE_ADDRESS, RESULT_REG);
    const float lux = 0.01 * (1 << (raw_data >> 12)) * (0x0FFF & raw_data);
    return lux;

}


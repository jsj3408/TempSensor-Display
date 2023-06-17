/*
 * i2c_comm.h
 *
 *  Created on: Jan 2, 2023
 *      Author: johns
 */

#ifndef I2C_COMM_H_
#define I2C_COMM_H_

/**********************************************************************************************************************
* Includes section
*********************************************************************************************************************/
#include "fsl_i2c.h"
#include "app_config.h"

/**********************************************************************************************************************
* Defines section
*********************************************************************************************************************/
#define I2C_SENSORS_MASTER_CLK_FREQ CLOCK_GetFreq(I2C1_CLK_SRC)

/**********************************************************************************************************************
* Function declaration section
*********************************************************************************************************************/
bool i2c_init(void);
status_t i2c_transfer(uint8_t address, i2c_direction_t action, uint8_t reg, void * volatile data, size_t dataLen);

#endif /* I2C_COMM_H_ */

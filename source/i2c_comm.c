/*
 * i2c_comm.c
 *
 *  Created on: Jan 2, 2023
 *      Author: johns
 */


/**********************************************************************************************************************
 * Includes section
 *********************************************************************************************************************/
 #include "i2c_comm.h"

/**********************************************************************************************************************
* Global variable
*********************************************************************************************************************/
static i2c_master_config_t masterConfig;
static i2c_master_transfer_t xfer;

/**********************************************************************************************************************
* Global function definition
*********************************************************************************************************************/

bool i2c_init(void)
{
	bool ret = true ;
	//Configure i2C communication
	masterConfig.baudRate_Bps = 200000U;
	masterConfig.enableStopHold = false;
	masterConfig.glitchFilterWidth = 50U;
	masterConfig.enableMaster = true;
	I2C_MasterInit(I2C1,&masterConfig,I2C_SENSORS_MASTER_CLK_FREQ);
	return ret ;
}

status_t i2c_transfer(uint8_t address, i2c_direction_t action, uint8_t reg, void * volatile data, size_t dataLen)
{
	xfer.slaveAddress = address;
	xfer.direction = action;
	xfer.subaddress = reg;
	xfer.subaddressSize = 1;
	xfer.data = (uint8_t *) data;
	xfer.dataSize = dataLen;
	xfer.flags = kI2C_TransferDefaultFlag;

	return  I2C_MasterTransferBlocking(I2C1, &xfer);
}

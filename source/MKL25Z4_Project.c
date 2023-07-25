/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL25Z4_Project.c
 * @brief   Application entry point.
 */
/**********************************************************************************************************************
* Includes section
*********************************************************************************************************************/
#include "MKL25Z4_Project.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/**********************************************************************************************************************
* Defines section
*********************************************************************************************************************/

#define USE_I2C
//#define GPIO_TOGGLE
//#define USE_SPI
/*
 * @brief   Application entry point.
 */
/**********************************************************************************************************************
* Global variable
*********************************************************************************************************************/
uint8_t status;
status_t result = kStatus_Success;

/**********************************************************************************************************************
* Global function definition
*********************************************************************************************************************/

int main(void)
{
    uint8_t chip_id_read[2] = {0};
    uint8_t data[10] = {0};
    uint8_t chip_addr[1] = {0xD0};
    uint8_t temp_len = 0; //length of the comp_temp output
    uint16_t output_print_len = 0; //length of output print
    unsigned char * temp_str = NULL; //string to hold the int-to-string text
    unsigned char output_print[] = "Temp is: ";
    uint32_t temp = 0;
    //status_t result = 1;
    uint8_t retry = 4;
    uint8_t ret = 0;
    state_t relay_action = off;
    int32_t comp_temp;
    uint8_t debug = 0;

	//SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
    /* Init board hardware. */
	KL_InitPins(); //Initializing pins
    printf("i2c/SPI pins and GPIO pins initialized...I hope\n");

    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }

    /*BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();*/
//#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
//    BOARD_InitDebugConsole();
//#endif
/*
 * The below macro was used to test SPI on the BMP280.
 * The purpose is served, and hence I am removing it
 * */

#ifdef USE_I2C
    ret = i2c_init();

    if (ret)
    {
    	printf("initialized i2c...\n");
    	ssd1306_init();
    	printf("Initialized SSD1306.....\n");
    	bmp280_init();

		printf("Initialized BMP280.");
		retry  = 50;
		result = 1;

		//ssd1306_write(output_print, sizeof(output_print));

		while(1)
		{
			result = i2c_transfer(BMP280_ADDR, kI2C_Read ,0xFA, data, 3); //read temperature
			temp = (uint32_t) (((uint32_t) data[2] >> 4) | ((uint32_t) data[1] << 4) | ((uint32_t) data[0] << 12));
			//printf("Raw temp is %d\n",temp);
			comp_temp = compensatedTemp((int32_t) temp);
			printf("Compensated temp is %d\n",comp_temp);
			//use snprintf to add the compensated temp to a string
			//use snprintf to find length of item
			temp_len = snprintf(NULL, 0, "%d", comp_temp);
			//allocate one more for null terminator
			temp_len += 1;
			temp_str = (unsigned char *) malloc(temp_len * sizeof(unsigned char));
			snprintf(temp_str, temp_len, "%d", comp_temp);
			//find length of final string to be displayed
			output_print_len = sizeof(output_print) + temp_len;
			//concatenate the temperature value with the output print
			strcat(output_print, temp_str);
			ssd1306_write(output_print, output_print_len);
			//ssd1306_write(temp_str, temp_len);
			SysTick_DelayTicks(1000U);
			free(temp_str);
			//10 is the size of the "Temp is: " string
			memset(output_print+9, 0, temp_len);
			//relay_action = controlTempAlgorithm (50 , 2);
			//printf ("Heater action:%d\n ",relay_action);
		}
    }
    else
    {
    	printf("initialization failed...\n");
    }
#endif

#ifdef GPIO_TOGGLE
    while (retry--)
    {
    	SysTick_DelayTicks(1000U);
    	printf("Toggling...\n");
    	GPIO_TogglePinsOutput(GPIOE, 0x40000000); //toggling PTE30. bit30 is enabled
    }
#endif
    printf("Hello World\n");

    /* Force the counter to be placed into memory. */
//    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
//    while(1) {
//        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
//        __asm volatile ("nop");
//    }
    return 0 ;
}


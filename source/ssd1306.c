/*
 * ssd1306.c
 *
 *  Created on: Jan 2, 2023
 *      Author: johns
 */

/**********************************************************************************************************************
* Includes section
*********************************************************************************************************************/
#include "ssd1306.h"

/**********************************************************************************************************************
* Global variable section
*********************************************************************************************************************/
uint8_t CMD_control_byte = 0x00 ;
uint8_t data_control_byte = 0x40 ;
/**********************************************************************************************************************
* Global function definition
*********************************************************************************************************************/
/**this function takes an array of uint8_t data and then transposes bits of the rows with columns.
 * INPUT: In this case the column bits are 8bits in total. The rows are the number of uint8_t data in the array.
 * OUTPUT: Rows are 8 in number, Column is variable)
 */

bool transpose(unsigned char * input, int rows,unsigned char * output)
{
	bool ret = true;
	\
	if ((input != NULL) || (output != NULL))
	{
		int i, j, k;
		i = 7;
		//this loops through the 8bits
		for(j = 0; j < 8 ; j++)
		{
			//starts at the very last row
			for (k = rows-1; k >= 0; k--)
			{
				output[j] = output[j] << 1 ;
				output[j] |= (input[k] >> i) & 0b01;
			}
			i-- ;
		}
	}
	else
	{
		ret = false;
	}
	return ret;
}


void ssd1306_init(void)
{
	uint8_t display_on   = 0xAF ;
	uint8_t display_reset = 0xA4 ;
	status_t result = 1 ;
	// data items:	//	set MUX ratio
	uint8_t config [] = { 0xA8, 0x3F,
					//	display offset
						0xD3, 0x00,
					//	display start line
						0x40,
					// 	segment re-map
						0xA1,
					// 	com output scan direction
						0xC8,
					//	com pins hardware config
						0xDA, 0x12,
					// 	contrast control
						0x81, 0x7F,
					//	disable display
						0xA4,
					//	normal display
						0xA6,
					//	set oscillator freq
						0xD5, 0x80,
					//	enable charge pump regulator
						0x8D, 0x14,
					//	put display on
						display_on } ;

	//ssd1306_cfg_t * config1;
	//config1->mux_ratio_addr =0xA8;

					//	setting horizontal addressing mode
	uint8_t addr_mode_default[] = {	0x20, 0x00,
					//  setting column start and end address
							0x21, 0x00, 0x7F,
					//	setting page start and end address
							0x22, 0x00, 0x07
	};


	unsigned char A_letter [] = {
			0b00010000,
			0b00101000,
			0b01000100,
			0b11111100,
			0b10000100,
			0b10000100
	};



	uint8_t read_data[1024] = {0};
    uint8_t blank_data[1024];
    memset(blank_data, 0, sizeof(blank_data));
    uint8_t full_data[1024];
    memset(full_data, 0xFF, sizeof(full_data));
    printf("Configuration CMDs....\n");
	result = i2c_transfer(SSD1306_ADDR, kI2C_Write, CMD_control_byte, config, sizeof(config));
	//printf("Switching off....\n");
	SysTick_DelayTicks(10U);
	printf("Next, configure data input addressing mode....\n");
	result = i2c_transfer(SSD1306_ADDR, kI2C_Write, CMD_control_byte, addr_mode_default, sizeof(addr_mode_default));

	printf("Try making data full white...\n");
	result = i2c_transfer(SSD1306_ADDR, kI2C_Write, data_control_byte, full_data, sizeof(full_data));
	SysTick_DelayTicks(500U);
	printf("Try clearing some data...\n");
	result = i2c_transfer(SSD1306_ADDR, kI2C_Write, data_control_byte, blank_data, sizeof(blank_data));
	SysTick_DelayTicks(500U);
	/*for (int h = 0; h < 1024; h++)
	{
		result = i2c_transfer(SSD1306_ADDR, kI2C_Write, data_control_byte, &full_data[0], 1);
		//result = i2c_transfer(SSD1306_ADDR, kI2C_Write, data_control_byte, 0xFF, 1);
		SysTick_DelayTicks(25U);
	}*/
	printf ("That should do it\n");
}

void ssd1306_write(unsigned char * input , uint16_t input_len)
{
	status_t result = 1 ;
	//	setting horizontal addressing mode for the input
	uint8_t addr_mode[] = {	0x20, 0x00,
					//  setting column start and end address
							0x21, 0x00, 0x7F,
					//	setting page start and end address
							0x22, 0x00, 0x07
	};

	/**each letter is 8x8. So in one page (containing 8 pixel rows),
	 * we can have 128/8 = 16 characters/letters.
	 * Hence, we have a total of 16*(8 pages) = 128 chars/letters in total
	**/
	//array to be sent to ssd1306
	uint8_t sending_arr[1024] = {0};
	memset(sending_arr, 0, 1024);

	unsigned char letter[8] = {0};
	unsigned char transposed_letter[8] = {0};
	int16_t offset = 0;
	//we dont want to evaluate the /0 character at the end
	uint16_t arr_offset = 0;
	for (int m = 0; m < input_len-1; m++)
	{
		if (input[m] != ' ')
		{
			offset = input[m]-calibri_7ptFontInfo.startChar;
			FONT_CHAR_INFO letter_info = calibri_7ptFontInfo.charInfo[offset];
			memcpy(letter, calibri_7ptFontInfo.data+letter_info.offset, 8);
			//transpose this letter
			if (!transpose (letter, 8, transposed_letter))
			{
				return;
			}
		}
		//add this transposed letter to the sending array
		memcpy(sending_arr + arr_offset, transposed_letter, 8);
		arr_offset += 8;
		memset(letter, 0, 8);
		memset(transposed_letter, 0, 8);
	}
	printf("Configure addressing mode...\n");
	result = i2c_transfer(SSD1306_ADDR, kI2C_Write, CMD_control_byte, addr_mode, sizeof(addr_mode));
	SysTick_DelayTicks(10U);
	printf("Try inputting some data...\n");
	result = i2c_transfer(SSD1306_ADDR, kI2C_Write, data_control_byte, sending_arr, sizeof(sending_arr));
}


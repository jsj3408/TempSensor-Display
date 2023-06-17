/*
 * ssd1306.h
 *
 *  Created on: Jan 2, 2023
 *      Author: johns
 */

#ifndef SSD1306_H_
#define SSD1306_H_

/**********************************************************************************************************************
* Includes section
*********************************************************************************************************************/
#include "i2c_comm.h"
#include "bitmap_array.h"

/**********************************************************************************************************************
* Defines section
*********************************************************************************************************************/
#define SSD1306_ADDR 0x3C

/**********************************************************************************************************************
* Typedef section
*********************************************************************************************************************/
typedef struct
{
	uint8_t mux_ratio_addr,
			mux_ratio_val,
			disp_offset_addr,
			disp_offset_val,
			segment_remap,
			output_scan_dir,
			hardware_cfg_addr,
			hardware_cfg_val,
			contrast_ctrl_addr,
			contrast_ctrl_val,
			disable_disp,
			normal_disp,
			osc_freq_addr,
			osc_freq_val,
			charge_regulator_addr,
			charge_regulator_val,
			display_on;
} ssd1306_cfg_t;

typedef struct
{
	uint8_t addr_mode_addr,
			addr_mode_set,
			set_column_addr,
			set_column_start,
			set_column_end,
			set_page_addr,
			set_page_start,
			set_page_end;
} addr_mode_cfg_t;

/**********************************************************************************************************************
* Function declaration section
*********************************************************************************************************************/
void ssd1306_init(void);
bool transpose(unsigned char * input, int rows,unsigned char * output);
void ssd1306_write(unsigned char * input , uint16_t input_len);

#endif /* SSD1306_H_ */

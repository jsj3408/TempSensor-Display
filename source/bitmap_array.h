/*
 * bitmap_array.h
 *
 *  Created on: Jan 14, 2023
 *      Author: johns
 */

#ifndef BITMAP_ARRAY_H_
#define BITMAP_ARRAY_H_
/**********************************************************************************************************************
* Includes section
*********************************************************************************************************************/
#include <stdint.h>

/**********************************************************************************************************************
* Typedef section
*********************************************************************************************************************/
// This structure describes a single character's display information
typedef struct
{
	const uint16_t widthBits;					// width, in bits (or pixels), of the character
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array

} FONT_CHAR_INFO;

// Describes a single font
typedef struct
{
	const uint8_t 			heightPages;	// height, in pages (8 pixels), of the font's characters
	const uint8_t 			startChar;		// the first character in the font (e.g. in charInfo and data)
	const uint8_t 			endChar;		// the last character in the font
	const uint8_t			spacePixels;	// number of pixels that a space character takes up
	const FONT_CHAR_INFO*	charInfo;		// pointer to array of char information
	const uint8_t*			data;			// pointer to generated array of character visual representation

} FONT_INFO;
/**********************************************************************************************************************
* Function declaration section
*********************************************************************************************************************/
/* Font data for Calibri 6pt */
extern const uint8_t calibri_7ptBitmaps[];
extern const FONT_INFO calibri_7ptFontInfo;
extern const FONT_CHAR_INFO calibri_7ptDescriptors[];



#endif /* BITMAP_ARRAY_H_ */

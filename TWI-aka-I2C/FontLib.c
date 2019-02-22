/*
 * FontLib.c
 *
 * Created: 21.02.2019 21:24:56
 *  Author: admin
 */ 
#include <stdint.h>
#include <avr/pgmspace.h>
#include "FontLib.h"
#include "I2C_Display.h"


// TEMPLATE
/*
[0x34] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000
}
*/

const uint8_t f6x8_table[CHAR_COUNT][CHAR_HEIGHT] PROGMEM = {
	// 0
	[0x30] = {
		0b00000000,
		0b01111110,
		0b10100001,
		0b10010001,
		0b10001001,
		0b10000101,
		0b01111110,
		0b00000000
	}, // 1
	[0x31] = {
		0b00000000,
		0b10000000,
		0b10000010,
		0b11111111,
		0b10000000,
		0b10000000,
		0b00000000,
		0b00000000
		
	}, // 2 
	[0x32] = {
		0b00000000,
		0b11100010,
		0b10010001,
		0b10010001,
		0b10001001,
		0b10001001,
		0b10000110,
		0b00000000
	}, // 3
	[0x33] = {
		0b00000000,
		0b01000010,
		0b10000001,
		0b10000001,
		0b10001001,
		0b10001001,
		0b01110110,
		0b00000000
	}, // 4
	[0x34] = {
		0b00000000,
		0b00111000,
		0b00100100,
		0b00100010,
		0b11111111,
		0b00100000,
		0b00100000,
		0b00000000
	}, // 5
	[0x35] = {
		0b00000000,
		0b01000111,
		0b10001001,
		0b10001001,
		0b10001001,
		0b10001001,
		0b01110001,
		0b00000000
	}, // 6
	[0x36] = {
		0b00000000,
		0b01111110,
		0b10010001,
		0b10010001,
		0b10010001,
		0b10010001,
		0b01100010,
		0b00000000
	}, // 7
	[0x37] = {
		0b00000000,
		0b00000001,
		0b00000001,
		0b11100001,
		0b00010001,
		0b00001001,
		0b00000111,
		0b00000000
	}, // 8
	[0x38] = {
		0b00000000,
		0b01110110,
		0b10001001,
		0b10001001,
		0b10001001,
		0b10001001,
		0b01110110,
		0b00000000
	}, // 9
	[0x39] = {
		0b00000000,
		0b01001110,
		0b10010001,
		0b10010001,
		0b10010001,
		0b10010001,
		0b01111110,
		0b00000000
	}, // :
	[0x3A] = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b01100110,
		0b01100110,
		0b00000000,
		0b00000000,
		0b00000000
	}
};

uint8_t f6x8_charbuff[CHAR_HEIGHT];

uint8_t *f6x8m_GetCharTable(uint8_t Char)
{
	for (uint8_t i = 0; i < CHAR_HEIGHT; i++)
	f6x8_charbuff[i] = pgm_read_word(f6x8_table[Char] + i);
	
	return f6x8_charbuff;
}

void CharToBuffer(uint16_t xStart, uint8_t chr){
	uint8_t * charTbl = f6x8m_GetCharTable(chr);
	for(uint8_t x = 0; x < CHAR_HEIGHT; x++)
		setColumnData(xStart + x, charTbl[x]);
}

void drawString(uint16_t xStart, char * str){
	uint8_t eof = 0;
	while(!eof){
		switch(*str){
			case '\0':
			eof = 1;
			break;
			default:
				CharToBuffer((xStart += CHAR_HEIGHT) - CHAR_HEIGHT, *str);
			break;
		}
		str++;
	}
}
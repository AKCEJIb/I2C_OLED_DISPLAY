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
		0b00111110,
		0b01100001,
		0b01010001,
		0b01001001,
		0b01000101,
		0b00111110,
		0b00000000
	}, // 1
	[0x31] = {
		0b00000000,
		0b01000000,
		0b01000010,
		0b01111111,
		0b01000000,
		0b01000000,
		0b00000000,
		0b00000000
		
	}, // 2
	[0x32] = {
		0b00000000,
		0b01100010,
		0b01010001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01000110,
		0b00000000
	}, // 3
	[0x33] = {
		0b00000000,
		0b00100010,
		0b01000001,
		0b01000001,
		0b01001001,
		0b01001001,
		0b00110110,
		0b00000000
	}, // 4
	[0x34] = {
		0b00000000,
		0b00011000,
		0b00010100,
		0b00010010,
		0b01111111,
		0b00010000,
		0b00010000,
		0b00000000
	}, // 5
	[0x35] = {
		0b00000000,
		0b00100111,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b00110001,
		0b00000000
	}, // 6
	[0x36] = {
		0b00000000,
		0b00111110,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b00110010,
		0b00000000
	}, // 7
	[0x37] = {
		0b00000000,
		0b00000001,
		0b00000001,
		0b01110001,
		0b00001001,
		0b00000101,
		0b00000011,
		0b00000000
	}, // 8
	[0x38] = {
		0b00000000,
		0b00110110,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b00110110,
		0b00000000
	}, // 9
	[0x39] = {
		0b00000000,
		0b00100110,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b00111110,
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
	}, // ����� �
	[0xC0] = {
		0b00000000,
		0b01111110,
		0b00001001,
		0b00001001,
		0b00001001,
		0b00001001,
		0b01111110,
		0b00000000
	}, // ����� �
	[0xC1] = {
		0b00000000,
		0b01111111,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b00110001,
		0b00000000
	}, // ����� �
	[0xC2] = {
		0b00000000,
		0b01111111,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01110110,
		0b00000000
	}, // ����� �
	[0xC3] = {
		0b00000000,
		0b01111111,
		0b00000001,
		0b00000001,
		0b00000001,
		0b00000001,
		0b00000001,
		0b00000000
	}, // ����� �
	[0xC4] = {
		0b00000000,
		0b01100000,
		0b00111000,
		0b00100110,
		0b00100001,
		0b00111111,
		0b01100000,
		0b00000000
	}, // ����� �
	[0xC5] = {
		0b00000000,
		0b01111111,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b01001001,
		0b00000000
	}, // ����� �
	[0xC6] = {
		0b00000000,
		0b01110111,
		0b00001000,
		0b01111111,
		0b00001000,
		0b01110111,
		0b00000000,
		0b00000000
	}, // ����� �
	[0xC7] = {
		0b00000000,
		0b00100010,
		0b01000001,
		0b01000001,
		0b01000001,
		0b01001001,
		0b00110110,
		0b00000000
	}, // ����� �
	[0xC8] = {
		0b00000000,
		0b01111111,
		0b00100000,
		0b00010000,
		0b00001000,
		0b00000100,
		0b01111111,
		0b00000000
	}, // ����� �
	[0xC9] = {
		0b00000000,
		0b01111111,
		0b00100000,
		0b00010001,
		0b00001001,
		0b00000100,
		0b01111111,
		0b00000000
	}, // ����� �
	[0xCA] = {
		0b00000000,
		0b01111111,
		0b00001000,
		0b00001000,
		0b00010100,
		0b01100011,
		0b00000000,
		0b00000000
	}, // ����� �
	[0xCB] = {
		0b00000000,
		0b01111100,
		0b00000010,
		0b00000001,
		0b00000001,
		0b00000001,
		0b01111111,
		0b00000000
	}, // ����� �
	[0xCC] = {
		0b00000000,
		0b01111111,
		0b00000010,
		0b00000100,
		0b00000010,
		0b01111111,
		0b00000000,
		0b00000000
	},// ����� �
	[0xCD] = {
		0b00000000,
		0b01111111,
		0b00001000,
		0b00001000,
		0b00001000,
		0b00001000,
		0b01111111,
		0b00000000
	}, // ����� �
	[0xCE] = {
		0b00000000,
		0b00111110,
		0b01000001,
		0b01000001,
		0b01000001,
		0b01000001,
		0b00111110,
		0b00000000
	}, // ����� �
	[0xCF] = {
		0b00000000,
		0b01111111,
		0b00000001,
		0b00000001,
		0b00000001,
		0b00000001,
		0b01111111,
		0b00000000
	}, // ����� �
	[0xD0] = {
		0b00000000,
		0b01111111,
		0b00001001,
		0b00001001,
		0b00001001,
		0b00001001,
		0b00000110,
		0b00000000
	}, // ����� �
	[0xD1] = {
		0b00000000,
		0b00111110,
		0b01000001,
		0b01000001,
		0b01000001,
		0b01000001,
		0b00100010,
		0b00000000
	}, // ����� �
	[0xD2] = {
		0b00000000,
		0b00000001,
		0b00000001,
		0b01111111,
		0b00000001,
		0b00000001,
		0b00000000,
		0b00000000
	}, // �
	[0xD3] = {
		0b00000000,
		0b00100111,
		0b01001000,
		0b01001000,
		0b01001000,
		0b01001000,
		0b00111111,
		0b00000000
	}, // �
	[0xD4] = {
		0b00000000,
		0b00011100,
		0b00100010,
		0b01111111,
		0b00100010,
		0b00011100,
		0b00000000,
		0b00000000
	}, // �
	[0xD5] = {
		0b00000000,
		0b01100011,
		0b00010100,
		0b00001000,
		0b00010100,
		0b01100011,
		0b00000000,
		0b00000000
	}, // �
	[0xD6] = {
		0b00000000,
		0b00011111,
		0b00100000,
		0b00100000,
		0b00100000,
		0b00111111,
		0b01100000,
		0b00000000
	}, // �
	[0xD7] = {
		0b00000000,
		0b00001111,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b01111111,
		0b00000000
	}, // �
	[0xD8] = {
		0b00000000,
		0b01111111,
		0b01000000,
		0b01111111,
		0b01000000,
		0b01111111,
		0b00000000,
		0b00000000
	}, // �
	[0xD9] = {
		0b00000000,
		0b00111111,
		0b00100000,
		0b00111111,
		0b00100000,
		0b00111111,
		0b01100000,
		0b00000000
	}, // �
	[0xDA] = {
		0b00000000,
		0b00000001,
		0b01111111,
		0b01000100,
		0b01000100,
		0b01000100,
		0b00111000,
		0b00000000
	}, // �
	[0xDB] = {
		0b00000000,
		0b01111111,
		0b01000100,
		0b01000100,
		0b00111000,
		0b00000000,
		0b01111111,
		0b00000000
	}, // �
	[0xDC] = {
		0b00000000,
		0b01111111,
		0b01000100,
		0b01000100,
		0b01000100,
		0b01000100,
		0b00111000,
		0b00000000
	}, // �
	[0xDD] = {
		0b00000000,
		0b00100010,
		0b01000001,
		0b01000001,
		0b01001001,
		0b01001001,
		0b00111110,
		0b00000000
	}, // �
	[0xDE] = {
		0b00000000,
		0b01111111,
		0b00001000,
		0b00111110,
		0b01000001,
		0b01000001,
		0b00111110,
		0b00000000
	}, // �
	[0xDF] = {
		0b00000000,
		0b01000110,
		0b00101001,
		0b00010001,
		0b00010001,
		0b00010001,
		0b01111111,
		0b00000000
	}, // >
	[0x3E] = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b01000001,
		0b00100010,
		0b00010100,
		0b00001000,
		0b00000000
	}, // - (�����)
	[0x2D] = {
		0b00000000,
		0b00000000,
		0b00001000,
		0b00001000,
		0b00001000,
		0b00001000,
		0b00000000,
		0b00000000
	}, // . (�����)
	[0x2E] = {
		0b00000000,
		0b01100000,
		0b01100000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000
	}, // ����� � (������� ��� ��������)
	[0xF7] = {
		0xE0, 0xF8, 0xFC, 0x3E, 0x0E, 0x07, 0x63, 0x73
	}, // ����� � (������� ��� ���������)
	[0xF8] = {
		0x3F, 0x1F,
		0x0F, 0x06, 0x82, 0xC0, 0xF0, 0xE0
	}, // ����� � (������� ��� �������)
	[0xF9] = {
		0x07, 0x1F, 0x3F, 0x3E,
		0x1C, 0x8C, 0xC6, 0xE2
	}, // ����� � (������� ��� ��������)
	[0xFA] = {
		0xF1, 0xF8, 0xFC, 0x7E, 0x7F, 0x3F,
		0x1F, 0x07
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

void drawStringCentered(char * str){
	drawString(128/2-strlen(str)*8/2, str);
}
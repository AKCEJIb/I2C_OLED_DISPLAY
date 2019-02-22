/*
 * TWI-aka-I2C.c
 *
 * Created: 18.02.2019 22:32:22
 * Author : admin
 * Ну это бан дурачок ёбаный =)
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "I2C_Display.h"
#include <util/delay.h>
#include "FontLib.h"

void DisplayInit(){
	DISP_Command(SSD1306_DISPLAY_OFF);
	_delay_ms(1);
	DISP_Command(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
	_delay_ms(1);
	DISP_Command(0x80);
	_delay_ms(1);
	DISP_Command(SSD1306_SET_MULTIPLEX_RATIO);
	_delay_ms(1);
	DISP_Command(0x3F);
	_delay_ms(1);
	DISP_Command(SSD1306_SET_DISPLAY_OFFSET);
	_delay_ms(1);
	DISP_Command(0x00);
	_delay_ms(1);
	DISP_Command(SSD1306_SET_START_LINE | 0x0);
	_delay_ms(1);
	
	// We use internal charge pump
	DISP_Command(SSD1306_CHARGE_PUMP);
	_delay_ms(1);
	DISP_Command(0x14);
	_delay_ms(1);
	
	// Horizontal memory mode
	DISP_Command(SSD1306_MEMORY_ADDR_MODE);
	_delay_ms(1);
	DISP_Command(0x00);
	_delay_ms(1);
	
	DISP_Command(SSD1306_SET_SEGMENT_REMAP | 0x1);
	_delay_ms(1);
	DISP_Command(SSD1306_COM_SCAN_DIR_DEC);
	_delay_ms(1);
	DISP_Command(SSD1306_SET_COM_PINS);
	_delay_ms(1);
	DISP_Command(0x12);
	_delay_ms(1);

	// Max contrast
	DISP_Command(SSD1306_SET_CONTRAST_CONTROL);
	_delay_ms(1);
	DISP_Command(0xCF);
	_delay_ms(1);
	DISP_Command(SSD1306_SET_PRECHARGE_PERIOD);
	_delay_ms(1);
	DISP_Command(0xF1);
	_delay_ms(1);
	
	DISP_Command(SSD1306_SET_VCOM_DESELECT);
	_delay_ms(1);
	DISP_Command(0x40);
	_delay_ms(1);
	DISP_Command(SSD1306_DISPLAY_ALL_ON_RESUME);
	_delay_ms(1);
	// Non-inverted display
	DISP_Command(SSD1306_NORMAL_DISPLAY);
	_delay_ms(1);
	// Turn display back on
	DISP_Command(SSD1306_DISPLAY_ON);
	_delay_ms(1);
}


int main(void)
{
	uint8_t hh = 9;
	uint8_t mm = 59;
	uint8_t ss = 50;
	
	char chaH[3];
	char chaM[3];
	char chaS[3];
	DDRB |= (1 << PB0 | 1 << PB1);
	sei();    
	
	DisplayInit();
	_delay_ms(10);
	

	clearScr();
	updateScreen(0);
	
	

	char HMS[15];
	char kek[16];
    while (1){
		itoa(hh,chaH, 10);
		itoa(mm,chaM, 10);
		itoa(ss,chaS, 10);
		
		if(hh < 10){
			strcpy(HMS, "0");
			strcat(HMS,chaH);
		}else
			strcpy(HMS,chaH);
		strcat(HMS,":");
		
		if(mm < 10)
		strcat(HMS,"0");
		strcat(HMS, chaM);
		strcat(HMS, ":");
		
		if(ss < 10)
		strcat(HMS,"0");
		strcat(HMS, chaS);
		
		
		clearScr();
		drawString(128/2-strlen(HMS)*8/2, HMS);
		updateScreen(4);
		ss++;
		if(ss > 59){
			ss = 0;
			mm++;
		}
		if(mm > 59){
			mm = 0;
			hh++;
		}
		if(hh > 23){
			hh = 0;
			mm = 0;
			ss = 0;
		}
		clearScr();
		
		if(strlen(kek) == 16)
			strcpy(kek,"");
		
		strcat(kek, ":");
		drawString(128/2-strlen(kek)*8/2, kek);

		updateScreen(2);
		updateScreen(6);
		_delay_ms(10);
	}
}


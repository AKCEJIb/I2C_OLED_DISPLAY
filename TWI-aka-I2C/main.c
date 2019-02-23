/*
* TWI-aka-I2C.c
*
* Created: 18.02.2019 22:32:22
* Author : admin
* ÕÛ ˝ÚÓ ·‡Ì ‰Û‡˜ÓÍ ∏·‡Ì˚È =)
*/


#define MENU_COUNT 3


#include "I2C_Display.h"
#include "FontLib.h"




uint8_t hh = 00;
uint8_t mm = 00;
uint8_t ss = 00;
uint8_t CLOCK_MODE = 0;

ISR(TIMER1_COMPA_vect){
	
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
}

int main(void)
{
	
	// char Hours, Minutes, Seconds
	char chaH[3];
	char chaM[3];
	char chaS[3];
	char HMS[15];
	
	// Animation
	//char ANIM_semicolons[16];
	
	
	// SETUP DEBUG LEDS
	DDRB |= (1 << PB0 | 1 << PB1 | 1 << PB3);
	sei();
	
	
	// SETUP BUTTONS INPUT
	PORTB |= (1 << PB4 | 1 << PB5 | 1 << PB6 | 1 << PB7);
	uint8_t dBtnMode = 0;
	uint8_t dBtnUp = 0;
	uint8_t dBtnDown = 0;
	uint8_t dBtnAccept = 0;
	
	// SETUP TIMER EVERY 1 SECOND
	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);
	OCR1A = 7812.5;
	TCCR1B |= ((1 << CS10) | (1 << CS12));
	
	// Initialize display
	DisplayInit();
	_delay_ms(10);
	
	// Menu settings
	uint8_t menuPos = 0;
	
	while (1){
		// Buttons
		
		
		
		uint8_t blBtnUp = abs(dBtnUp - (~PINB & (1<<PB4)));
		dBtnUp = ~PINB & (1<<PB4);
		
		uint8_t blBtnDown = abs(dBtnDown - (~PINB & (1<<PB5)));
		dBtnDown = ~PINB & (1<<PB5);
		
		uint8_t blBtnMode = abs(dBtnMode - (~PINB & (1<<PB6)));
		dBtnMode = ~PINB & (1<<PB6);
		
		uint8_t blBtnAccept = abs(dBtnAccept - (~PINB & (1<<PB7)));
		dBtnAccept = ~PINB & (1<<PB7);
		
		
		if((~PINB & (1<<PB4)) && blBtnUp){
			if(menuPos == 0) menuPos = MENU_COUNT;
			menuPos--;	
		}
		if((~PINB & (1<<PB5))  && blBtnDown){
			menuPos++;
			if(menuPos >= MENU_COUNT) menuPos = 0;
		}
		
		if((~PINB & (1<<PB6) ) && blBtnMode){
			CLOCK_MODE ^= 1;
			clearScr();
		}
		
		if((~PINB & (1<<PB7)) && blBtnAccept){
			
		}
		
		// Drawable
		switch(CLOCK_MODE)
		{
			case 0: // Just Clock
			
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
			
			// Draw Time
			clearBuff();
			drawStringCentered(HMS);
			updateScreen(DLINE_CENTER2);
			
			
			// Draw Borders
			clearBuff();
			drawStringCentered("----------------");
			updateScreen(DLINE_2);
			updateScreen(DLINE_6);
			
			clearBuff();
			drawString(2, "˜¯");
			drawStringCentered("◊¿—€");
			updateScreen(DLINE_0);
			clearBuff();
			drawString(2,"˘˙◊√”");
			updateScreen(DLINE_1);
			//// Draw ANIMATIONS
			//clearScr();
			//if(strlen(ANIM_semicolons) == 16)
			//strcpy(ANIM_semicolons,"");
			//
			//strcat(ANIM_semicolons, ":");
			//drawStringCentered(ANIM_semicolons);
			//
			//updateScreen(2); // The same as 6 layer
			//updateScreen(6); // The same as 2 layer
			break;
			case 1: // Settings menu
			
			
			clearBuff();
			drawStringCentered("Õ¿—“–Œ… »:");
			updateScreen(DLINE_0);
			clearBuff();
			if(menuPos == 0) drawString(0,">");
			drawString(8,"1. ¡”ƒ»À‹Õ» ");
			updateScreen(DLINE_2);
			clearBuff();
			if(menuPos == 1) drawString(0,">");
			drawString(8,"2. ”—“ ¬–≈Ãﬂ");
			updateScreen(DLINE_CENTER1);
			clearBuff();
			if(menuPos == 2) drawString(0,">");
			drawString(8,"3. ”—“ ƒ¿“”");
			updateScreen(DLINE_CENTER2);
			
			break;
		}
		_delay_ms(10);
	}
}


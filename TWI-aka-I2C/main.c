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
uint8_t hh = 00;
uint8_t mm = 00;
uint8_t ss = 00;
uint8_t CLOCK_MODE = 0;
ISR(TIMER1_COMPA_vect){
	PORTB ^= (1 << PB3);
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
	char ANIM_semicolons[16];
	
	
	// SETUP DEBUG LEDS
	DDRB |= (1 << PB0 | 1 << PB1 | 1 << PB3);
	sei();
	
	
	// SETUP BUTTONS INPUT
	PORTB |= (1 << PB4 | 1 << PB5 | 1 << PB6 | 1 << PB7);
	
	
	// SETUP TIMER EVERY 1 SECOND
	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);
	OCR1A = 7812.5;
	TCCR1B |= ((1 << CS10) | (1 << CS12));
	
	// Initialize display
	DisplayInit();
	_delay_ms(10);
	


	
	while (1){
		// Buttons
		if(~PINB & (1<<PB6)){
			CLOCK_MODE = 1;
			clearScr();
			for(uint8_t i = 0; i < 8; i++)
				updateScreen(i);
		}
		
		// Drawable
		switch(CLOCK_MODE)
		{
			case 0:
			
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
			clearScr();
			drawString(128/2-strlen(HMS)*8/2, HMS);
			updateScreen(4);
			
			
			// Draw ANIMATIONS
			clearScr();
			if(strlen(ANIM_semicolons) == 16)
			strcpy(ANIM_semicolons,"");
			
			strcat(ANIM_semicolons, ":");
			drawString(128/2-strlen(ANIM_semicolons)*8/2, ANIM_semicolons);
			
			updateScreen(2); // The same as 6 layer
			updateScreen(6); // The same as 2 layer
			break;
			case 1:
			// Draw Alarm Choose
			
			clearScr();
			drawString(0,"КРАБ");
			updateScreen(0);
			clearScr();
			drawString(0,"СОСИ ЖОПУ");
			updateScreen(1);
			break;
		}
		_delay_ms(10);
	}
}


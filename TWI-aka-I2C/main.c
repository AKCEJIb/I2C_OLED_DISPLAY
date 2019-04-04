/*
* TWI-aka-I2C.c
*
* Created: 18.02.2019 22:32:22
* Author : AKCEJIb
*/


#define MENU_COUNT 2
#define ALARM_TIME 10

// Buttons
#define buttonMode PC1
#define buttonAccept PC0
#define buttonUp PC2
#define buttonDown PC3

#define soundSpeaker PB1

#include "I2C_Display.h"
#include "NoteLib.h"

uint8_t hh = 00;
uint8_t mm = 00;
uint8_t ss = 00;
uint8_t CLOCK_MODE = 0;


// Alarm settings
uint8_t alarmHrs = 0;
uint8_t alarmMin = 0;
uint8_t alarmSecs = 0;
uint8_t alarmEnabled = 0;
uint8_t alarmPos = 0;
uint8_t alarmTime = 0;
uint8_t alarmFlag = 0;
uint8_t setTim = 0;

// Music
uint8_t bit = 0;
uint8_t note = 0;
uint8_t trackLenght = sizeof(alarmSong)/sizeof(*alarmSong);

ISR(TIMER1_COMPA_vect){
	sei();
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
	
	if(alarmEnabled && hh == alarmHrs && mm == alarmMin && ss == alarmSecs) alarmFlag = 0;
	if(alarmEnabled && hh >= alarmHrs && mm >= alarmMin && (ss >= alarmSecs || mm > alarmMin) && !alarmFlag) alarmTime++;
	if(alarmTime >= ALARM_TIME){
		alarmTime = 0;
		alarmEnabled = 0;
		TCCR0 = 0;
		bit = 0;
		note = 0;
		PORTB &= ~(1 << soundSpeaker);
	}
}
ISR(TIMER0_OVF_vect){
	sei();
	TCNT0 = 255-setTim;
	PORTB ^= (1 << soundSpeaker);
	
}

int main(void)
{
	cli();
	// char Hours, Minutes, Seconds
	char chaH[3];
	char chaM[3];
	char chaS[3];
	char HMS[9];
	
	
	// SETUP BUTTONS INPUT
	PORTC |= (1 << buttonAccept | 1 << buttonUp | 1 << buttonDown | 1 << buttonMode);
	
	uint8_t dBtnMode = 0;
	uint8_t dBtnUp = 0;
	uint8_t dBtnDown = 0;
	uint8_t dBtnAccept = 0;
	
	sei();
	// SETUP 16bit TIMER EVERY 1 SECOND
	TCCR1B |= (1 << WGM12);
	TIMSK |= ( 1 << OCIE1A);
	TCCR1B |= (1 << CS12 | 1 << CS10); // Timer Enable
	OCR1A = 5858;
	
	// Setup 8bit timer
	TIMSK |= ( 1 << TOIE0 ); // Allow 8bit timer interrupt
	//TCCR0 |= ( 1 << CS00 | 1 << CS02); // Setup prescale by 1024 =)
	
	

	// Initialize display
	DisplayInit();
	_delay_ms(10);
	clearScr();
	_delay_ms(10);
	// Menu settings
	uint8_t menuPos = 0;
	

	
	
	while (1){
		
		// Alarm
		if(alarmEnabled && hh >= alarmHrs && mm >= alarmMin && (ss >= alarmSecs || mm > alarmMin) && !alarmFlag){
			note = pgm_read_word(&alarmSong[bit]);
			if(!note) TCCR0 = 0; else TCCR0 |= ( 1 << CS00 | 1 << CS02);
			if(note != 1)
			setTim = note;
			if(bit++ >= trackLenght) bit = 0;
		}
		
		// Buttons
		uint8_t blBtnUp = abs(dBtnUp - (~PINC & (1 << buttonUp)));
		dBtnUp = ~PINC & (1<<buttonUp);
		
		uint8_t blBtnDown = abs(dBtnDown - (~PINC & (1 << buttonDown)));
		dBtnDown = ~PINC & (1<<buttonDown);
		
		uint8_t blBtnMode = abs(dBtnMode - (~PINC & (1 << buttonMode)));
		dBtnMode = ~PINC & (1<<buttonMode);
			
		uint8_t blBtnAccept = abs(dBtnAccept - (~PINC & (1 << buttonAccept)));
		dBtnAccept = ~PINC & (1<<buttonAccept);
		
		
		if((~PINC & (1<<buttonUp)) && blBtnUp){
			switch(CLOCK_MODE){
				case 1:  // Menu
					if(menuPos == 0) menuPos = MENU_COUNT;
					menuPos--;
				break;
				case 2: // Alarm
					switch(alarmPos){
						case 0:
							alarmHrs++;
							if(alarmHrs >= 24) alarmHrs = 0;
						break;
						case 1:
							alarmMin++;
							if(alarmMin >= 60) alarmMin = 0;
						break;
						case 2:
							alarmSecs++;
							if(alarmSecs >= 60) alarmSecs = 0;
						break;
					}
				break;
				
				case 3: // Set time
				switch(alarmPos){
					case 0:
					hh++;
					if(hh >= 24) hh = 0;
					break;
					case 1:
					mm++;
					if(mm >= 60) mm = 0;
					break;
					case 2:
					ss++;
					if(ss >= 60) ss = 0;
					break;
				}
				break;
			}
		}
		if((~PINC & (1<<buttonDown))  && blBtnDown){
			switch(CLOCK_MODE){
				case 1: // Menu
					menuPos++;
					if(menuPos >= MENU_COUNT) menuPos = 0;
				break;
				case 2:  // Alarm
					switch(alarmPos){
						case 0:
							if(alarmHrs == 0) alarmHrs = 24;
							alarmHrs--;
						break;
						case 1:
							if(alarmMin == 0) alarmMin = 60;
							alarmMin--;
						break;
						case 2:
							if(alarmSecs == 0) alarmSecs = 60;
							alarmSecs--;
						break;
					}
				break;
				
				case 3:  // Set time
				switch(alarmPos){
					case 0:
					if(hh == 0) hh = 24;
					hh--;
					break;
					case 1:
					if(mm == 0) mm = 60;
					mm--;
					break;
					case 2:
					if(ss == 0) ss = 60;
					ss--;
					break;
				}
				break;
			}
		}
		
		if((~PINC & (1 << buttonMode) ) && blBtnMode && !dBtnAccept){
			if(CLOCK_MODE > 0){
				CLOCK_MODE = 0;
			}else{
				CLOCK_MODE = 1;
			}
			clearScr();
		}
		
		if((~PINC & (1 << buttonAccept)) && blBtnAccept){
			switch(CLOCK_MODE){
				case 1: // Settings menu
				
					switch(menuPos){
						case 0: // Alarm menu setup
							alarmPos = 0;
							CLOCK_MODE = 2;
							clearScr();
						break;
						case 1: // Clock menu setup
							CLOCK_MODE = 3;
							clearScr();
						break;
					}
				
				break;
				case 2: // Alarm setup menu
					if(dBtnMode){ // Disable alarm
						alarmEnabled = 0;
						alarmPos = 0;
						clearScr();
						CLOCK_MODE = 0;
					}
					alarmPos++;
					if(alarmPos > 2) {
						alarmPos = 0;
						alarmEnabled = 1; // Enable alarm
						
						if(alarmHrs <= hh) alarmFlag = 1;
						if(alarmMin <= mm) alarmFlag = 1;
						if(alarmSecs <= ss) alarmFlag = 1;
						
						clearScr();
						CLOCK_MODE = 0;
					}
				break;
				case 3: // Clock menu
					alarmPos++;
					if(alarmPos > 2 && !dBtnMode) alarmPos = 0;
				break;
				default:
					// Disable alarm
					if(alarmEnabled && hh >= alarmHrs && mm >= alarmMin && (ss >= alarmSecs || mm > alarmMin) && !alarmFlag) {
						alarmTime = 0;
						alarmEnabled = 0;
						TCCR0 = 0;
						bit = 0;
						note = 0;
						PORTC &= ~(1 << soundSpeaker);
					}
				break;
			}
		}
		
		// Set timer to 0, for not clicking while changing time
		if(CLOCK_MODE == 3) TCNT1 = 0;
		
		// Drawable
		switch(CLOCK_MODE)
		{
			/*--------------- Main Screen -----------------*/
			case 0: // Time, Main Screen
			
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
				drawString(2, "÷ø");
				drawStringCentered("×ÀÑÛ");
				updateScreen(DLINE_0);
				clearBuff();
				drawString(2,"ùú×ÃÓ");
				if(alarmEnabled) drawStringRight("ÁÓÄ: ÂÊË");
				updateScreen(DLINE_1);
			
			break;
			
			/*-----------------MENU---------------*/
			case 1: // Settings menu
			
			
				clearBuff();
				drawStringCentered("ÍÀÑÒÐÎÉÊÈ:");
				updateScreen(DLINE_0);
				clearBuff();
				if(menuPos == 0) drawString(0,">");
				drawString(8,"1. ÁÓÄÈËÜÍÈÊ");
				updateScreen(DLINE_2);
				clearBuff();
				if(menuPos == 1) drawString(0,">");
				drawString(8,"2. ÓÑÒ ÂÐÅÌß");
				updateScreen(DLINE_CENTER1);
				clearBuff();
			break;
			/*---------------SET ALARM-----------------*/
			case 2:
				itoa(alarmHrs, chaH, 10);
				itoa(alarmMin, chaM, 10);
				itoa(alarmSecs, chaS, 10);
				
				if(alarmHrs < 10){
					strcpy(HMS, "0");
					strcat(HMS,chaH);
				}else
				strcpy(HMS,chaH);
				strcat(HMS,":");
				
				if(alarmMin < 10)
				strcat(HMS,"0");
				strcat(HMS, chaM);
				strcat(HMS, ":");
				
				if(alarmSecs < 10)
				strcat(HMS,"0");
				strcat(HMS, chaS);
			
				clearBuff();
				drawStringCentered("ÓÑÒ ÁÓÄÈËÜÍÈÊ");
				updateScreen(DLINE_0);
				
				
				clearBuff();
				drawStringCentered(HMS);
				updateScreen(DLINE_CENTER2);
				
				clearBuff();
				drawString((alarmPos-1)*3*8+128/2-8,"--");
				updateScreen(DLINE_5);
				
			break;
			/*------------SET TIME-------------*/
			case 3: // Time set menu
				clearBuff();
				drawStringCentered("ÓÑÒ ÂÐÅÌß");
				updateScreen(DLINE_0);
				
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
				
				
				clearBuff();
				drawString((alarmPos-1)*3*8+128/2-8,"--");
				updateScreen(DLINE_5);
			break;
		
		}
		_delay_ms(10);
	}
}


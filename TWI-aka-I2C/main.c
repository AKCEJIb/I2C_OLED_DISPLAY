/*
* TWI-aka-I2C.c
*
* Created: 18.02.2019 22:32:22
* Author : AKCEJIb
*/


#define MENU_COUNT 2
#define ALARM_TIME 10

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
	if(alarmEnabled && hh == alarmHrs && mm == alarmMin && ss == alarmSecs) alarmFlag = 0;
	if(alarmEnabled && hh >= alarmHrs && mm >= alarmMin && (ss >= alarmSecs || mm > alarmMin) && !alarmFlag) alarmTime++;
	if(alarmTime >= ALARM_TIME){
		alarmTime = 0;
		alarmEnabled = 0;
		TCCR0 = 0;
		bit = 0;
		note = 0;
		PORTB &= ~(1 << PB3);
	}
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
ISR(TIMER0_OVF_vect){
	
	TCNT0 = 255-setTim;
	PORTB ^= (1 << PB3);
	
}

int main(void)
{
	
	// char Hours, Minutes, Seconds
	char chaH[3];
	char chaM[3];
	char chaS[3];
	char HMS[9];
	
	
	// SETUP DEBUG LEDS
	DDRB |= (1 << PB0 | 1 << PB1 | 1 << PB3);
	cli();
	
	
	// SETUP BUTTONS INPUT
	PORTB |= (1 << PB4 | 1 << PB5 | 1 << PB6 | 1 << PB7);
	uint8_t dBtnMode = 0;
	uint8_t dBtnUp = 0;
	uint8_t dBtnDown = 0;
	uint8_t dBtnAccept = 0;
	
	// SETUP 16bit TIMER EVERY 1 SECOND
	TCCR1B |= (1 << WGM12);
	TIMSK |= ( 1 << OCIE1A);
	OCR1A = 7811.5;
	TCCR1B |= ((1 << CS10) | (1 << CS12)); // Timer Enable
	
	
	// Setup 8bit timer
	TIMSK |= ( 1 << TOIE0 ); // Allow 8bit timer interrupt
	//TCCR0 |= ( 1 << CS00 | 1 << CS02); // Setup prescale by 1024 =)
	
	
	sei();
	// Initialize display
	DisplayInit();
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
		uint8_t blBtnUp = abs(dBtnUp - (~PINB & (1<<PB4)));
		dBtnUp = ~PINB & (1<<PB4);
		
		uint8_t blBtnDown = abs(dBtnDown - (~PINB & (1<<PB5)));
		dBtnDown = ~PINB & (1<<PB5);
		
		uint8_t blBtnMode = abs(dBtnMode - (~PINB & (1<<PB6)));
		dBtnMode = ~PINB & (1<<PB6);
			
		uint8_t blBtnAccept = abs(dBtnAccept - (~PINB & (1<<PB7)));
		dBtnAccept = ~PINB & (1<<PB7);
		
		
		if((~PINB & (1<<PB4)) && blBtnUp){
			switch(CLOCK_MODE){
				case 1:  // Меню
					if(menuPos == 0) menuPos = MENU_COUNT;
					menuPos--;
				break;
				case 2: // Будильник
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
				
				case 3: // Уст времени
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
		if((~PINB & (1<<PB5))  && blBtnDown){
			switch(CLOCK_MODE){
				case 1: // Меню
					menuPos++;
					if(menuPos >= MENU_COUNT) menuPos = 0;
				break;
				case 2:  // Будильник
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
				
				case 3:  // Установка времени
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
		
		if((~PINB & (1<<PB6) ) && blBtnMode && !dBtnAccept){
			if(CLOCK_MODE > 0){
				CLOCK_MODE = 0;
			}else{
				CLOCK_MODE = 1;
			}
			clearScr();
		}
		
		if((~PINB & (1<<PB7)) && blBtnAccept){
			switch(CLOCK_MODE){
				case 1: // Меню настроек
				
					switch(menuPos){
						case 0: // Переходим в меню настройки будильника
							alarmPos = 0;
							CLOCK_MODE = 2;
							clearScr();
						break;
						case 1: // Переходим в меню настройки времени
							CLOCK_MODE = 3;
							clearScr();
						break;
					}
				
				break;
				case 2: // Меню установки будильника
					if(dBtnMode){
						alarmEnabled = 0;
						alarmPos = 0;
						clearScr();
						CLOCK_MODE = 0;
					}
					alarmPos++;
					if(alarmPos > 2) {
						alarmPos = 0;
						alarmEnabled = 1; // Включение будильника
						
						if(alarmHrs <= hh) alarmFlag = 1;
						if(alarmMin <= mm) alarmFlag = 1;
						if(alarmSecs <= ss) alarmFlag = 1;
						
						clearScr();
						CLOCK_MODE = 0;
					}
				break;
				case 3: // Меню установки времени
					alarmPos++;
					if(alarmPos > 2 && !dBtnMode) alarmPos = 0;
				break;
				default:
					// Если будет звонить будильник сделать отключение
					if(alarmEnabled && hh >= alarmHrs && mm >= alarmMin && (ss >= alarmSecs || mm > alarmMin) && !alarmFlag) {
						alarmTime = 0;
						alarmEnabled = 0;
						TCCR0 = 0;
						bit = 0;
						note = 0;
						PORTB &= ~(1 << PB3);
					}
				break;
			}
		}
		
		// Set timer to 0, for not clicking while changing time
		if(CLOCK_MODE == 3) TCNT1 = 0;
		
		// Drawable
		switch(CLOCK_MODE)
		{
			/*--------------------------------*/
			case 0: // Дата, часы
			
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
				drawString(2, "чш");
				drawStringCentered("ЧАСЫ");
				updateScreen(DLINE_0);
				clearBuff();
				drawString(2,"щъЧГУ");
				if(alarmEnabled) drawStringRight("БУД: ВКЛ");
				updateScreen(DLINE_1);
			
			break;
			
			/*-----------------МЕНЮ---------------*/
			case 1: // Меню настроек
			
			
				clearBuff();
				drawStringCentered("НАСТРОЙКИ:");
				updateScreen(DLINE_0);
				clearBuff();
				if(menuPos == 0) drawString(0,">");
				drawString(8,"1. БУДИЛЬНИК");
				updateScreen(DLINE_2);
				clearBuff();
				if(menuPos == 1) drawString(0,">");
				drawString(8,"2. УСТ ВРЕМЯ");
				updateScreen(DLINE_CENTER1);
				clearBuff();
			break;
			/*---------------УСТ БУДИЛЬНИК-----------------*/
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
				drawStringCentered("УСТ БУДИЛЬНИК");
				updateScreen(DLINE_0);
				
				
				clearBuff();
				drawStringCentered(HMS);
				updateScreen(DLINE_CENTER2);
				
				clearBuff();
				drawString((alarmPos-1)*3*8+128/2-8,"--");
				updateScreen(DLINE_5);
				
			break;
			/*------------УСТ ВРЕМЯ-------------*/
			case 3: // Меню установки времени
				clearBuff();
				drawStringCentered("УСТ ВРЕМЯ");
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


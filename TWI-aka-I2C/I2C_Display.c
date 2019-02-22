/*
 * I2C_Display.c
 *
 * Created: 19.02.2019 1:00:53
 *  Author: admin
 */ 

#define I2C_START_BYTE 0x08
#define I2C_ASK_ADDR_BYTE 0x18
#define I2C_NASK_BYTE 0x20
#define I2C_ASK_BYTE 0x28

#include "I2C_Display.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>


uint16_t _SEND_ADDRESS, _SEND_PACKET_COUNT, _SEND_PACKET_COUNT_AMOUNT;
uint8_t * _SEND_PACKETS;
uint8_t bufferok[128];

void I2C_Start(){
	TWBR = TWBR_val;
	TWCR |= ( 1 << TWIE | 1 << TWEN | 1 << TWSTA | 1 << TWINT);
}
void I2C_Stop(){
	TWCR |= (1 << TWEN | 1 << TWSTO | 1 << TWINT |  1 << TWIE );
}

void I2C_SendByte(uint8_t cAddress, uint8_t cByte){
	
	_SEND_ADDRESS = cAddress;
	_SEND_PACKETS = (uint8_t *)malloc(1 * sizeof(uint8_t));
	
	_SEND_PACKETS[0] = cByte;
	_SEND_PACKET_COUNT_AMOUNT = 1;
	_SEND_PACKET_COUNT = 1;
	I2C_Start();	
}


void I2C_SendPacket(uint8_t * cPacket, uint8_t cSize){
	
	
	_SEND_PACKET_COUNT = cSize;
	_SEND_PACKET_COUNT_AMOUNT = cSize;
	
	_SEND_PACKETS = (uint8_t *)malloc(cSize * sizeof(uint8_t));
	
	memcpy(_SEND_PACKETS, cPacket, cSize * sizeof(uint8_t));
		
	
	_SEND_ADDRESS = DISPLAY_ADDR;

	I2C_Start();
}

void DISP_Command(uint8_t uCommand){
	
	uint8_t ControlByte = 0x00;
	
	uint8_t packet[] = {ControlByte, uCommand};
		
	_SEND_PACKET_COUNT_AMOUNT = sizeof(packet)/sizeof(*packet);
	_SEND_PACKET_COUNT = _SEND_PACKET_COUNT_AMOUNT;
	
	_SEND_PACKETS = (uint8_t *)malloc(_SEND_PACKET_COUNT * sizeof(uint8_t));
	
	memcpy(_SEND_PACKETS, packet, _SEND_PACKET_COUNT * sizeof(uint8_t));
	
	
	_SEND_ADDRESS = DISPLAY_ADDR;

	I2C_Start();
}
void DISP_BufferData(uint8_t * uData, uint16_t uSize) {
	uint8_t ControlByte = 0x40;
	uint8_t * packet;
	uint16_t constSize = uSize;
	packet = (uint8_t *)malloc((uSize +1) * sizeof(uint8_t));
	packet[0] = ControlByte;
	uint16_t cnt = 1;
	
	while(uSize--){
		packet[cnt++] = *(uData++);
	}
	_SEND_PACKET_COUNT_AMOUNT = constSize + 1;
	_SEND_PACKET_COUNT = _SEND_PACKET_COUNT_AMOUNT;
	
	_SEND_PACKETS = (uint8_t *)malloc(_SEND_PACKET_COUNT * sizeof(uint8_t));
	
	memcpy(_SEND_PACKETS, packet, _SEND_PACKET_COUNT * sizeof(uint8_t));
	
	
	_SEND_ADDRESS = DISPLAY_ADDR;

	free(packet);
	I2C_Start();	
}
void DISP_Data(uint8_t uData){
	
	uint8_t ControlByte = 0x40;
	
	uint8_t packet[] = {ControlByte, uData};
	
	_SEND_PACKET_COUNT_AMOUNT = sizeof(packet)/sizeof(*packet);
	_SEND_PACKET_COUNT = _SEND_PACKET_COUNT_AMOUNT;
	
	_SEND_PACKETS = (uint8_t *)malloc(_SEND_PACKET_COUNT * sizeof(uint8_t));
	
	memcpy(_SEND_PACKETS, packet, _SEND_PACKET_COUNT * sizeof(uint8_t));
	
	
	_SEND_ADDRESS = DISPLAY_ADDR;

	I2C_Start();
}


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


void updateScreen(uint8_t y){
	DISP_Command(SSD1306_SET_COLUMN_ADDR);
	_delay_us(300);
	DISP_Command(0);
	_delay_us(300);
	DISP_Command(SSD1306_WIDTH-1);
	_delay_us(300);
	DISP_Command(SSD1306_SET_PAGE_ADDR);
	_delay_us(300);
	DISP_Command(y);
	_delay_us(300);
	DISP_Command(y);
	_delay_us(300);

	DISP_BufferData(bufferok, 128);
	_delay_ms(15);
}

void clearScr(){
	memset(bufferok, 0x00, 128);
}

void setColumnData(uint16_t col, uint8_t data){
	bufferok[col] = data;
}

void drawPixel(uint16_t x, uint8_t y, uint8_t MODE){
	//uint16_t	by;
	uint8_t		bi;

	if ((x>=0) & (x<SSD1306_WIDTH) & (y>=0) & (y<SSD1306_HEIGHT))
	{
		//by=((y/8)*SSD1306_WIDTH)+x;
		bi=y % 8;
		
		switch(MODE){
			case 1:
				bufferok[x] |= (1 << bi);
			break;
			case 0:
				bufferok[x] &= ~(1 << bi);
			break;
		}
	
	}
}

// Ќаше ебучие прерывание, паравозик томас. QUAVO?
ISR(TWI_vect){
	switch(TWSR){
		case 0x08: // Start
			TWDR = _SEND_ADDRESS;
			TWCR = (1<<TWINT | 1<<TWEN | 1 << TWIE);
		break;
		case 0x18: // SLA+W ACK
			TWDR = _SEND_PACKETS[_SEND_PACKET_COUNT_AMOUNT-_SEND_PACKET_COUNT];
			TWCR = (1<<TWINT | 1<<TWEN | 1 << TWIE);
		break;
		case 0x28: // BYTE ACK
			_SEND_PACKET_COUNT--;
			if(_SEND_PACKET_COUNT > 0){
				PORTB &= ~(1 << PB0);
				TWDR = _SEND_PACKETS[_SEND_PACKET_COUNT_AMOUNT-_SEND_PACKET_COUNT];
				TWCR = (1<<TWINT | 1<<TWEN | 1 << TWIE);	
			}else{
				I2C_Stop();
				PORTB |= (1 << PB0);
				free(_SEND_PACKETS);
			}
		break;
		case 0x20: // SLA+W NACK
			I2C_Stop();
			PORTB |= (1 << PB1); // ¬ключаем пин слева первый сигнализиру€ об ошибке
			free(_SEND_PACKETS);
		break;
		//case 0x30: // BYTE NACK
//
			//PORTB |= (1 << PB1); // ¬ключаем пин слева первый сигнализиру€ об ошибке
		//break;
		//case 0x40: // READ ACK
	//
			//PORTB |= (1 << PB1); // ¬ключаем пин слева первый сигнализиру€ об ошибке
		//break;
		//case 0x48: // READ NACK
		//
			//PORTB |= (1 << PB1); // ¬ключаем пин слева первый сигнализиру€ об ошибке
		//break;
		//case 0x50: // RECEIVE BYTE
//
			//PORTB |= (1 << PB1); // ¬ключаем пин слева первый сигнализиру€ об ошибке
		//break;
		
	}
}

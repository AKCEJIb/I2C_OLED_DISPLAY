/*
 * I2C_Display.h
 *
 * Created: 19.02.2019 1:01:42
 *  Author: admin
 */ 

#include <stdint.h>
#include <avr/pgmspace.h>
#ifndef I2C_DISPLAY_H_
#define I2C_DISPLAY_H_

#define F_CPU 8000000UL // CPU Frequency
#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

#define DISPLAY_ADDR 0b1111000

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define SSD1306_SET_CONTRAST_CONTROL					0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME					0xA4
#define SSD1306_DISPLAY_ALL_ON							0xA5
#define SSD1306_NORMAL_DISPLAY							0xA6
#define SSD1306_INVERT_DISPLAY							0xA7
#define SSD1306_DISPLAY_OFF								0xAE
#define SSD1306_DISPLAY_ON								0xAF
#define SSD1306_NOP										0xE3
// Scrolling Commands
#define SSD1306_HORIZONTAL_SCROLL_RIGHT					0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT					0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT	0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT		0x2A
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3
// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN						0x00
#define SSD1306_SET_HIGHER_COLUMN						0x10
#define SSD1306_MEMORY_ADDR_MODE						0x20
#define SSD1306_SET_COLUMN_ADDR							0x21
#define SSD1306_SET_PAGE_ADDR							0x22
// Hardware Configuration Commands
#define SSD1306_SET_START_LINE							0x40
#define SSD1306_SET_SEGMENT_REMAP						0xA0
#define SSD1306_SET_MULTIPLEX_RATIO						0xA8
#define SSD1306_COM_SCAN_DIR_INC						0xC0
#define SSD1306_COM_SCAN_DIR_DEC						0xC8
#define SSD1306_SET_DISPLAY_OFFSET						0xD3
#define SSD1306_SET_COM_PINS							0xDA
#define SSD1306_CHARGE_PUMP								0x8D
// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO				0xD5
#define SSD1306_SET_PRECHARGE_PERIOD					0xD9
#define SSD1306_SET_VCOM_DESELECT						0xDB

void I2C_Start();
void I2C_Stop();
void I2C_SendByte(uint8_t cAddress, uint8_t cByte);
void I2C_SendPacket(uint8_t * cPacket, uint8_t cSize);
void DISP_Command(uint8_t uCommand);
void DISP_Data(uint8_t uData);
void drawPixel(uint16_t x, uint8_t y, uint8_t MODE);
void updateScreen(uint8_t y);
void DISP_BufferData(uint8_t * uData, uint16_t uSize);
void clearScr();
void setColumnData(uint16_t col, uint8_t data);
void DisplayInit();
#endif /* I2C_DISPLAY_H_ */
/*
 * FontLib.h
 *
 * Created: 21.02.2019 22:11:00
 *  Author: admin
 */ 


#ifndef FONTLIB_H_
#define FONTLIB_H_

#define CHAR_COUNT 256
#define CHAR_HEIGHT 8

void drawString(uint16_t xStart, char * str);
void drawStringCentered(char * str);
void drawStringRight(char * str);

#endif /* FONTLIB_H_ */
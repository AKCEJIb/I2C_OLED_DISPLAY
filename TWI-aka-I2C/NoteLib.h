/*
 * notes.h
 *
 * Created: 22.02.2019 12:45:23
 *  Author: admin
 */ 

#include <stdint.h>
#include <avr/pgmspace.h>

#ifndef NOTES_H_
#define NOTES_H_

#define E1 F_CPU/(1024*2)/329
#define G1 F_CPU/(1024*2)/392
#define A1 F_CPU/(1024*2)/440
#define A1d F_CPU/(1024*2)/466
#define H1 F_CPU/(1024*2)/493
#define C2 F_CPU/(1024*2)/523
#define C2d F_CPU/(1024*2)/554
#define D2 F_CPU/(1024*2)/587
#define D2d F_CPU/(1024*2)/622
#define E2 F_CPU/(1024*2)/659
#define F2 F_CPU/(1024*2)/698
#define F2d F_CPU/(1024*2)/739
#define G2 F_CPU/(1024*2)/784
#define A2 F_CPU/(1024*2)/880
#define G2d F_CPU/(1024*2)/830
#define A2d F_CPU/(1024*2)/932
#define C3 F_CPU/(1024*2)/1046
#define P 0
#define Dur 1

const uint8_t alarmSong[] PROGMEM = {
	E1, G1, E1, G1, E1, G1, A1, G1, A1, E1, G1, E1, A1, G1, G2, A2, A1, E1, E2, A1, G1, P, P, P, P
};


#endif /* NOTES_H_ */
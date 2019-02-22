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


#define E1 12134
#define G1 10204
#define A1 9090
#define A1d 8580
#define H1 8099
#define C2 7648
#define C2d 7220
#define D2 6814
#define D2d 6430
#define E2 6069
#define F2 5730
#define G2 5102
#define A2 4545
#define G2d 4815
#define A2d 4291
#define C3 3822
#define P 0
#define End 1

const uint16_t mario[45] PROGMEM = {E2,E2,P,E2,P,C2,E2,P,
	G2,P,P,G1,P,P,
	C2,P,P,G1,P,P,E1,P,
	P,A1,P,H1,P,A1d,A1,P,
	G1,E2,G2,A2,P,F2,G2,
P,E2,P,C2,D2,H1,End};


#endif /* NOTES_H_ */
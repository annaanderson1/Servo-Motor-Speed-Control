/*
 * 	shared.h
 *
 * 	Created: 2019-11-19 15:23:48
 *  Author: Joakim Cedergren
 *
 * 	Contains global variables and #define to be included in all .c files.
 *
 */


#ifndef SHARED_H_
#define SHARED_H_

#define F_CPU 1000000UL
#define BAUD 2400

#ifdef MAIN_FILE
	unsigned int AB;
	unsigned int pwm;
	char recieved_bytes[5];
	bool newCommand;
	uint16_t clk_prev;
	uint16_t clk_elapsed;
	int speed_set;
	int speed_actual;
	bool newMeasurement;

#else
	extern unsigned int AB;
	extern unsigned int pwm;
	extern char recieved_bytes[5];
	extern bool newCommand;
	extern uint16_t clk_prev;
	extern uint16_t clk_elapsed;
	extern int speed_set;
	extern int speed_actual;
	bool newMeasurement;

#endif
#endif
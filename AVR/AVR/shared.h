/*
 * shared.h
 *
 * Created: 2019-11-19 15:23:48
 *  Author: tmk19jc
 */ 


#ifndef SHARED_H_
#define SHARED_H_

#define F_CPU 1000000UL
#define BAUD 2400

#ifdef MAIN_FILE
	unsigned int AB;
	unsigned int pwm;
	int speed_set;
	int speed_actual;
	char recieved_bytes[5];
	bool newCommand;

#else
	extern unsigned int AB;
	extern unsigned int pwm;
	extern char recieved_bytes[5];
	extern bool newCommand;
	extern int speed_set;
	extern int speed_actual;

#endif
#endif 
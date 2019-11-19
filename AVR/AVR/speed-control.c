/*
 * speed-control.c
 *
 * Created: 2019-11-19 10:30:50
 *  Author: tmk19jc
 */ 

#define F_CPU 1000000UL
#define BAUD 2400

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Global variables */
extern unsigned int AB;
extern unsigned int pwm;
extern char recieved_bytes[5];
extern bool newCommand;
extern int speed;

/* Routine for clockwise rotation of encoder */
static void clockwise(){
	turnOff_C(PC2);
	turnOn_C(PC0);
	if(pwm < 245){
		pwm += 10;
	}
	else{
		pwm = 255;
	}
}

/* Routine for counterclockwise rotation of encoder */
static void counterclockwise(){
	turnOff_C(PC0);
	turnOn_C(PC2);
	if(pwm > 10){
		pwm -= 10;
	}
	else{
		pwm = 0;
	}
}


/* ISR for PCINT14-8 */
ISR(PCINT1_vect){
	cli();
	unsigned int ABnew = 0x00;
	unsigned int A = 0x00;
	unsigned int B = 0x00;
	
	A = (PINC & (1 << PC5));
	A = (A >> (PC5 - 1));	// Sets A in pos 1
	
	B = (PINC & (1 << PC4));
	B = (B >> PC4);			// Sets B in pos 0
	
	ABnew = A | B;
	
	switch(ABnew){
		case 0:
		if(AB == 2){
			clockwise();
		}
		else{
			counterclockwise();
		}
		break;
		
		case 1:
		if(AB == 0){
			clockwise();
		}
		else{
			counterclockwise();
		}
		break;
	}
	AB = ABnew;
	sei();
}
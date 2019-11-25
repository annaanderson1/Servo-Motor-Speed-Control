/*
 * 	speed-control.c
 *
 * 	Created: 2019-11-19 10:30:50
 *  Author: Joakim Cedergren
 */

#include <stdbool.h>
#include <inttypes.h>
#include "shared.h"

#include <avr/io.h>
#include <avr/interrupt.h>



/* Turns off output on selected pin on PORTC */
void turnOff_C(int pin){
	PORTC &= ~(1 << pin);

}
/* Turns on output on selected pin on PORTC */
void turnOn_C(int pin){
	PORTC |= (1 << pin);
}

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

/* Calculates the speed for a single encoder-interrupt. */
static int calc_curr_speed(int time_elapsed){

	return 0;
}

/* Calculates the filtered speed and stores the value i global speed_actual */
void calc_filtered_speed(){

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
	/* Pseudo code framework for speed measurement-interrupts

	time_elapsed = time_now - time_prev;
	time_now = time_prev;
	speed_measured[speed_measured_pos] = calc_curr_speed(time_elapsed);
	speed_measured_pos++;
	if(speed_measured_pos == SPEED_AVG_SIZE){
		speed_measured_pos = 0;
	}
	*/
	
	uint16_t clk_curr = 0x0000;
	clk_curr |= (TCNT1H << 8) | TCNT1L;
	
	// Case: first measurement
	if(clk_prev == 0){
		clk_prev = clk_curr;
		clk_elapsed = clk_curr;
	}
	// Case: clock overflow
	else if(clk_curr <= clk_prev){
		uint16_t temp = 0xFFFF;
		temp = temp - clk_prev;
		clk_elapsed = temp + clk_curr;
	}
	else{
		clk_elapsed = clk_curr - clk_prev;
	}
	newMeasurement = true;
	
	speed_actual = TCNT1L;
	sei();
}
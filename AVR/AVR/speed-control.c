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

extern bool newMeasurement;
extern unsigned short clk_curr;
extern unsigned short clk_prev;
extern unsigned short clk_elapsed;

/*	Converts the difference in clk increments to microseconds */
static unsigned long calc_delta_time(Shared_Data* shared_ptr){
	unsigned short delta_clk = clk_elapsed;

	// scales based on prescaling
	unsigned short prescale = 8;

	unsigned long temp;

	temp = (unsigned long)delta_clk * prescale;

	return temp;
}

/* Inserts calculated rpm to first pos in rpm-array, and shifts the rest to the right.
 * Filters outliers (0 < rpm < 130)
*/
static void insert_rpm(Shared_Data* shared_ptr, unsigned long rpm){

    int i;
    unsigned long temp;

    temp = rpm >> N;

    if(temp < 0 || temp > 150){
        return;
    }

    for(i = MEASUREMENTS_SIZE - 1; i >= 0; i--){
        shared_ptr->rpm_measurements[i] = shared_ptr->rpm_measurements[i-1];
    }
    shared_ptr->rpm_measurements[0] = rpm;

}

/*	Calculates the speed between two encoder-interrupts, using fixed point arithmetics.
 *	Qm.n values defined in shared.h
*/
void calc_latest_rpm(Shared_Data* shared_ptr){
	unsigned long delta_time;
	unsigned short delta_rev_inverse = 96;
	unsigned long MS_TO_S = 1000000;
	unsigned short S_TO_MIN = 60;
	unsigned long long numerator;
	unsigned long long denominator;
	unsigned long long rpm;
	
	delta_time = calc_delta_time(shared_ptr);
	
	delta_time = delta_time << N;
	delta_rev_inverse = delta_rev_inverse << N;
	MS_TO_S = MS_TO_S << N;
	S_TO_MIN = S_TO_MIN << N;

	numerator = (unsigned long long)MS_TO_S * S_TO_MIN;
	numerator = numerator >> N;

	denominator = (unsigned long long)delta_rev_inverse * delta_time;
	denominator = denominator >> N;

	rpm = numerator << N;
	rpm = rpm + (denominator >> 1);
	rpm = rpm / denominator;
	
	shared_ptr->delta_time = delta_time >> N;		// Used for debugging
	shared_ptr->curr_rpm = (unsigned long)rpm >> N;	// Used for debugging
	insert_rpm(shared_ptr, rpm);
	
}


void calc_avg_rpm(Shared_Data* shared_ptr){
	unsigned long long temp = 0;
	int i;
	
	for(i = 0; i < MEASUREMENTS_SIZE; i++){
		temp = temp + shared_ptr->rpm_measurements[i];
	}
	
	// Divide by MEASUREMENTS_SIZE (64)
	temp = temp >> 5;
	
	// convert back from Qm.n to normal int
	temp = temp >> N;
	shared_ptr->rpm_avg = temp;
	
}

static void calc_clk_elapsed(){
	
	if(clk_curr < clk_prev){
		unsigned short temp = 0xFFFF;
		temp = temp - clk_prev;
		temp = temp + clk_curr;
		clk_elapsed = temp;
	}
	else{
		clk_elapsed = clk_curr - clk_prev;
		
	}
	clk_prev = clk_curr;
}

ISR(PCINT1_vect){
	cli();
	
	clk_curr = TCNT1;
	calc_clk_elapsed();
	newMeasurement = true;
	
	sei();
}
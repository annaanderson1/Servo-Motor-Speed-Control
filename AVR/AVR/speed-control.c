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

/*	Converts the difference in clk increments to microseconds */
static uint32_t calc_delta_time(){

	uint16_t delta_clk = clk_elapsed;

	// scales based on prescaling
	uint16_t prescale = 8;

	uint32_t temp;

	temp = (uint32_t)delta_clk * prescale;

	return temp;
}

/* Inserts calculated rpm to first pos in rpm-array, and shifts the rest to the right.
 * Filters outliers (0 < rpm < 130)
*/
static void insert_rpm(uint32_t rpm){

    int i;
    /*uint32_t temp;

    temp = rpm >> N;

    if(temp < 0 || temp > 130){
        return;
    }
*/
    for(i = MEASUREMENTS_SIZE - 1; i >= 0; i--){
        rpm_measurements[i] = rpm_measurements[i-1];
    }
    rpm_measurements[0] = rpm;

}

/*	Calculates the speed for a single encoder-interrupt, using fixed point arithmetics.
 *	Qm.n values defined in shared.h
*/
void calc_latest_rpm(){
	uint32_t delta_time = clk_elapsed*8;
	uint16_t delta_rev_inverse = 96;
	uint64_t MS_TO_S = 1000000;
	uint16_t S_TO_MIN = 60;
	uint64_t numerator;
	uint64_t denominator;
	uint64_t rpm;
	
	test_var2 = delta_time;
	
	delta_time = delta_time << N;
	delta_rev_inverse = delta_rev_inverse << N;
	MS_TO_S = MS_TO_S << N;
	S_TO_MIN = S_TO_MIN << N;

	numerator = (uint64_t)MS_TO_S * S_TO_MIN;
	numerator = numerator >> N;

	denominator = (uint64_t)delta_rev_inverse * delta_time;
	denominator = denominator >> N;

	rpm = numerator << N;
	rpm = rpm + (denominator >> 1); // For correct rounding
	rpm = rpm / denominator;
	
	uint32_t rpm32 = (uint32_t)rpm;
	rpm32 = rpm32 >> N;
	curr_rpm = rpm32;
	
	test_var1 = denominator;
	
	
	
	insert_rpm(rpm32);
	
}


void calc_avg_rpm(){
	uint64_t temp = 0;
	int i;
	
	for(i = 0; i < MEASUREMENTS_SIZE; i++){
		temp = temp + rpm_measurements[i];
	}
	
	// Divide by MEASUREMENTS_SIZE (64)
	temp = temp >> 6;
	
	// convert back from Qm.n to normal int
	temp = temp >> N;
	rpm_avg = temp;
	
}

void calc_time_elapsed(){
	
	if(clk_curr < clk_prev){
		uint16_t temp = 0xFFFF;
		temp = temp - clk_prev;
		temp = temp + clk_curr;
		clk_elapsed = temp;
	}
	else{
		clk_elapsed = clk_curr - clk_prev;
		
	}
	clk_prev = clk_curr;
}

/* ISR for PCINT14-8 */
ISR(PCINT1_vect){
	cli();
	
	clk_curr = TCNT1;
	newMeasurement = true;
	
	sei();
}
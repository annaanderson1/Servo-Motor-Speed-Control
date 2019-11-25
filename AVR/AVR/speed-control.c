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

/*	Converts the difference in clk increments to milliseconds, using fixed point arithmetics.
 *	Qm.n values defined in shared.h
*/ 
static uint16_t calc_delta_time(){
	
	uint16_t delta_clk = clk_elapsed;
	delta_clk = delta_clk << N;
	
	// 1 MHz F_CPU equals to 1 ms per clk increment
	uint16_t freq = 1000;
	freq = freq << N;

	uint32_t temp;

	temp = (uint32_t)delta_clk * freq;
	temp = temp >> N;
	return temp;
}

/* Inserts calculated rpm to first pos in rpm-array, and shifts the rest to the right */ 
static void insert_rpm(uint32_t rpm){
	
	// Case: array is full
	if(pos_last_rpm == (MEASUREMENTS_SIZE - 1) ){
		int i;
		for(i = MEASUREMENTS_SIZE - 1; i >= 0; i--){
			rpm_measurements[i] = rpm_measurements[i-1];
		}
		rpm_measurements[0] = rpm;
		
	}
	// Case: array not yet full
	else {
		rpm_measurements[pos_last_rpm + 1] = rpm;
		pos_last_rpm++;
	}
	
}

/*	Calculates the speed for a single encoder-interrupt, using fixed point arithmetics.
 *	Qm.n values defined in shared.h
*/
void calc_latest_rpm(){
	uint16_t delta_time = calc_delta_time();
	uint16_t delta_rev_inverse = 96;
	uint16_t MS_TO_S = 1000;
	uint16_t S_TO_MIN = 60;
	uint32_t numerator;
	uint32_t denominator;
	uint64_t rpm;
		
	delta_rev_inverse = delta_rev_inverse << N;
	MS_TO_S = MS_TO_S << N;
	S_TO_MIN = S_TO_MIN << N;

	numerator = (uint32_t)MS_TO_S * S_TO_MIN;
	numerator = numerator >> N;

	denominator = (uint32_t)delta_rev_inverse * delta_time;
	denominator = denominator >> N;

	rpm = numerator << N;
	rpm = rpm + (denominator >> 1); // For correct rounding
	rpm = rpm / denominator;
	
	
	test_var1 = denominator;
	test_var2 = delta_time;
	curr_rpm = rpm >> N;
	
	//test_var3 = rpm >> N;
	
	insert_rpm(rpm);
	
}

/* Calculates the filtered speed and stores the value i global speed_actual */
void calc_filtered_speed(){

}

void calc_avg_rpm(){
	uint64_t temp = 0;
	int i;
	
	for(i = 0; i < MEASUREMENTS_SIZE; i++){
		temp = temp + rpm_measurements[i];
	}
	
	// Devide by MEASUREMENTS_SIZE (64)
	temp = temp >> 6;
	
	// convert back from Qm.n to normal int
	temp = temp >> N;
	rpm_avg = temp;
	
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
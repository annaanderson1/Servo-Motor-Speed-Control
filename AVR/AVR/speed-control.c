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

static void set_prescale(int ps){
	if (ps == 1){
		TCCR1B &= ~ 0xff;
		TCCR1B = (1 << CS10);
	}
	else if(ps == 8){
		TCCR1B &= ~ 0xff;
		TCCR1B = (1 << CS11);
	}
}
/*	Converts the difference in clk increments to microseconds */
static unsigned long calc_delta_time(Shared_Data* shared_ptr){
	unsigned short delta_clk = clk_elapsed;

	// scales based on prescaling
	unsigned short prescale;
	
	if(shared_ptr->speed_set <= 20){
		prescale = 8;
		set_prescale(8);
	}
	else{
		prescale = 1;
		set_prescale(1);
	}

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

    if(temp < 0 || temp > 250 ){
        return;
    }

    for(i = MEASUREMENTS_SIZE - 1; i >= 0; i--){
        shared_ptr->rpm_measurements[i] = shared_ptr->rpm_measurements[i-1];
    }
    shared_ptr->rpm_measurements[0] = rpm;

}

static void update_fine_tuning(Shared_Data* shared_ptr){
	short fine_tuning;
	
	fine_tuning = ADCL;	// value: 0-1024
	fine_tuning |= (ADCH << 8);
	TIFR1 |= (1 << TOV0);	// clear timer1 overflow flag
	fine_tuning = (fine_tuning << 1);   // value: 0 - 2048
	fine_tuning = fine_tuning - 1024;   // value: -1028 - 1028
	fine_tuning = fine_tuning/100;		// value: -10 - 10
	shared_ptr->fine_tuning = fine_tuning;	// for debuggning
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
	int size;
	int size_shift;
	
	
	if(shared_ptr->speed_set <=20){
		size = 16;
		size_shift = 4;
	}
	else if(shared_ptr->speed_set <=50){
		size = 64;
		size_shift = 6;
	}
	else if(shared_ptr->speed_set <= 100){
		size = 64;
		size_shift = 6;
	}
	else if(shared_ptr->speed_set > 100){
		size = 64;
		size_shift = 6;
	}
	
	for(i = 0; i < size; i++){
		temp = temp + shared_ptr->rpm_measurements[i];
	}
	
	// Divide by MEASUREMENTS_SIZE (32)
	temp = temp >> size_shift;
	
	// convert back from Qm.n to normal int
	//temp = temp >> N;
	shared_ptr->rpm_avg = temp;
	
}


void control(Shared_Data* shared_ptr){
	long Kp;
	long Ki;
	
	update_fine_tuning(shared_ptr);
	
	long e = ((long)shared_ptr->speed_set << N) - (long)shared_ptr->rpm_avg;
	e = e + ((long)shared_ptr->fine_tuning << N);
	shared_ptr->error = (short)e;	// For debugging
	
	e = e << (N_CTRL-N);
	
	if(shared_ptr->speed_set >= 100){
		Kp = 100;
		Ki = 500;
	}
	else if(shared_ptr->speed_set >= 80){
		Kp = 100;
		Ki = 550;
	}
	else if(shared_ptr->speed_set >= 10){
		Kp = 100;
		Ki = 1000;
	}
	else{
		Kp = 100;
		Ki = 550;
	}
	
	long integral = Ki*e;
	integral = integral >> N_CTRL;
	shared_ptr->integral = shared_ptr->integral + integral;
	
	long long pwm = (long long)e * Kp + shared_ptr->integral;
	pwm = pwm >> N_CTRL;
	
	
	pwm = pwm >> N_CTRL;	// Convert to regular number

	
	if(pwm < 0){
		pwm = 0;		
	}
	else if(pwm > 255){
		pwm = 255;
	}
	shared_ptr->speed_actual = (int)pwm;
	OCR0A = pwm;
	OCR0B = pwm;
	
}

ISR(PCINT1_vect){
	cli();
	
	clk_curr = TCNT1;
	calc_clk_elapsed();
	newMeasurement = true;
	
	sei();
}
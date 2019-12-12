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
#include <string.h>
#include <stdlib.h>

#define SIZE_16 16
#define SIZE_32 32
#define SIZE_64 64
#define SIZE_128 128
#define DIVISION_16 4
#define DIVISION_32 5
#define DIVISION_64 6
#define DIVISION_128 7

extern bool newMeasurement;
extern unsigned short clk_curr;
extern unsigned short clk_prev;
extern unsigned short clk_elapsed;
extern char recieved_bytes[5];
extern Shared_Data* shared_ptr;

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

/* 
 *
*/
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

/* 
 * Inserts calculated rpm to first pos in rpm-array, and shifts the rest to the right.
 * Filters outliers (0 < rpm || rpm > 250)
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

/*
 * Updates the fine tuning value to first calculation since last function call.
 * Stores the value in shared_ptr->fine_tuning
*/
void update_fine_tuning(Shared_Data* shared_ptr){
	short fine_tuning;
	
	fine_tuning = ADCL;	
	fine_tuning |= (ADCH << 8);	// value: 0-1024
	TIFR1 |= (1 << TOV0);	// clear timer1 overflow flag
	
	fine_tuning = (fine_tuning << 1);   // value: 0 - 2048
	fine_tuning = fine_tuning - 1024;   // value: -1028 - 1028
	fine_tuning = fine_tuning/100;		// value: -10 - 10
	
	shared_ptr->fine_tuning = fine_tuning;
}

/*	
 * Calculates the speed between two encoder-interrupts, using fixed point arithmetics.
 * Inserts the speed to the rpm-array in shared_ptr->rpm_measurements
 * Qm.n values defined in shared.h
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
	shared_ptr->rpm_curr = (unsigned long)rpm >> N;	// Used for debugging
	insert_rpm(shared_ptr, rpm);
	
}

/*
 * Calculates the filtered running average rpm. Stores result in shared_ptr->rpm_avg
 * Number of elements counted depends on speed_set
*/
void calc_avg_rpm(Shared_Data* shared_ptr){
	unsigned long long temp = 0;
	int i;
	int size;
	int size_shift;
	
	if(shared_ptr->speed_set <=20){
		size = SIZE_32;
		size_shift = DIVISION_32;
	}
	else if(shared_ptr->speed_set <=50){
		size = SIZE_32;
		size_shift = DIVISION_32;
	}
	else if(shared_ptr->speed_set < 100){
		size = SIZE_32;
		size_shift = DIVISION_32;
	}
	else if(shared_ptr->speed_set >= 100){
		size = SIZE_32;
		size_shift = DIVISION_32;
	}/*
	else if(shared_ptr->speed_set >= 100){
		size = SIZE_32;
		size_shift = DIVISION_32;
	}*/
	else{
		size = SIZE_32;
		size_shift = DIVISION_32;
	}
	
	
	for(i = 0; i < size; i++){
		temp = temp + shared_ptr->rpm_measurements[i];
	}
	
	// Divide by size
	temp = temp >> size_shift;
	
	shared_ptr->rpm_avg = temp;
}

/* Sets a new set value for the speed. Range: 0-120. */
void set_speed(Shared_Data* shared_ptr){
	char sub_str[4];
	int res;
	strncpy(sub_str, recieved_bytes + 1, 3);
	
	res = atoi(sub_str);
	shared_ptr->speed_set = res;
}

/*
 * Interrupt Service Routine for the encoder mounted on the motor.
 * Calculates the difference in clk-value between current and previous interrupt.
 * Global bool newMeasurement flags that a new measurement have been recieved.
*/
ISR(PCINT1_vect){
	cli();
	
	clk_curr = TCNT1;
	calc_clk_elapsed();
	newMeasurement = true;
	
	sei();
}

/*
 * PI controller for the electrical motor, implemented using fixed point arithmetics.
 * Kp and Ki values is used to tune the controller, depending on different rpm.
*/
ISR(TIMER2_OVF_vect){
	
	cli();
	long Kp;
	long Ki;
		
	long e = ((long)shared_ptr->speed_set << N) - (long)shared_ptr->rpm_avg;
	e = e + ((long)shared_ptr->fine_tuning << N);
	shared_ptr->error = (short)e;	// For debugging
	
	e = e << (N_CTRL-N);
	/*
	if(shared_ptr->speed_set <= 20){
		Kp = 130;
		Ki = 117;
	}
	else if(shared_ptr->speed_set <= 50){
		Kp = 35;
		Ki = 75;
	}
	else if(shared_ptr->speed_set <= 60){
		Kp = 35;
		Ki = 75;
	}
	else if(shared_ptr->speed_set <= 90){
		Kp = 40;
		Ki = 100;
	}
	else if(shared_ptr->speed_set < 100){
		Kp = 35;
		Ki = 70;
	}
	else if(shared_ptr->speed_set > 20){
		Kp = 130;	//80 165
		Ki = 117;	//145 325
	}
*/
	Kp = 130;	//175
	Ki = 117;	// 300
	
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
	
	shared_ptr->pwm = (short)pwm;
	OCR0A = pwm;
	OCR0B = pwm;
	sei();
}
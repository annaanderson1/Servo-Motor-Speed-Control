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
#define N 4
#define MEASUREMENTS_SIZE 32
#define N_CTRL 7

typedef struct {
	short speed_set;
	int speed_actual;
	unsigned short clk_prev;
	unsigned short clk_elapsed;
	unsigned long curr_rpm;
	unsigned long rpm_avg;
	unsigned long rpm_measurements[MEASUREMENTS_SIZE];
	unsigned long delta_time;
	long long integral;
	short error;
	short fine_tuning;
} Shared_Data;

void init_shared_data();

#endif
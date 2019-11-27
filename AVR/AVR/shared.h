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
#define N 5
#define MEASUREMENTS_SIZE 64

typedef struct {
	int speed_set;
	int speed_actual;
	uint16_t clk_prev;
	uint16_t clk_elapsed;
	uint32_t curr_rpm;
	uint32_t rpm_avg;
	uint32_t rpm_measurements[MEASUREMENTS_SIZE];
	uint32_t delta_time;
} Shared_Data;

void init_shared_data();

#endif
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
#define MEASUREMENTS_SIZE 128
#define N_CTRL 7


/*
 * Struct containing all shared data, excluding globals that needs to be accessed in ISR
*/
typedef struct {
	short speed_set;
	short pwm;
	short error;
	short fine_tuning;
	long long integral;
	unsigned long rpm_curr;
	unsigned long rpm_avg;
	unsigned long rpm_measurements[MEASUREMENTS_SIZE];
	unsigned long delta_time;
} Shared_Data;

/*
 * Initiates the struct variables.
*/
void init_shared_data();

#endif
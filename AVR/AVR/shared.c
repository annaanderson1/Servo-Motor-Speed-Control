/*
 *	shared.c
 *
 *	Created: 2019-11-27 16:00:18
 *  Author: Joakim Cedergren
 *
 *	Contains global variables and #define to be included in all .c files.
 *
 */ 

#include <stdbool.h>
#include <inttypes.h>
#define MEASUREMENTS_SIZE 128


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
void init_shared_data(Shared_Data* shared_ptr){
	shared_ptr->speed_set = 0;
	shared_ptr->pwm = 0;
	shared_ptr->error = 0;
	shared_ptr->fine_tuning = 0;
	shared_ptr->integral = 0;
	shared_ptr->rpm_curr = 0;
	shared_ptr->rpm_avg = 0;
	shared_ptr->delta_time = 0;
}
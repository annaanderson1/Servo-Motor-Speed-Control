/*
 * shared.c
 *
 * Created: 2019-11-27 16:00:18
 *  Author: joakimcedergren
 */ 

#include <stdbool.h>
#include <inttypes.h>
#define MEASUREMENTS_SIZE 64

typedef struct {
	int speed_set;
	int speed_actual;
	unsigned short clk_prev;
	unsigned short clk_elapsed;
	unsigned long curr_rpm;
	unsigned long rpm_avg;
	unsigned long rpm_measurements[MEASUREMENTS_SIZE];
	unsigned long delta_time;
} Shared_Data;

void init_shared_data(Shared_Data* shared_ptr){
	shared_ptr->speed_set = 0;
	shared_ptr->speed_actual = 0;
	shared_ptr->clk_prev = 0;
	shared_ptr->clk_elapsed = 0;
	shared_ptr->curr_rpm = 0;
	shared_ptr->rpm_avg = 0;
	shared_ptr->delta_time = 0;
}
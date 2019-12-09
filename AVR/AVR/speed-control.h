/*
 * speed-control.h
 *
 * Created: 2019-11-19 10:31:14
 *  Author: tmk19jc
 */ 


#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_

void calc_latest_rpm(Shared_Data* shared_ptr);
void calc_filtered_speed(Shared_Data* shared_ptr);
void calc_avg_rpm(Shared_Data* shared_ptr);
void set_speed(Shared_Data* shared_ptr);
void control(Shared_Data* shared_ptr);

#endif
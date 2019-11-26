/*
 * speed-control.h
 *
 * Created: 2019-11-19 10:31:14
 *  Author: tmk19jc
 */ 


#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_

void calc_latest_rpm();
void calc_filtered_speed();
void calc_avg_rpm();
void calc_time_elapsed();
#endif
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

#ifdef MAIN_FILE
	unsigned int AB;
	unsigned int pwm;
	char recieved_bytes[5];
	bool newCommand;
	uint16_t clk_prev;
	uint16_t clk_curr;
	uint16_t clk_elapsed;
	int speed_set;
	int speed_actual;
	bool newMeasurement;
	uint32_t curr_rpm;
	uint32_t rpm_measurements[MEASUREMENTS_SIZE];
	int pos_last_rpm;
	uint32_t rpm_avg;
	uint32_t test_var1;
	uint32_t test_var2;
	uint64_t test_var3;
	uint32_t delta_time;

#else
	extern unsigned int AB;
	extern unsigned int pwm;
	extern char recieved_bytes[5];
	extern bool newCommand;
	extern uint16_t clk_prev;
	extern uint16_t clk_curr;
	extern uint16_t clk_elapsed;
	extern int speed_set;
	extern int speed_actual;
	bool newMeasurement;
	extern uint32_t curr_rpm;
	extern uint32_t rpm_measurements[MEASUREMENTS_SIZE];
	extern int pos_last_rpm;
	extern uint32_t rpm_avg;
	extern uint32_t test_var1;
	extern uint32_t test_var2;
	extern uint64_t test_var3;
	extern uint32_t delta_time;
	
#endif
#endif
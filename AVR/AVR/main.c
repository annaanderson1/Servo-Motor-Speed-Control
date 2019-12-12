/*
 * main.c
 *
 * Created: 2019-10-15 12:33:13
 *  Author: tmk19jc
 */ 


#include <stdbool.h>
#include <inttypes.h>
#include "shared.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "registers.h"
#include "serial.h"
#include "speed-control.h"
#include "transmit.h"

/* Globals */
bool newCommand;
bool newMeasurement;
char recieved_bytes[5];
unsigned short clk_curr;
unsigned short clk_prev;
unsigned short clk_elapsed;
Shared_Data* shared_ptr;

int main(void){
	Shared_Data shared_data;
	//Shared_Data* shared_ptr = &shared_data;
	shared_ptr = &shared_data;
	init_shared_data(shared_ptr);
	
	newCommand = false;
	newMeasurement = false;
	clk_curr = 0;
	clk_prev = 0;
	clk_elapsed = 0;
	shared_ptr->speed_set = 50;
	
	setup_registers();
	sei();
	
	while(1){
		
		if(newCommand){
			switch(*recieved_bytes){
				case '0':
					set_speed(shared_ptr);
					transmit_empty();
					break;
				case '1':
					transmit_rpm_avg(shared_ptr);
					break;
				case '2':
					transmit_fine_tuning(shared_ptr);
					break;
				case '3':
					transmit_error(shared_ptr);
					break;
				case '4':
					transmit_pwm(shared_ptr);
					break;
				case '5':
					transmit_speed_set(shared_ptr);
					break;
				case '6':
					transmit_rpm_curr(shared_ptr);
					break;
				case '7':
					transmit_clk_elapsed(shared_ptr);
					break;
				case '8':
					transmit_delta_time(shared_ptr);
					break;
				default:
					transmit_empty();
			}
			newCommand = false;
		}
		
		if(newMeasurement){
			calc_latest_rpm(shared_ptr);
			calc_avg_rpm(shared_ptr);
			update_fine_tuning(shared_ptr);
			newMeasurement = false;
		}
	}
	return 0;
}
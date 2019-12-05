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

int main(void){
	Shared_Data shared_data;
	Shared_Data* shared_ptr = &shared_data;
	init_shared_data(shared_ptr);
	
	newCommand = false;
	newMeasurement = false;
	clk_curr = 0;
	clk_prev = 0;
	clk_elapsed = 0;
	shared_ptr->speed_set = 110;
	
	setup_registers();
	sei();
	
	while(1){
		
		if(newCommand){
			switch(*recieved_bytes){
				case '0':
					transmit_0(shared_ptr);
					break;
				case '1':
					transmit_1(shared_ptr);
					break;
				case '2':
					transmit_2(shared_ptr);
					break;
				case '3':
					transmit_3(shared_ptr);
					break;
				case '4':
					transmit_4(shared_ptr);
					break;
				case '5':
					transmit_5(shared_ptr);
					break;
				case '6':
					transmit_6(shared_ptr);
					break;
				case '7':
					transmit_7(shared_ptr);
					break;
				case '8':
					transmit_8(shared_ptr);
					break;
				case '9':
					transmit_9(shared_ptr);
					break;
				default:
					transmit_empty();
			}
			newCommand = false;
		}
		if(newMeasurement){
			calc_latest_rpm(shared_ptr);
			calc_avg_rpm(shared_ptr);
			control(shared_ptr);
			newMeasurement = false;
		}
	}
	return 0;
}
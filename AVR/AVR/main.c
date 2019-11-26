/*
 * speed-control.c
 *
 * Created: 2019-10-15 12:33:13
 *  Author: tmk19jc
 */ 

#define MAIN_FILE

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

int main(void){
	AB = 0;
	pwm = 50;
	speed_set = 0;
	speed_actual = 0;
	newCommand = false;
	newMeasurement = false;
	pos_last_rpm = 0;
	rpm_avg = 0;
	clk_curr = 0;
	clk_prev = 0;

	setup_registers();
	sei();
	//OCR0B = 20;
	//OCR0A = 20;
	while(1){
		char buf[6];
		char sub_str[4];
		char *endptr;
		int val;

		if(newCommand){
			memset(buf,' ', 6*sizeof(char));
			memset(sub_str,'%', 4*sizeof(char));
			val = 0;
			
			switch(*recieved_bytes){
				case '0':
					break;
				case '1':
					break;
				case '2':
					strncpy(sub_str, recieved_bytes + 1, 3);
					strncpy(sub_str + 3, ";", 1);	
					val = strtol(sub_str, &endptr, 10);
					speed_set = val;
					sprintf(buf, sub_str);
					memset(buf,' ', 6*sizeof(char));
					OCR0B = speed_set;
					OCR0A = speed_set;
					break;
				case '3':
					sprintf(buf, "%d", speed_set);
					break;
				case '4':
					sprintf(buf, "%d", speed_actual);
					break;
				case '5':
					sprintf(buf, "%u", clk_elapsed);
					break;
				case '6':
					sprintf(buf, "%u", rpm_avg);
					break;
				case '7':
					sprintf(buf, "%u", test_var1);
					break;
				case '8':
					sprintf(buf, "%u", test_var2);
					break;
				case '9':
					sprintf(buf, "%u", curr_rpm);
					break;
			}
			
			USART_transmit(buf);
			memset(buf,' ', 6*sizeof(char));
			newCommand = false;
		}
	
		if(newMeasurement){
			calc_time_elapsed();
			calc_latest_rpm();
			//calc_avg_rpm();
			newMeasurement = false;
		}
		
		
		
	}
	return 0;
}
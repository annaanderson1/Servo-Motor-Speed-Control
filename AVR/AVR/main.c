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
#include "transmit.h"




int main(void){
	AB = 0;
	pwm = 50;
	int speed_set = 50;
	speed_actual = 0;
	newCommand = false;
	newMeasurement = false;
	pos_last_rpm = 0;
	rpm_avg = 0;
	clk_curr = 0;
	clk_prev = 0;
	
	int* speed_set_pnt;
	speed_set_pnt = &speed_set;

	newMeasurement = true;
	clk_elapsed = 10000;

	setup_registers();
	sei();

	//uint16_t test = 60000;

	while(1){
		//char buf[6];
		//char sub_str[4];
		//char temp[30];
		//char *endptr;
		//int val;
		
		if(newCommand){
			//memset(buf,' ', 6*sizeof(char));
			//memset(sub_str,'%', 4*sizeof(char));
			//val = 0;
			
			switch(*recieved_bytes){
				case '0':
					transmit_0();
					break;
				case '1':
					transmit_1();
					break;
				case '2':
					transmit_2(speed_set_pnt);
					break;
				case '3':
					transmit_3(speed_set_pnt);
					break;
				case '4':
					transmit_4();
					break;
				case '5':
					transmit_5();
					break;
				case '6':
					transmit_6();
					break;
				case '7':
					transmit_7();
					break;
				case '8':
					transmit_8();
					break;
				case '9':
					transmit_9();
					break;
				
			}
			
			//USART_transmit(buf);
			//memset(buf,' ', 6*sizeof(char));
			//memset(temp,' ', 30*sizeof(char));
			newCommand = false;
		}
	
		if(newMeasurement){
			//calc_time_elapsed();
			calc_latest_rpm();
			//calc_avg_rpm();
			newMeasurement = false;
		}
		
		
		
	}
	return 0;
}
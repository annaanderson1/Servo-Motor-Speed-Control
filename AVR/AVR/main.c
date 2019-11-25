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

	setup_registers();
	sei();
	
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
					break;
				case '3':
					sprintf(buf, "%d", speed_set);
					break;
				case '4':
					sprintf(buf, "%d", speed_actual);
					break;
				case '5':
					sprintf(buf, "%d", clk_elapsed);
					break;
			}
			
			USART_transmit(buf);
			memset(buf,' ', 6*sizeof(char));
			newCommand = false;
		}
		
	
		
		
	}
	return 0;
}
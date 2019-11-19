/*
 * speed-control.c
 *
 * Created: 2019-10-15 12:33:13
 *  Author: tmk19jc
 */ 


#define F_CPU 1000000UL
#define BAUD 2400
#define FOSC 1000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "registers.h"
#include "serial.h"
#include "speed-control.h"

/* Global variables */
unsigned int AB;
unsigned int pwm;
int speed;
char recieved_bytes[5];
bool newCommand;


int main(void){
	Registers registers;
	Registers *reg = &registers;
	AB = 0;
	pwm = 50;
	speed = 0;
	newCommand = false;

	setup_registers();
	sei();
	
	while(1){
		char buf[6];
		char sub_str[4];
		char *endptr;
		int val;
		
		reg = set_trigger(reg, 0);

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
					speed = val;
					sprintf(buf, sub_str);
					memset(buf,' ', 6*sizeof(char));					
					break;
				case '3':
					sprintf(buf, "%d", speed);
				break;
			}
			
			USART_transmit(buf);
			memset(buf,' ', 6*sizeof(char));
			newCommand = false;
		}
		
	}
	return 0;
}
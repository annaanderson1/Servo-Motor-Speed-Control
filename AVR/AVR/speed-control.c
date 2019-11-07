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
#include "registers.h"

/* Global variables */
unsigned int AB;
unsigned int pwm;


int main(void){
	Registers registers;
	Registers *reg = &registers;
	AB = 0;
	pwm = 0;

	setup_registers();
	//sei();
	
	while(1){
	
		reg = set_trigger(reg, pwm);
		//unsigned char data = 'a';
		unsigned char data = USART_recieve();
		USART_transmit(data);
		_delay_ms(500);
    }
	return 0;
}
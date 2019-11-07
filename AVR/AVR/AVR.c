/*
 * AVR.c
 *
 * Created: 2019-10-15 12:33:13
 *  Author: tmk19jc
 */ 


/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include "registers.h"

// Global variables 
unsigned int AB;
unsigned int pwm;

int main(void){
	Registers registers;
	Registers *reg = &registers;
	AB = 0;
	pwm = 0;

	setup_registers();
	
	while(1){
	
		reg = set_trigger(reg, pwm);
    }
	return 0;
}
*/
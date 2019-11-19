/*
 * registers.c
 *
 * Created: 2019-10-15 12:44:18
 *  Author: tmk19jc
 */ 
#include <stdbool.h>
#include "shared.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>



typedef struct {
	
	unsigned int setValue;
	
} Registers;

/************************************************************************/
/* PRIVATE FUNCTIONS                                                    */
/************************************************************************/

static void setup_DDR(){
	
	DDRB = 0x00;										// no output
	DDRC = (1 << PC2) | (1 << PC1) | (1 << PC0);		// PC0-PC2 output
	DDRD = (1 << PD6) | (1 << PD5) | (1 << PD1);		// PD1, PD5 & PD6 output
	
}

static void setup_PORT(){
		
	PORTB = 0x00;
	PORTC = 0x00;//(1 << PC2) | (1 << PC1) | (1 << PC0);		// Turns on LEDs
	PORTD = 0x00;
	
}

// Sets up PWM on PD6
static void setup_PWM(){
	
	TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00);		// fast PWM, inverting (pg.113, 115)
	TCCR0B =  (0 << CS02) | (0 << CS01) | (1 << CS00);							// no pre scaling  (pg. 117)

}

static void setup_USART(){
	
	DDRD |= (1 << PD1);
	unsigned int ubrr = (((F_CPU / (BAUD * 16UL))) - 1 );
	
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
		
	// Enable reciever for interrupt and transmitter (pg. 202)
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);
	
	// Set frame format: 8 data, 2 stop, 0 parity (pg. 204)
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

}


static void setup_interrupts(){
	
	PCICR = (1 << PCIE1);						// Enables possibility of interrupts on pins 14-8
	PCMSK1 = (1 << PCINT13) | (1 << PCINT12);	// Enables interrupts on pin PC5 & PC4
	
}


/************************************************************************/
/* PUBLIC FUNCTIONS														*/
/************************************************************************/

/* Initial setup of registers */ 
void setup_registers(){
	
	setup_DDR();
	setup_PORT();	
	setup_PWM();
	setup_USART();
	setup_interrupts();
		
}



/* Sets the PWM trigger value*/ 
Registers* set_trigger(Registers* reg, int setValue){
	
	OCR0A = setValue;
	OCR0B = setValue;
	return reg;

}


/************************************************************************/
/* INTERRUPT SERVICE ROUTINES											*/
/************************************************************************/









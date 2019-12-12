/*
 * 	registers.c
 *
 * 	Created: 2019-10-15 12:44:18
 *  Author: Joakim Cedergren
 *
 * 	Used for setting up registers on the ATMega88 for speed controller.
 *
 */

#include <stdbool.h>
#include <inttypes.h>
#include "shared.h"
#include <avr/io.h>

/*
 *	Enables output on the following pins:
 *	PC0-PC2
 *	PC1, PD5-PD6
 *	See details in datasheet pg. 99-100
*/
static void setup_DDR(){

	DDRB |= 0x00;
	DDRC |= (1 << PC2) | (1 << PC1) | (1 << PC0);
	DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD1);

}

/*
 *	Sets up a PWM output on PD5 & PD6.
 *	PWM-mode: fast PWM, inverted, no prescaling.
 *	See details in datasheet pg. 113-117
*/
static void setup_PWM(){

	TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00);
	TCCR0B =  (0 << CS02) | (0 << CS01) | (1 << CS00);

}

/*
 *	Sets up USART for serial communication with the PC master.
 * 	Frame format: 8 data bits, 2 stop bits, 0 parity bits.
 * 	Baud rate according to #define in shared.h
 * 	See details in datasheet pg. 201-205
*/
static void setup_USART(){

	DDRD |= (1 << PD1);
	unsigned int ubrr = (((F_CPU / (BAUD * 16UL))) - 1 );

	// Set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// Enable reciever for interrupt and transmitter
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);

	// Set frame format: 8 data, 2 stop, 0 parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

}

/*
 *	Enables interrupts on pin PC4-PC5
 * 	See details in datasheet pg. 81-82
*/
static void setup_interrupts(){

	// Enables possibility of interrupts on pins 14-8
	PCICR = (1 << PCIE1);
	// Enables interrupts on pin PC4-PC5
	PCMSK1 = (1 << PCINT13) | (1 << PCINT12);

}

/*	
 *	Sets up clk/8 prescaling on 16-bit timer OC1
 *	See details in datasheet pg. 141-147
*/	
static void setup_speed_clock(){
	TCCR1B |= (1 << CS11);
}

/* 
 * Sets up ADC conversion on pin PC3.
 * Auto triggered by timer/counter1 overflow. 
 *		Must clear timer overflow flag to calculate new value, done by:
 *		TIFR1 |= (1 << TOV0);
 * Disabled digital input on pin PC3 for less power usage
 * See details in datasheet pg. 268-272
 */
static void setup_ADC(){
	ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0);
	ADCSRA |= (1 << ADEN) | (1 << ADATE);
	ADCSRB |= (1 << ADTS2) | (1 << ADTS1);
	DIDR0 |= (1 << ADC3D);
	
}

static void setup_control(){
	TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20);
	TIMSK2 |= (1 << TOIE2);
}

/*
 *	Initiates registers for DDR, PWM, USART, interrupts, encoder clock speed, and ADC conversion.
*/
void setup_registers(){

	setup_DDR();
	setup_PWM();
	setup_USART();
	setup_interrupts();
	setup_speed_clock();
	setup_ADC();
	setup_control();

}

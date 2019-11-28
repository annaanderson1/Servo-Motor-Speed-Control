/*
 * 	serial.c
 *
 * 	Created: 2019-11-19 10:27:11
 * 	Author: Joakim Cedergren
 *
 * 	Functions and ISR for the serial communication with the master PC.
 *
 */

#include <stdbool.h>
#include <inttypes.h>
#include "shared.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

extern bool newCommand;
extern char recieved_bytes[5];

/*
 * 	Transmits 5 characters over the USART.
 * 	If number of submitted chars < 5, the empty spots is padded with " "
 * 	*data: pointer to chars to be transmitted
 * 	See details in datasheet pg. 190
*/
void USART_transmit(char *data){
	int i = 0;
	char buf[6];
	
	strcpy(buf,"%%%%%");
	strncpy(buf, data, 5);

	for(i = 0; i < 5; i++){
		// Wait for empty transmit buffer
		while( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = buf[i];
		_delay_ms(5);
	}
}

/*
 * 	Interrupt service routine for the USART reciever.
 *	Listens on the RX pin for 5 bytes, and stores them in
 *	order in the global variable recieved_bytes.
 *	Global bool newCommand flags that a new message has been recieved.
*/
ISR(USART_RX_vect){
	cli();
	PORTC &= ~(1 << PC2);
	PORTC |= (1 << PC1);
	for(int i = 0; i < 5; i++){
		while( !(UCSR0A & (1 << RXC0)) );
		recieved_bytes[i] = UDR0;
	}
	PORTC &= ~(1 << PC1);
	PORTC |= (1 << PC2);
	newCommand = true;
	sei();
}

/*
 * serial.c
 *
 * Created: 2019-11-19 10:27:11
 *  Author: tmk19jc
 */ 

#include <stdbool.h>
#include "shared.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>


// See pg. 190
/* Transmits the data-string over the USART */
void USART_transmit(char *data){
	int i = 0;
	int j = 0;
	char buf[6];
	int data_len = strlen(data);
	
	strncpy(buf, data, data_len);
	
	// Pads data with trailing #
	while( (data_len + i) <= 5){
		int pos = (data_len + i);
		strcpy(buf + pos, " ");
		i++;
	}
	
	for(j = 0; j < 5; j++){
		// Wait for empty transmit buffer
		while( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = buf[j];
		_delay_ms(5);
	}
}

/* ISR for serial receiver */
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

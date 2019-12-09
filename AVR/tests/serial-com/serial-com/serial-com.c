/*
 * serial_com.c
 *	
 * Loops the received char back over the serial com.
 *
 * Created: 2019-12-03 16:06:11
 *  Author: joakimcedergren
 */ 

#define F_CPU 1000000UL
#define BAUD 2400

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

static void setup(){
	DDRD |= (1 << PD1);
	DDRC |= (1 << PC2) | (1 << PC1 | (1 << PC0));
	unsigned int ubrr = (((F_CPU / (BAUD * 16UL))) - 1 );

	// Set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// Enable reciever for interrupt and transmitter
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);

	// Set frame format: 8 data, 2 stop, 0 parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static void USART_transmit(char *data){
	//while( !(UCSR0A & (1 << UDRE0)) );
	UDR0 = data[0];	
	
}

ISR(USART_RX_vect){
	cli();
	PORTC &= ~(1 << PC2);
	PORTC |= (1 << PC1);
	
	unsigned char recieved_byte;
	recieved_byte = UDR0;
	UDR0 = recieved_byte;
	_delay_ms(50);
	PORTC &= ~(1 << PC1);
	PORTC |= (1 << PC2);
	sei();
}


int main(void){
	
	setup();
	PORTC |= (1 << PC0);
	sei();
	char s[5];
	strncpy(s, "t", 1);
	
    while(1){
        USART_transmit(s);
		_delay_ms(500);
    }
}
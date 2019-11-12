/*
 * registers.c
 *
 * Created: 2019-10-15 12:44:18
 *  Author: tmk19jc
 */ 

#define F_CPU 1000000UL
#define BAUD 2400
//#define UBBR (FOSC/(16UL * BAUD))-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Global variables */
extern unsigned int AB;
extern unsigned int pwm;
extern unsigned char recieved_bytes[5];

typedef struct {
	
	unsigned int setValue;
	
} Registers;

/************************************************************************/
/* PRIVATE FUNCTIONS                                                    */
/************************************************************************/

void setup_DDR(){
	
	DDRB = 0x00;										// no output
	DDRC = (1 << PC2) | (1 << PC1) | (1 << PC0);		// PC0-PC2 output
	DDRD = (1 << PD6) | (1 << PD5) | (1 << PD1);		// PD1, PD5 & PD6 output
	
}

void setup_PORT(){
		
	PORTB = 0x00;
	PORTC = 0x00;//(1 << PC2) | (1 << PC1) | (1 << PC0);		// Turns on LEDs
	PORTD = 0x00;
	
}

// Sets up PWM on PD6
void setup_PWM(){
	
	TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00);		// fast PWM, inverting (pg.113, 115)
	TCCR0B =  (0 << CS02) | (0 << CS01) | (1 << CS00);							// no pre scaling  (pg. 117)

}

void setup_USART(){
	
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


void setup_interrupts(){
	
	PCICR = (1 << PCIE1);						// Enables possibility of interrupts on pins 14-8
	//PCMSK1 = (1 << PCINT13) | (1 << PCINT12);	// Enables interrupts on pin PC5 & PC4
	
}

/* Turns off output on selected pin on PORTC */
void turnOff_C(int pin){
	PORTC &= ~(1 << pin);
	 
}
/* Turns on output on selected pin on PORTC */
void turnOn_C(int pin){
	PORTC |= (1 << pin);
}

/* Routine for clockwise rotation of encoder */
void clockwise(){
	turnOff_C(PC2);
	turnOn_C(PC0);
	if(pwm < 245){
		pwm += 10;
	}
	else{
		pwm = 255;
	}
}

/* Routine for counterclockwise rotation of encoder */
void counterclockwise(){
	turnOff_C(PC0);
	turnOn_C(PC2);
	if(pwm > 10){
		pwm -= 10;
	}
	else{
		pwm = 0;
	}
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
	
	reg->setValue = setValue;
	OCR0A = setValue;
	OCR0B = setValue;
	
	return reg;
}

// See pg. 190
/* Transmits the data-string over the USART. Ends transmission with a ; */ 
void USART_transmit(char *data){
	int pos = 0;
	
	while(*(data + pos) != '\0' ){
		// Wait for empty transmit buffer
		while( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = *(data + pos);
		_delay_ms(100);
		pos++;
	}
	UDR0 = ';';
}

uint8_t USART_recieve(void){
	
	// Wait for data to be recieved
	while( !(UCSR0A & (1 << RXC0)) );
	// Get and return recieved data from the buffer
	return UDR0;
	
}

/************************************************************************/
/* INTERRUPT SERVICE ROUTINES											*/
/************************************************************************/

/* ISR for PCINT14-8 */
ISR(PCINT1_vect){
	
	unsigned int ABnew = 0x00;
	unsigned int A = 0x00;
	unsigned int B = 0x00;
	
	A = (PINC & (1 << PC5));
	A = (A >> (PC5 - 1));	// Sets A in pos 1
	
	B = (PINC & (1 << PC4));
	B = (B >> PC4);			// Sets B in pos 0
	
	ABnew = A | B;
	
	switch(ABnew){
		case 0:	
			if(AB == 2){
				clockwise();
			}
			else{
				counterclockwise();
			}
			break;
		
		case 1:
			if(AB == 0){
				clockwise();
			}
			else{
				counterclockwise();
			}
			break;
	}
	AB = ABnew;

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
	PORTC |= (1 << PC0);
	for(int i = 4; i > -1; i--){
		while( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = recieved_bytes[i];
		_delay_ms(100);
	}
	PORTC &= ~(1 << PC0);
	
	PORTC |= (1 << PC2);
	sei();
}








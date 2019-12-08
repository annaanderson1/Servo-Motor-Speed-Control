/*
 * interrupts.c
 *
 * Created: 2019-12-04 09:25:03
 *  Author: joakimcedergren
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int AB;

static void setup(){
	DDRC = (1 << PC2) | (1 << PC1) | (1 << PC0);	// for LEDs

	// Enables possibility of interrupts on pins 14-8
	PCICR = (1 << PCIE1);
	// Enables interrupts on pin PC4-PC5
	PCMSK1 = (1 << PCINT13) | (1 << PCINT12);

}

static void turnOff_C(int pin){
	PORTC &= ~(1 << pin);

}
/* Turns on output on selected pin on PORTC */
static void turnOn_C(int pin){
	PORTC |= (1 << pin);
}

static void clockwise(){
	turnOff_C(PC2);
	turnOn_C(PC0);
}

static void counterclockwise(){
	turnOff_C(PC0);
	turnOn_C(PC2);
}

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

/* In order to test interrupt pins individually, disable one pin in
 * setup function, register PCMSK1
 */ 
int main(void){
    
	setup();
	sei();
	
	while(1){
		 
    }
}
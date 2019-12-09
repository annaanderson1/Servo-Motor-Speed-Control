/*
 * adc.c
 *
 * Created: 2019-12-08 14:36:57
 *  Author: joakimcedergren
 */ 


#include <avr/io.h>


static void setup(){
	DDRC = (1 << PC2) | (1 << PC1) | (1 << PC0);
	
	ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0);
	ADCSRA |= (1 << ADEN) | (1 << ADATE);
	ADCSRB |= (1 << ADTS2) | (1 << ADTS1);
	DIDR0 |= (1 << ADC3D);
	
	TCCR1B |= (1 << CS11);
}

static void turnOff_C(int pin){
	PORTC &= ~(1 << pin);

}

static void turnOn_C(int pin){
	PORTC |= (1 << pin);
}



int main(void){
	short fine_tuning;
	fine_tuning = 0;
	
	setup();
	
    while(1){
		fine_tuning = ADCL;
		fine_tuning |= (ADCH << 8);
		TIFR1 |= (1 << TOV0);
		
		// Vin < ~1,5V
		if(fine_tuning <= 300){
			turnOff_C(PC0);
			turnOff_C(PC1);
			turnOn_C(PC2);
		}
		// Vin < ~3,4V
		else if(fine_tuning <= 700){
			turnOff_C(PC0);
			turnOn_C(PC1);
			turnOff_C(PC2);
		}
		else{
			turnOn_C(PC0);
			turnOff_C(PC1);
			turnOff_C(PC2);
		}
    }
	
	return 0;
}
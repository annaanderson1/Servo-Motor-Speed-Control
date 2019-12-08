/*
 * led.c
 *
 * Created: 2019-12-08 15:09:05
 *  Author: joakimcedergren
 */ 


#include <avr/io.h>


static void setup(){
	DDRC = (1 << PC2) | (1 << PC1) | (1 << PC0);
}

static void turnOff_C(int pin){
	PORTC &= ~(1 << pin);

}

static void turnOn_C(int pin){
	PORTC |= (1 << pin);
}

int main(void){
    
	setup();
	
	turnOn_C(PC0);
	turnOn_C(PC1);
	turnOn_C(PC2);
	
	while(1){
        
    }
}
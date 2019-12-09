/*
 * pwm.c
 *
 *
 *	Sends an inverted PWM over pin PD5 and PD6
 *
 * Created: 2019-12-03 16:20:52
 *  Author: joakimcedergren
 */ 


#include <avr/io.h>

// 0 ~ 4,9V
// 100 ~ 3,0V
// 200 ~ 1,0V
// 250 ~ 0,1V
#define PWM 250

static void setup(){
	
	DDRD |= (1 << PD6) | (1 << PD5);
	TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00);
	TCCR0B =  (0 << CS02) | (0 << CS01) | (1 << CS00);

}

int main(void){
    
	setup();
	
	OCR0A = PWM;
	OCR0B = PWM;
	
	while(1){
        
    }
}
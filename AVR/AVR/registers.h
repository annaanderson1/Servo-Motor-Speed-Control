/*
 * registers.h
 *
 * Created: 2019-10-15 12:40:57
 *  Author: tmk19jc
 */ 


#ifndef REGISTERS_H_
#define REGISTERS_H_

typedef struct {
	unsigned int setValue;	
} Registers;

/*
	Initiates the DDRx, PORTx and PINx registers for all the pins.
*/
void setup_registers();

/*
	Sets the trigger value for the PWM signal. Must be an int in range 0-255
*/
Registers* set_trigger(Registers* reg, int setValue);

/*
	Transmits data over the serial port
*/
void USART_transmit(unsigned char data);

/*
	Recieves data over the serial port
*/
uint8_t USART_recieve(void);

#endif 

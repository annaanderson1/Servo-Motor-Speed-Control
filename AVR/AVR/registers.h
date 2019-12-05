/*
 * registers.h
 *
 * Created: 2019-10-15 12:40:57
 *  Author: Joakim Cedergren
 *
 * Used for setting up registers on the ATMega88 for speed controller.
 *
 */


#ifndef REGISTERS_H_
#define REGISTERS_H_

/*
 *  Initiates registers for DDR, PWM, USART, interrupts, encoder clock speed, and ADC conversion.
*/
void setup_registers();

#endif

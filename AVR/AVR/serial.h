/*
 * 	serial.h
 *
 * 	Created: 2019-11-19 10:27:21
 * 	Author: Joakim Cedergren
 *
 *	Functions and ISR for the serial communication with the master PC.
 *
 */

#ifndef SERIAL_H_
#define SERIAL_H_

/*
 * 	Transmits 5 characters over the USART.
 * 	If number of submitted chars < 5, the empty spots is padded with " "
 * 	*data: pointer to chars to be transmitted
 * 	See details in datasheet pg. 190
*/
void USART_transmit(char *data);



#endif
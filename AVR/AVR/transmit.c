/*
 *	transmit.c
 *
 *	Created: 2019-11-27 14:07:14
 *  Author: Joakim Cedergren
 *	
 *	Functions for transmitting messages over the USART.
 *
 */ 

#include <stdbool.h>
#include <inttypes.h>
#include "shared.h"
#include "serial.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

/* Globals */ 
extern bool newCommand;
extern bool newMeasurement;
extern unsigned short clk_curr;
extern char recieved_bytes[5];
extern unsigned short clk_elapsed;

/* Transmits the fine tuning over the USART. */
void transmit_fine_tuning(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%hd", shared_ptr->fine_tuning);
	USART_transmit(temp);
}

/* Transmits the set speed over the USART. */
void transmit_speed_set(Shared_Data* shared_ptr){
	char temp[6];
	
	snprintf(temp, 6, "%hd",  shared_ptr->speed_set);
	USART_transmit(temp);
}

/* Transmits the pwm output over the USART. */
void transmit_pwm(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	
	snprintf(temp, 6, "%d", shared_ptr->pwm);
	USART_transmit(temp);
}

/* Transmits the clk_elapsed over the USART. */
void transmit_clk_elapsed(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);

	snprintf(temp, 6, "%hu", clk_elapsed);
	USART_transmit(temp);
}

/* Transmits the delta_time over the USART. */
void transmit_delta_time(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%lu", shared_ptr->delta_time);
	USART_transmit(temp);
}

/* Transmits the control error over the USART. */
void transmit_error(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%hd", (shared_ptr->error >> N));
	USART_transmit(temp);
}

/* Transmits the average rpm over the USART. */
void transmit_rpm_avg(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%lu", (shared_ptr->rpm_avg >> N));
	USART_transmit(temp);
}

/* Transmits the latest measured rpm over the USART. */
void transmit_rpm_curr(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%lu", shared_ptr->rpm_curr);
	USART_transmit(temp);
}

/* Transmits five " " over the USART. */
void transmit_empty(){
	char temp[6];
	strncpy(temp, "     ", 5);
	USART_transmit(temp);
}


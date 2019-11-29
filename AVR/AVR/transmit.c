/*
 * transmit.c
 *
 * Created: 2019-11-27 14:07:14
 *  Author: tmk19jc
 */ 
#include <stdbool.h>
#include <inttypes.h>
#include "shared.h"
#include "serial.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

extern bool newCommand;
extern bool newMeasurement;
extern unsigned short clk_curr;
extern char recieved_bytes[5];

void transmit_0(Shared_Data* shared_ptr){
	
}

void transmit_1(Shared_Data* shared_ptr){

}

/* Sets a new set value for the speed. Range: 0-120. Transmits an empty message. */
void transmit_2(Shared_Data* shared_ptr){
	char temp[6];
	char sub_str[4];
	int res;
	strncpy(temp, "     ", 5);
	strncpy(sub_str, recieved_bytes + 1, 3);
		
	res = atoi(sub_str);
	shared_ptr->speed_set = res;
	OCR0A = res;
	OCR0B = res;
	USART_transmit(temp);

}

/* Transmits the set speed. */
void transmit_3(Shared_Data* shared_ptr){
	char temp[6];
	
	snprintf(temp, 6, "%d",  shared_ptr->speed_set);
	USART_transmit(temp);
}


void transmit_4(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	
	snprintf(temp, 6, "%d", shared_ptr->speed_actual);
	USART_transmit(temp);
}

void transmit_5(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);

	snprintf(temp, 6, "%hu", shared_ptr->clk_elapsed);
	USART_transmit(temp);
}

void transmit_6(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%lu", shared_ptr->delta_time);
	USART_transmit(temp);
}

void transmit_7(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	USART_transmit(temp);
}

void transmit_8(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%lu", shared_ptr->rpm_avg);
	USART_transmit(temp);
}

void transmit_9(Shared_Data* shared_ptr){
	char temp[6];
	strncpy(temp, "     ", 5);
	snprintf(temp, 6, "%lu", shared_ptr->curr_rpm);
	USART_transmit(temp);
}

void transmit_empty(){
	char temp[6];
	strncpy(temp, "     ", 5);
	USART_transmit(temp);
}



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

extern bool newCommand;
extern bool newMeasurement;
extern uint16_t clk_curr;
extern char recieved_bytes[5];

void transmit_0(Shared_Data* shared_ptr){
	
}

void transmit_1(Shared_Data* shared_ptr){

}
/*
void transmit_2(Shared_Data* shared_ptr){
	char temp[6];
	//char sub_str[4];

	//char *endptr;
	int val;
	//long l;
	val = 60;
	shared_ptr->speed_set = val;
	
	//strncpy(sub_str, recieved_bytes + 1, 3);
	
	//strncpy(sub_str + 3, ";", 1);
	//l = strtol(sub_str, &endptr, 10);
	//speed_set = val;
	//sprintf(buf, sub_str);
	//memset(buf,' ', 6*sizeof(char));
	USART_transmit(temp);
	//OCR0B = speed_set;
	//OCR0A = speed_set;
}
*/
void transmit_2(Shared_Data* shared_ptr){
	char temp[6];
	
	shared_ptr->speed_set = 60;

	USART_transmit(temp);

}


void transmit_3(Shared_Data* shared_ptr){
	char temp[6];
	
	sprintf(temp, "%d",  shared_ptr->speed_set);
	USART_transmit(temp);
}


void transmit_4(Shared_Data* shared_ptr){
	char temp[6];
	sprintf(temp, "%d", shared_ptr->speed_actual);
}

void transmit_5(Shared_Data* shared_ptr){
	char temp[6];
	snprintf(temp, 6, "%" PRIu16, shared_ptr->clk_elapsed);
	USART_transmit(temp);
}

void transmit_6(Shared_Data* shared_ptr){
	char temp[6];
	sprintf(temp, "%" PRIu32, shared_ptr->rpm_avg);
}

void transmit_7(Shared_Data* shared_ptr){
	//char temp[6];
	//sprintf(temp, "%u", test_var1);
}

void transmit_8(Shared_Data* shared_ptr){
	char temp[6];
	uint16_t test = 60000;
	
	snprintf(temp, 6, "%" PRIu16, test);
	
	USART_transmit(temp);
}

void transmit_9(Shared_Data* shared_ptr){
	char temp[6];
	sprintf(temp, "%" PRIu32, shared_ptr->curr_rpm);
	//strncpy(buf, temp, 5);
}




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

void transmit_0(){
	
}

void transmit_1(){

}

void transmit_2(int* speed_set_pnt){
	char temp[6];
	//char sub_str[4];

	//char *endptr;
	int val;
	//long l;
	val = 60;
	
	//strncpy(sub_str, recieved_bytes + 1, 3);
	
	//strncpy(sub_str + 3, ";", 1);
	//l = strtol(sub_str, &endptr, 10);
	//speed_set = val;
	speed_set_pnt = &val;
	//sprintf(buf, sub_str);
	//memset(buf,' ', 6*sizeof(char));
	USART_transmit(temp);
	//OCR0B = speed_set;
	//OCR0A = speed_set;
}

void transmit_3(int* speed_set_pnt){
	char temp[6];
	
	sprintf(temp, "%d",  *speed_set_pnt);
	USART_transmit(temp);
}


void transmit_4(){
	char temp[6];
	sprintf(temp, "%d", speed_actual);
}

void transmit_5(){
	char temp[6];
	snprintf(temp, 6, "%" PRIu16, clk_elapsed);
	USART_transmit(temp);
}

void transmit_6(){
	char temp[6];
	sprintf(temp, "%" PRIu32, rpm_avg);
}

void transmit_7(){
	char temp[6];
	sprintf(temp, "%u", test_var1);
}

void transmit_8(){
	char temp[6];
	uint16_t test = 60000;
	
	snprintf(temp, 6, "%" PRIu16, test);
	
	USART_transmit(temp);
}

void transmit_9(){
	char temp[6];
	sprintf(temp, "%" PRIu32, curr_rpm);
	//strncpy(buf, temp, 5);
}




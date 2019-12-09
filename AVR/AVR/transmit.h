/*
 *	transmit.h
 *
 *	Created: 2019-11-27 14:07:24
 *  Author: Joakim Cedergren
 *
 *	Functions for transmitting messages over the USART.
 *
 */ 


#ifndef TRANSMIT_H_
#define TRANSMIT_H_

/* Transmits the fine tuning over the USART. */
void transmit_fine_tuning(Shared_Data* shared_ptr);

/* Transmits the set speed over the USART. */
void transmit_speed_set(Shared_Data* shared_ptr);

/* Transmits the pwm output over the USART. */
void transmit_pwm(Shared_Data* shared_ptr);

/* Transmits the clk_elapsed over the USART. */
void transmit_clk_elapsed(Shared_Data* shared_ptr);

/* Transmits the delta_time over the USART. */
void transmit_delta_time(Shared_Data* shared_ptr);

/* Transmits the control error over the USART. */
void transmit_error(Shared_Data* shared_ptr);

/* Transmits the average rpm over the USART. */
void transmit_rpm_avg(Shared_Data* shared_ptr);

/* Transmits the latest measured rpm over the USART. */
void transmit_rpm_curr(Shared_Data* shared_ptr);

/* Transmits five " " over the USART. */
void transmit_empty();

#endif 
/*
 * pid.c
 *
 * Created: 24-Nov-18 18:19:10
 *  Author: ScorpionIPX
 */ 

#include "global.h"
#include "pid.h"
#include "usart.h"
#include "display.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int fuzzy_table[50] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345, 360, 375, 390, 405, 420, 435, 450, 465, 480, 495, 510, 525, 540, 555, 570, 585, 600, 615, 630, 645, 660, 675, 690, 705, 720, 735};

float KP = 25;
float KI = 1.2;
float KD = 10;

float error = 0;
float integral = 0;
float derivative = 0;
float previous_error = 0;
float pid_result = 0;

void init_optocoupler(void)
{
	INT0_CNT = 0;
	REVOLUTIONS = 0;
	DDRD &= 0b11111011;   // configure INT0 as input
	PORTD |= 0b0000100;
	GICR = 1 << INT0;
	MCUCR = (1 << ISC00);
}

void init_pid(void)
{
	// set up timer with prescaler = 1024
	TCCR0 |= (1 << CS02)|(1 << CS00);
		
	// initialize counter
	TCNT0 = 0;
		
	// enable overflow interrupt
	TIMSK |= (1 << TOIE0);
	REVOLUTIONS_PER_MINUTE = 0;
	TICKS = 0;
}

void pid(void)
{
	error = TARGET_TICKS - TICKS;
	integral += error;
	if(integral > 1500)
	{
		integral = 1500;
	}
	derivative = previous_error - error;
	pid_result = (KP * error) + (KI * integral) + (KD * derivative);
	
	OCR1B = (unsigned int) (pid_result);
	
	if(OCR1B > ICR1)
	{
		OCR1B = ICR1;
	}
	
	previous_error = error;
	
	
}

int fuzzy_pwm[61] = {-5, -5, -5, -5, -5, -5, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5};
// int fuzzy_pwm[61] = {-30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -1, -1, -1, 0, 1, 1, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};

void fuzzy(void)
{
	error = TARGET_TICKS - TICKS;
	if(error < -30)
	{
		error = -30;
	}
	if(error > 30)
	{
		error = 30;
	}
	
	unsigned int defuzzy_index = ((unsigned int)(error));
	OCR1B += fuzzy_pwm[defuzzy_index + 30];
	
	if(OCR1B < 0)
	{
		OCR1B = 0;
	}
	if(OCR1B > ICR1)
	{
		OCR1B = ICR1;
	}
}

ISR(INT0_vect)  // external interrupt_zero ISR (INT0)
{
	INT0_CNT++;
	TICKS ++;
	if(INT0_CNT >= TICKS_PER_REVOLUTION)
	{
		REVOLUTIONS ++;
		// USART_Transmit(REVOLUTIONS);
	}
}

// every 16.384 ms
ISR(TIMER0_OVF_vect)
{
	TIMER0_CNT ++;
	pid();
	//fuzzy();
	if(TIMER0_CNT >= 10)
	{
		update_display_buffer(TICKS);
		TIMER0_CNT = 0;
	}
	USART_Transmit(255);
	USART_Transmit(TARGET_TICKS);
	USART_Transmit(TICKS);
	USART_Transmit(error);
	USART_Transmit(0);
	USART_Transmit(0);
	//USART_Transmit((unsigned char)(error));
	//update_display_buffer(REVOLUTIONS_PER_MINUTE);
	TICKS = 0;
}
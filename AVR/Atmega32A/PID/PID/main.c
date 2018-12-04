/*
 * PID.c
 *
 * Created: 24-Nov-18 17:06:28
 * Author : ScorpionIPX
 */ 

#include "global.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#include "display.h"
#include "pid.h"
#include "usart.h"
#include "pwm.h"

int main(void)
{
	TARGET_TICKS = 13;
    init_display();
	init_optocoupler();
	init_pid();
	USART_Init(MYUBRR);
	init_pwm();
	dummy = 90;
	OCR1A = 0;
	sei();
	OCR1B = 0;
	_delay_ms(50);

    while (1)
    {
		display();
    }
}





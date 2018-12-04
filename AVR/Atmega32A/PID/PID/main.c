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
#include "adc_driver.h"

int main(void)
{
	TARGET_TICKS = 23;
    init_display();
	init_optocoupler();
	init_pid();
	USART_Init(MYUBRR);
	init_pwm();
	ADC_init();
	dummy = 90;
	sei();
	_delay_ms(150);

    while (1)
    {
		display();
    }
}





/*
 * usart.h
 *
 * Created: 24-Nov-18 18:35:22
 *  Author: ScorpionIPX
 */ 


#ifndef USART_H_
#define USART_H_

#include "global.h"

#define BAUD 38400
#define MYUBRR F_CPU / 16 / BAUD - 1

void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data);
unsigned char USART_Receive(void);

#endif /* USART_H_ */
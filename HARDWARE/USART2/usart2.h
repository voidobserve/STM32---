#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"
	
extern uint8_t USART2_RX_BUF[128]; // ���ջ���
extern uint8_t USART2_RX_FALG; // ����״̬���


void USART2_Init(u32 baud);


#endif



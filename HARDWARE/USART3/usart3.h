#ifndef __USART3_H
#define __USART3_H

#include "stm32f4xx.h"

#ifndef USART_REC_LEN
#define USART_REC_LEN 512 // �����������ֽ���
#endif                    // end def USART_REC_LEN

extern u8 USART3_RX_DATA;               // ����3���յ�������--1�ֽ�
extern u8 USART3_RX_BUF[USART_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8 USART3_RX_STA;                // ����״̬���
extern u8 USART3_RX_CNT;                // ��¼���ջ������е�ǰ�����˶��ٸ��ֽ�

extern uint8_t Tx3Buffer[512];
extern volatile uint32_t Rx3Counter;
extern volatile uint8_t Rx3Data;
extern volatile uint8_t Rx3End;
extern volatile uint8_t Rx3Buffer[512];

void USART3_Init(u32 bound);
void USART3_SendByte(u8 Byte);
void USART3_SendString(char *str);
void USART_SendBytes(USART_TypeDef *pUSARTx, uint8_t *buf, uint32_t len);

#endif

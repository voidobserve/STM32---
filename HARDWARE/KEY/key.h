#ifndef __KEY__H
#define __KEY__H

#include "sys.h"

extern volatile uint8_t key;

// ע�⣬��������ַ�ʽֻ��ͬʱʹ��һ�֣����������ظ�����ľ���

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0    GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) // PA0
#define KEY1    GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)  // PE2
#define KEY2    GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)  // PE3
#define KEY3    GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)  // PE4


/*����ķ�ʽ��ͨ��λ��������ʽ��ȡIO*/

// #define KEY0		PEin(4)	//PE4
// #define KEY1		PEin(3)	//PE3
// #define KEY2		PEin(2)	//PE2
// #define WK_UP		PAin(0)	//PA0

/*�����ǽ�����ת��Ϊ��Ӧ�ļ�ֵ*/
#define KEY0_PRES 1
#define KEY1_PRES 2
#define KEY2_PRES 3
#define KEY3_PRES 4

void KEY_Init(void); // ���̶�Ӧ��IO�ڳ�ʼ��
void KEY_IT_Init(void); // ������ʼ�������Ҵ򿪰�����Ӧ���ж�
u8 KEY_Scan(u8);     // ����ɨ�躯��

#endif

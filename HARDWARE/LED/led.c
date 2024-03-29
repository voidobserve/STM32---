#include "led.h"
#include "stm32f4xx.h" //ֱ������ͷ�ļ�

// �������ͷ�ļ�
#include "sys.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	// ����Ϊ�ߵ�ƽ������
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
}

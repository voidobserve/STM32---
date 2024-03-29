/*************************************************
 Copyright ? 0010. All rights reserved.
// �ļ���
@ File name: ultraviolet_light.c -- ������LED������Դ����
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
            ������LED������Դ����
		������LEDʹ��5V���磬ͨ����Ƭ�����Ƽ̵����Ŀ���������������LED��
		�̵����Ŀ��ƶ�����Ϊ�͵�ƽʹ�ܡ�

@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#include "ultraviolet_light.h"
#include "stm32f4xx.h"

/**
 * @brief  ������LED���ƶ˵ĳ�ʼ��
 * 		 		������LEDʹ��5V���磬ͨ����Ƭ�����Ƽ̵����Ŀ���������������LED
 * 
 * @param  ��
 * @retval ��
*/
void UlrtavioletLight_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	// ʱ��ʹ��
	ULTRAVIOLET_LIGHT_RCC_CMD(ULTRAVIOLET_LIGHT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ULTRAVIOLET_LIGHT_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(ULTRAVIOLET_LIGHT_PORT, &GPIO_InitStructure);

	// ����Ϊ�ߵ�ƽ������
	ULTRAVIOLET_LIGHT_SET(1);
}



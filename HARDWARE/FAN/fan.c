/*************************************************
 Copyright ? 0010. All rights reserved.
// �ļ���
@ File name: fan.c -- ���ȵ�����Դ����
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
			���ȵ�����Դ����

			ע��: ���ȵ�PWM������Ҫ����������ʱ����ʱ��Ƶ�ʺ͵��Ի�ã�
				 ��˾����ʹ���п��ܻ�Ҫ�޸����еĳ�ʼ������

@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#include "fan.h"
#include "stm32f4xx.h"

// ��¼����ת�ٵ�ȫ�ֱ���
volatile int cur_fan_speed = CUR_FAN_SPEED_CLOSE;

/**
 * @brief  ��ʱ��ͨ��PWM���Ʒ��ȵĳ�ʼ������
 *
 * @param  ��
 * @retval ��
 */
void FAN_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// 1.�򿪶�ʱ����ʱ��
	FAN_TIM_RCC_CMD(FAN_TIM_RCC, ENABLE);

	// 2.��GPIO�˿�ʱ��
	FAN_RCC_CMD(FAN_RCC, ENABLE);

	// 3.����GPIO���Ų���ʼ��
	GPIO_InitStructure.GPIO_Pin = FAN_PIN;			   // ���ű��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   // ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // �������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // �������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // ��������
	GPIO_Init(FAN_PORT, &GPIO_InitStructure);

	FAN_SET_CLOSE();

	// 4.��Ҫѡ������Ҫ���õĹ���  PB3 -- TIM2_CH2
	GPIO_PinAFConfig(FAN_PORT, FAN_PIN_SOURCE, FAN_AF_PERIPHERAL);

	// 5.���ö�ʱ���Ļ������� + ��ʼ��   ע�⣺�����źŵ����ھ����ܶ�һ��  ����10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;				// TIM2 84MHZ / 84 = 1000000HZ  1us����1��
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;					// �������ڣ�����100��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				// ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
	TIM_TimeBaseInit(FAN_TIM, &TIM_TimeBaseStructure);

	// 6.���ö�ʱ����ͨ�� + ��ʼ����ʱ��ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // PWMģʽ1  CNT < CCR1 ͨ����Ч
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ʹ������Ƚ�
	// TIM_OCInitStructure.TIM_Pulse = 50;							  // CCR�Ĵ����ĳ�ֵ  Ĭ��ռ�ձ�50%
	TIM_OCInitStructure.TIM_Pulse = 0;						  // CCR�Ĵ����ĳ�ֵ  Ĭ��ռ�ձ�0%�����Ȳ���ת
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // �ߵ�ƽ��Ч
	FAN_TIM_OC_INIT(FAN_TIM, &TIM_OCInitStructure);			  // TIM2_CH2��ʼ��

	// 7.ʹ�ܶ�ʱ��ͨ���Ե�Ԥװ�ؼĴ���
	FAN_TIM_PRELOAD_CONFIG(FAN_TIM, TIM_OCPreload_Enable);

	// 8.ʹ���Զ�����Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(FAN_TIM, ENABLE);

	// 9.�򿪶�ʱ��
	TIM_Cmd(FAN_TIM, ENABLE);

	FAN_SET_CLOSE();
}


// ���ݼ�¼����ת�ٵı������ڷ���ת��
void FAN_Speed_Adjust(void)
{
	switch (cur_fan_speed)
	{
	case CUR_FAN_SPEED_CLOSE:
		FAN_SET_CLOSE();
		break;
	case CUR_FAN_SPEED_MIN:
		FAN_SET_MIN_SPEED();
		break;
	case CUR_FAN_SPEED_LOW:
		FAN_SET_LOW_SPEED();
		break;
	case CUR_FAN_SPEED_MID:
		FAN_SET_MID_SPEED();
		break;
	case CUR_FAN_SPEED_MAX:
		FAN_SET_MAX_SPEED();
		break;
	default:
		break;
	}
}

void FAN_Speed_Inc(void)
{
	cur_fan_speed++;

	if (CUR_FAN_SPEED_MAX < cur_fan_speed)
	{
		cur_fan_speed = CUR_FAN_SPEED_MAX;
	}

	FAN_Speed_Adjust();
}


void FAN_Speed_dec(void)
{
	cur_fan_speed--;

	if (CUR_FAN_SPEED_CLOSE > cur_fan_speed)
	{
		cur_fan_speed = CUR_FAN_SPEED_CLOSE;
	}

	FAN_Speed_Adjust();
}

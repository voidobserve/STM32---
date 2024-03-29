#include "tim.h"
#include "stm32f4xx.h"

volatile uint8_t tim6_update_cnt = 0; // ������ʱ��TIM6�����¼��ļ�������TIM6ÿ5s����һ�θ����жϣ�

// TIM14�ĳ�ʼ��
void TIM14_PWMConfig(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// 1.��TIM14��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	// 2.��GPIO�˿�ʱ��  PF9
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	// 3.����GPIO���Ų���ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		   // ���ű��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   // ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // �������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // �������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // ��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	// 4.��Ҫѡ������Ҫ���õĹ���  PF9 -- TIM14_CH1
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);

	// 5.���ö�ʱ���Ļ������� + ��ʼ��   ע�⣺�����źŵ����ھ����ܶ�һ��  ����10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;				// TIM14 84MHZ / 8400 = 10000HZ  100us����1��
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;					// 10ms * 1000 / 100 = 100��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				// ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM14ֻ�ܵ�������
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	// 6.���ö�ʱ����ͨ�� + ��ʼ����ʱ��ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // PWMģʽ1  CNT < CCR1 ͨ����Ч
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ʹ������Ƚ�
	TIM_OCInitStructure.TIM_Pulse = 50;							  // CCR�Ĵ����ĳ�ֵ  Ĭ��ռ�ձ�50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // �ߵ�ƽ��Ч
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);

	// 7.ʹ�ܶ�ʱ��ͨ���Ե�Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);

	// 8.ʹ���Զ�����Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM14, ENABLE);

	// 9.�򿪶�ʱ��
	TIM_Cmd(TIM14, ENABLE);
}


// ������ʱ��6��ʼ����������MQTTЭ�鶨ʱ50s��ÿ50���ñ�־λ��һ
// ��⵽�����־λ��һ�󣬸�����������������
// ��ʼ���󣬻�����ʱ��ÿ5s����һ���ж�
void TIM6_Init(void)
{
		// �����ʼ���ṹ�����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// �򿪶�ʱ�������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// ����ʱ����ʼ���ṹ������ĳ�Ա��ֵ
	TIM_TimeBaseStructure.TIM_Period = 50000 - 1;				// ��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;				// Ԥ��Ƶֵ  84MHz / 8400 = 10000Hz����1000ms��10000���źŲ��Σ���1ms����10��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				// ����Ƶ
	// ��ʼ����ʱ��6
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	// ����Ƕ�������жϿ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// ѡ��ʱ�����ж�Դ��ʹ����������ж�
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	// �򿪶�ʱ��6
	TIM_Cmd(TIM6, ENABLE);
}




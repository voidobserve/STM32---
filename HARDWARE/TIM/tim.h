#ifndef __TIM_H
#define __TIM_H

#include "stm32f4xx.h"

extern volatile uint8_t tim6_update_cnt; // ������ʱ��TIM6�����¼��ļ�������TIM6ÿ5s����һ�θ����жϣ�

// ������ʱ��6��ʼ����������MQTTЭ�鶨ʱ50s��ÿ50���ñ�־λ��һ
// ��⵽�����־λ��һ�󣬸�����������������
// ��ʼ���󣬻�����ʱ��ÿ5s����һ���ж�
void TIM6_Init(void);

void TIM14_PWMConfig(void);

#endif

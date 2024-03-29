#ifndef __DUST_SENSOR_H
#define __DUST_SENSOR_H

#include "stm32f4xx.h"

extern volatile float pm2_5_mg_per_cubic_meter;

// =================================================================
// ====================================================
//-----------------�۳������� ģ��������˿���ض���----------------
// �۳�������ģ������������ӵĶ˿�
#ifndef DUST_SENSOR_AO_PORT 
#define DUST_SENSOR_AO_PORT    GPIOA
#endif
// �۳�������ģ������������ӵĶ˿ڵ����ź�
#ifndef DUST_SENSOR_AO_PIN 
#define DUST_SENSOR_AO_PIN    GPIO_Pin_6
#endif
// �۳�������ģ������������ӵĶ˿ڶ�Ӧ��ʱ��ʹ�ܺ���

// �۳�������ģ������������ӵĶ˿ڶ�Ӧ��ʱ��

// ����LED��ʹ��PE6

//-----------------�۳������� ģ��������˿���ض���----------------
// ====================================================
// =================================================================

#define DUST_LED_SET(N)  ( (N) ? (GPIO_SetBits(GPIOG, GPIO_Pin_12)) : (GPIO_ResetBits(GPIOG, GPIO_Pin_12)) )

void DustSensor_Init(void);
void DustSensor_GetVal(void);

void DustSensor_Test(void);

#endif


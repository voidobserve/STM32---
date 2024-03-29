/*************************************************
 Copyright ? 0010. All rights reserved.

// �ļ���
@ File name: ms1100.h -- ms1100��ȩ���ģ���ͷ�ļ�
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
            ms1100.c -- ms1100��ȩ���ģ���ͷ�ļ�
        ע�⣺MS1100��ȩ���ģ�������ģ���ѹֵ��0~5V��
            Ҫ��ȷ��MCU��Ӧ��IO���Ƿ���5V����

@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#ifndef __MS1100_H
#define __MS1100_H

#include "stm32f4xx.h"
#include "my_config.h"

extern volatile float hcho_mg_per_cubic_meter;

// ============================================================================================
// ===============================================================================
//-----------------MS1100��ȩŨ�ȼ��ģ�����ض���----------------
// ���ȩŨ�ȼ��ģ��ADC��������ӵ�GPIO�˿�
#ifndef MS1100_PROT
#define MS1100_PROT              GPIOA
#endif
// ���ȩŨ�ȼ��ģ��ADC��������ӵ�GPIO�˿ڵ����ź�
#ifndef MS1100_PIN
#define MS1100_PIN               GPIO_Pin_6
#endif
// ���ȩŨ�ȼ��ģ��ADC��������ӵ�GPIO�˿ڵ�ʱ��ʹ�ܺ���
#ifndef MS1100_RCC_CMD
#define MS1100_RCC_CMD           RCC_AHB1PeriphClockCmd
#endif
// ���ȩŨ�ȼ��ģ��ADC��������ӵ�GPIO�˿ڵ�ʱ��
#ifndef MS1100_RCC
#define MS1100_RCC               RCC_AHB1Periph_GPIOA
#endif
// ���ȩŨ�ȼ��ģ��ADC��������ӵ�GPIO�˿ڸ��õ���ADC
#ifndef MS1100_ADC               
#define MS1100_ADC          ADC1
#endif
// ADC��ʱ��ʹ�ܺ���
#ifndef MS1100_ADC_RCC_CMD
#define MS1100_ADC_RCC_CMD  RCC_APB2PeriphClockCmd
#endif
// ADC��ʱ��
#ifndef MS1100_ADC_RCC
#define MS1100_ADC_RCC  RCC_APB2Periph_ADC1
#endif
// ADC�Ĺ���ͨ��
#ifndef MS1100_ADC_REGULARCHANNEL
#define MS1100_ADC_REGULARCHANNEL ADC_Channel_6
#endif

//-----------------MS1100��ȩ���ģ�����ض���----------------
// ===============================================================================
// ============================================================================================

void MS1100_Init(void);                // MS1100��ȩ��������ʼ������
float MS1100_ADCValToPPM(u16 adc_val); // ���ɼ����ĵ�ѹֵת������ppmΪ��λ��ʾ�ĸ�����ֵ

void MS1100_Get_Mg_per_cubic_meter(void); // ��ȡһ�μ�ȩŨ�ȵ���ֵ����ŵ�ȫ�ֱ�����

void MS1100_Test(void);                // MS1100��ȩģ��Ĳ��Ժ����������Ǹ���ѭ����

#endif

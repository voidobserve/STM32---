/*************************************************
 Copyright ? 0010. All rights reserved.

// �ļ���
@ File name: ms1100.c -- ms1100��ȩ���ģ�����������
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
            ms1100.c -- ms1100��ȩ���ģ�����������
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
#include <math.h>
#include "stm32f4xx.h"
#include "ms1100.h"

// ============================================
// ������
#include <stdio.h>
#include "usart.h"
#include "delay.h"
// ============================================

volatile float hcho_mg_per_cubic_meter = 0;

// PA6  ADC12_IN6
// MS1100��ȩ��������ʼ��
// ��3.3V��ѹ�ֳ���4096��

/**
 * @brief  MS1100��ȩ��������ʼ������
 *
 * @param  ��
 * @retval ��
 */
void MS1100_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 1.��ADC����Ľӿ�ʱ��
    MS1100_ADC_RCC_CMD(MS1100_ADC_RCC, ENABLE);

    // 2.��GPIO�˿�ʱ��
    MS1100_RCC_CMD(MS1100_PIN, ENABLE);

    // 3.����GPIO���Ų���+��ʼ�� ע�⣺����ģʽΪģ��ģʽ
    GPIO_InitStructure.GPIO_Pin = MS1100_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(MS1100_PROT, &GPIO_InitStructure);

    // 4.����ADC����Ĳ���
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     // ����ģʽ
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;                  // Ԥ��Ƶֵ
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      // ��ʹ��DMA
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; // ���β����ļ��ʱ��
    ADC_CommonInit(&ADC_CommonInitStructure);

    // 5.����ADC�����ת�����ȡ����ݶ���
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      // ת������  12bit
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               // ��ɨ��
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                         // ������ת��
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // ��ʹ���ⲿ����Դ
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      // ���ݶ���  �Ҷ���
    ADC_InitStructure.ADC_NbrOfConversion = 1;                                  // ת���ܴ���
    ADC_Init(MS1100_ADC, &ADC_InitStructure);

    // 6.����ADC�Ĺ���ͨ��  ��ת��ʱ�� = 3 + 12 = 15����
    ADC_RegularChannelConfig(MS1100_ADC, MS1100_ADC_REGULARCHANNEL, 1, ADC_SampleTime_3Cycles);

    // 7.ʹ��ADC
    ADC_Cmd(MS1100_ADC, ENABLE);
}

/**
 * @brief  ���ɼ����ĵ�ѹֵת������ppmΪ��λ��ʾ�ĸ�����ֵ
 *        ��������Ĺ�ʽ��ֱ�ӿ��ֲ��е�ͼƬ��0ppm��1ppm��б�ʼ���ģ�
 *        ���ֻ�ܲ��0~1ppm���������ܴ�
 *
 * @param  adc_val �Ӽ�ȩ��������ģ������ɼ����ĵ�ѹֵ
 * @retval ppm ת���õģ���ppmΪ��λ�ĸ�����ֵ
 */
float MS1100_ADCValToPPM(u16 adc_val)
{
    float ppm = 0;
    ppm = 1.46 * (adc_val * 3.3 / 4096);
    return ppm;
}

float MS1100_PPMToMG(float ppm)
{
    float mg_per_cubic_meter = 0;
    mg_per_cubic_meter = (float)1.341 * ppm;
    return mg_per_cubic_meter;
}

void MS1100_Get_Mg_per_cubic_meter(void)
{
    u16 adc_value = 0;
    float ppm = 0;

    // ����ADCת��
    ADC_SoftwareStartConv(ADC1);

    // �ȴ�ת����� EOC��־
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;

    // ��ȡADCת�����  12bit���� value 0 ~ 4095
    adc_value = ADC_GetConversionValue(ADC1);
    printf("adc_value = %d\r\n", adc_value);
    ppm = MS1100_ADCValToPPM(adc_value);
    printf("ppm: %f\r\n", ppm);

    hcho_mg_per_cubic_meter = MS1100_PPMToMG(ppm);
    printf("��ȩŨ��: %f mg/m^3\r\n", hcho_mg_per_cubic_meter);
}

/**
 * @brief  MS1100��ȩģ��Ĳ��Ժ����������Ǹ���ѭ����
 *
 * @param  ��
 * @retval ��
 */
void MS1100_Test(void)
{
    u16 adc_value = 0;
    float ppm = 0;

    while (1)
    {
        // ����ADCת��
        ADC_SoftwareStartConv(ADC1);

        // �ȴ�ת����� EOC��־
        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
            ;

        // ��ȡADCת�����  12bit���� value 0 ~ 4095
        adc_value = ADC_GetConversionValue(ADC1);
        printf("adc_value = %d\r\n", adc_value);
        ppm = MS1100_ADCValToPPM(adc_value);
        printf("ppm: %f\r\n", ppm);

        delay_ms(1000);
    }
}

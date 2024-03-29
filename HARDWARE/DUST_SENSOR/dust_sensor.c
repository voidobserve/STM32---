#include "dust_sensor.h"
#include "stm32f4xx.h"
#include "delay.h" // ��ʱ�����ӿ�

#include <stdio.h>
#include "usart.h"

volatile float pm2_5_mg_per_cubic_meter = 0;

// �۳���������ʼ��
void DustSensor_Init(void)
{
    // ���Ʒ۳��ĺ���LED�� ILED��ʼ��
    // |--ʹ��PG12

    GPIO_InitTypeDef GPIO_InitStructure;

    // ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // �������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   // ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    // ����Ϊ�͵�ƽ--�������۳���������LED
    GPIO_ResetBits(GPIOG, GPIO_Pin_12);

    // ������۳������������ѹֵ�Ķ˿ڳ�ʼ������ʼ��GPIO��ADC��
    // |--Ŀǰʹ��PA4--ADC2_IN4
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 1.��ADC����Ľӿ�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

    // 2.��GPIO�˿�ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // 3.����GPIO���Ų���+��ʼ�� ע�⣺����ģʽΪģ��ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 4.����ADC����Ĳ���
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     // ����ģʽ
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;                  // Ԥ��Ƶֵ  84MHZ /2 = 42MHZ
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
    ADC_Init(ADC2, &ADC_InitStructure);

    // 6.����ADC�Ĺ���ͨ��  ��ת��ʱ�� = 3 + 12 = 15����
    ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);

    // 7.ʹ��ADC
    ADC_Cmd(ADC2, ENABLE);
}

void DustSensor_GetVal(void)
{
    u16 adc_value = 0;

    DUST_LED_SET(0);
    delay_us(5);

    DUST_LED_SET(1);
    delay_us(280);

    // ����ADCת��
    ADC_SoftwareStartConv(ADC2);

    // �ȴ�ת����� EOC��־
    while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET)
        ;

    // ��ȡADCת�����  12bit���� value 0 ~ 4095
    adc_value = ADC_GetConversionValue(ADC2);

    DUST_LED_SET(0);
    delay_us(5);

    printf("adc_value = %d\r\n", adc_value);

    pm2_5_mg_per_cubic_meter = adc_value * 3.3 * 5.8 / 4096;
    printf("�۳�Ũ�ȣ� %lf mg/m^3\r\n", pm2_5_mg_per_cubic_meter);
}

void DustSensor_Test(void)
{
    u16 adc_value = 0;

    while (1)
    {
        DUST_LED_SET(0);
        delay_us(5);

        DUST_LED_SET(1);
        delay_us(280);

        // ����ADCת��
        ADC_SoftwareStartConv(ADC2);

        // �ȴ�ת����� EOC��־
        while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET)
            ;

        // ��ȡADCת�����  12bit���� value 0 ~ 4095
        adc_value = ADC_GetConversionValue(ADC2);

        DUST_LED_SET(0);
        delay_us(5);

        printf("adc_value = %d\r\n", adc_value);

        printf("�۳�Ũ�ȣ� %lf mg/m^3\r\n", adc_value * 3.3 * 5.8 / 4096);
        delay_ms(1000);
    }
}

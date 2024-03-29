#include "light_sensor.h"
#include "stm32f4xx.h"

// ���������ʼ��
void LightSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	// 1.��ADC����Ľӿ�ʱ��  PF7 -- ADC3_IN5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	// 2.��GPIO�˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	// 3.����GPIO���Ų���+��ʼ�� ע�⣺����ģʽΪģ��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	// 4.����ADC����Ĳ���
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					 // ����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					 // Ԥ��Ƶֵ  84MHZ /2 = 42MHZ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		 // ��ʹ��DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; // ���β����ļ��ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);

	// 5.����ADC�����ת�����ȡ����ݶ���
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;						// ת������  12bit
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								// ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							// ������ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // ��ʹ���ⲿ����Դ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						// ���ݶ���  �Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;									// ת�����
	ADC_Init(ADC3, &ADC_InitStructure);

	// 6.����ADC�Ĺ���ͨ��  ��ת��ʱ�� = 3 + 12 = 15����
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	// 7.ʹ��ADC
	ADC_Cmd(ADC3, ENABLE);
}

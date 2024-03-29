/*************************************************
 Copyright ? 0010. All rights reserved.

// �ļ���
@ File name: dht11.c -- dht11��ʪ�ȴ�����������Դ����
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
		   dht11��ʪ�ȴ�����������Դ����

@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#include <stdbool.h>

#include "stm32f4xx.h"
#include "dht11.h"
#include "delay.h" // ��������ʱ

// ==========================================
// ================================
// DHT11����ʹ�õ��ĺ����ӿ�
#include "usart.h"
#include <stdio.h>
// ================================
// ==========================================

uint8_t dhtbuf[5] = {0};

/**
 * @brief   DHT11��Ӧ�˿ڵĳ�ʼ������
 *
 * @param   ��
 * @retval  ��
 */
void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ��GPIOG�����ʱ��
	DHT11_DATA_RCC_CMD(DHT11_DATA_RCC, ENABLE);

	// ������Ӧ��GPIO����ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // �������
	GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;	   // ��DHT11��DATA������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // ����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DHT11_DATA_PORT, &GPIO_InitStructure);

	// Ĭ���������ŵ�ƽ
	DHT11_SET(1);
}

/**
 * @brief  ��DHT11��Ӧ�˿�����Ϊ����ģʽ
 *
 * @param  ��
 * @retval ��
 */
void DHT11_SetInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	DHT11_DATA_RCC_CMD(DHT11_DATA_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DHT11_DATA_PORT, &GPIO_InitStructure);
}

/**
 * @brief  ��DHT11��Ӧ�˿�����Ϊ���ģʽ
 *
 * @param  ��
 * @retval ��
 */
void DHT11_SetOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	DHT11_DATA_RCC_CMD(DHT11_DATA_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DHT11_DATA_PORT, &GPIO_InitStructure);
}


/**
 * @brief  ��DHT11���Ϳ�ʼ�ź�
 * 
 * @param  ��
 * @retval ��
*/
void DHT11_SendStart(void)
{
	// ������һ��������ʲô״̬���������ó����ģʽ
	DHT11_SetOutputMode();

	// ȷ������Ϊ����״̬
	DHT11_SET(1);
	delay_us(5);

	// ����������20ms��Ҫ�����18ms��
	DHT11_SET(0);
	delay_ms(20);

	// ����������30us��Ҫ��20~40us��
	DHT11_SET(1);
	delay_us(30);
}


/**
 * @brief  �ȴ�DHT11��Ӧ���źţ�����ʱ�жϣ�
 * 
 * @param  ��
 * @retval true--DHT11��Ӧ�ˣ�false--DHT11�ڳ�ʱ�����δ������Ӧ
*/ 
bool DHT11_GetAck(void)
{
	int cnt = 0; // ���ڳ�ʱ�ļ���

	// 1.��������Ϊ����ģʽ
	DHT11_SetInputMode();

	// 2.�ȴ�DHT11ģ�����������  Ϊ����߳���ɿ��ԣ�Ӧ����Ϊʹ�ó�ʱ����  100us
	while (DHT11_READ == SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}

	// ����ڳ�ʱ����ڶ�û����Ӧ��˵����ʱ��
	if (cnt >= 100)
		return false;

	cnt = 0;

	// 3.���DHT11�����ŵ�ƽ���ͣ�����Ҫ�����Ƿ�����ʱ��Ϊ80us����
	while (DHT11_READ == RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}

	if (cnt >= 100)
		return false; // ˵��δ��Ӧ��ԭ���ǳ�ʱ��
	return true; // ˵����Ӧ��
}


/** 
 * @brief  ��DHT11��ȡһλ���ݣ�����ʱ�жϣ�
 * 
 * @param  databit��Ҫ���һλ���������ݵ��������ں����ڣ��������������ֵ��ǰ�Ƕ��٣�
 * 		ֻҪ���Ǵ����˳�������ֵ�Ϳ϶���0��1
 * @retval true--DHT11��Ӧ�ˣ�false--DHT11�ڳ�ʱ�����δ������Ӧ
*/
bool DHT11_GetDataBit(u8 *databit)
{
	int cnt = 0; // ���ڼ���

	// 1.�ȴ�DHT11�����ŵ�ƽ����
	while (DHT11_READ == SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}

	// ����ڳ�ʱ����ڶ�û����Ӧ��˵����ʱ��
	if (cnt >= 100)
		return false; 

	cnt = 0;

	// 2.�ȴ�DHT11�����ŵ�ƽ����
	while (DHT11_READ == RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}

	// ����ڳ�ʱ����ڶ�û����Ӧ��˵����ʱ��
	if (cnt >= 100)
		return false; 

	// 3.��ʱ28us < us < 70us
	delay_us(40);

	*databit = DHT11_READ; // ���ض�ȡ����һλ����

	return true;
}


/**
 * @brief  ��ȡDHT11����ʪ������
 * 
 * @param  buf ���DHT11���ص����ݣ�
 * 		����DHT11���ص���5���ֽڵ����ݣ��������Ҫ������5���ֽڵĴ�С
 * @retval true--�ɹ���ȡ�������ݣ�false--DHT11��ʱδ��Ӧ����DHT11���͵���������
*/ 
bool DHT11_GetData(u8 buf[5])
{
	int i, j;
	u8 databit = 0; // �������һλ����������
	uint8_t data = 0; // ���ڴ洢1�ֽ�  8bit

	// 1.���Ϳ�ʼ�ź�
	DHT11_SendStart();

	// 2.�ȴ�DHT11��Ӧ
	if (DHT11_GetAck())
	{
		// 3.ѭ����ȡ40bit
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 8; j++)
			{
				data <<= 1;
				DHT11_GetDataBit(&databit); // ����û�н��д��������������ѡ���ڵ��Խ׶ν���
				data |= databit;
			}

			buf[i] = data;
			data = 0;
		}

		// 4.����У��
		if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		return false; // ��ȡ����ʧ�ܣ�ԭ����δ��Ӧ
	}
}

// DHT11���Ժ������ڲ��Ǹ���ѭ��
// �����ڲ��Ѿ��Լ���ʼ����ʹ�õ������裬����DHT11����
// ��Ҫ�õ�����1��printf����
void DHT11_Test(void)
{
	char buf[128] = {0};

	uint8_t dhtbuf[5] = {0};

	// 1.Ӳ����ʼ��
	uart_init(9600);
	DHT11_Init();
	
	while (1)
	{
		DHT11_GetData(dhtbuf);
		// ��GB2312�����£���������Ż�����
		sprintf(buf, "�¶� = %d ��,ʪ��=%d %%RH \n\n", dhtbuf[2], dhtbuf[0]);
		printf("%s\r\n", buf);
		delay_ms(500);
		delay_ms(500);
	}
}

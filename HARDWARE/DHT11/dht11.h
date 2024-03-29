/*************************************************
 Copyright ? 0010. All rights reserved.
// �ļ���
@ File name: dht11.h -- dht11��ʪ�ȴ�������ͷ�ļ�
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
            dht11��ʪ�ȴ�������ͷ�ļ�

@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#ifndef __DHT11_H
#define __DHT11_H

#include <stdbool.h>
#include "stm32f4xx.h"

#include "my_config.h"

extern uint8_t dhtbuf[5];

// =================================================================
// ====================================================
//-----------------��ʪ�ȴ�����DHT11�˿���ض���----------------

// ��ʪ�ȴ�����DHT11��DATA�������Ŷ�Ӧ�Ķ˿�
#ifndef DHT11_DATA_PORT 
#define DHT11_DATA_PORT             GPIOG
#endif 
// ��ʪ�ȴ�����DHT11��DATA�������Ŷ�Ӧ�Ķ˿ڵ����ź�
#ifndef DHT11_DATA_PIN
#define DHT11_DATA_PIN              GPIO_Pin_9
#endif
// ��ʪ�ȴ�����DHT11��DATA�������ŵ�ʱ��ʹ�ܺ���
#ifndef DHT11_DATA_RCC_CMD
#define DHT11_DATA_RCC_CMD          RCC_AHB1PeriphClockCmd
#endif
// ��ʪ�ȴ�����DHT11��DATA�������ŵ�ʱ��
#ifndef DHT11_DATA_RCC
#define DHT11_DATA_RCC              RCC_AHB1Periph_GPIOG
#endif

//-----------------��ʪ�ȴ�����DHT11�˿���ض���----------------
// ====================================================
// =================================================================

#define DHT11_SET(n) \
       ((n) ? GPIO_SetBits(DHT11_DATA_PORT, DHT11_DATA_PIN) : GPIO_ResetBits(DHT11_DATA_PORT, DHT11_DATA_PIN))

#define DHT11_READ          GPIO_ReadInputDataBit(DHT11_DATA_PORT, DHT11_DATA_PIN)

void DHT11_Init(void); // DHT11��Ӧ�˿ڵĳ�ʼ������

void DHT11_SetInputMode(void);  // ��DHT11��Ӧ�˿�����Ϊ����ģʽ
void DHT11_SetOutputMode(void); // ��DHT11��Ӧ�˿�����Ϊ���ģʽ

void DHT11_SendStart(void); // ��DHT11���Ϳ�ʼ�ź�
bool DHT11_GetAck(void);    // �ȴ�DHT11��Ӧ���źţ�����ʱ�жϣ�

bool DHT11_GetDataBit(u8 *databit); //��DHT11��ȡһλ���ݣ�����ʱ�жϣ�
bool DHT11_GetData(u8 buf[5]);      // ��ȡDHT11����ʪ������

void DHT11_Test(void); // DHT11���Ժ������ڲ��Ǹ���ѭ����һ��ֻ�ڵ��Ե�ʱ��ʹ��

#endif

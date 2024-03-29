/*************************************************
 Copyright ? 0010. All rights reserved.
// �ļ���
@ File name: ultraviolet_light.h -- ������LED��ͷ�ļ�
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
            ������LED��ͷ�ļ�
          ������LEDʹ��5V���磬ͨ����Ƭ�����Ƽ̵����Ŀ���������������LED��
          �̵����Ŀ��ƶ�����Ϊ�͵�ƽʹ�ܡ�

@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#ifndef __ULTRAVIOLET_H
#define __ULTRAVIOLET_H

#include "stm32f4xx.h"
#include "my_config.h" 

// =================================================================
// ====================================================
//-----------------������LED���ƶ˿���ض���----------------

// ������LED�������Ŷ�Ӧ�Ķ˿�
#ifndef ULTRAVIOLET_LIGHT_PORT 
#define ULTRAVIOLET_LIGHT_PORT             GPIOC
#endif 
// ������LED�������Ŷ�Ӧ�Ķ˿ڵ����ź�
#ifndef ULTRAVIOLET_LIGHT_PIN
#define ULTRAVIOLET_LIGHT_PIN              GPIO_Pin_7
#endif
// ������LED�������Ŷ�Ӧ�Ķ˿ڵ�ʱ��ʹ�ܺ���
#ifndef ULTRAVIOLET_LIGHT_RCC_CMD
#define ULTRAVIOLET_LIGHT_RCC_CMD          RCC_AHB1PeriphClockCmd
#endif
// ������LED�������Ŷ�Ӧ�Ķ˿ڵ�ʱ��
#ifndef ULTRAVIOLET_LIGHT_RCC
#define ULTRAVIOLET_LIGHT_RCC              RCC_AHB1Periph_GPIOC
#endif

//-----------------������LED���ƶ˿���ض���----------------
// ====================================================
// =================================================================

// ������LED������������ߵ͵�ƽ�ĺ꺯����0--����͵�ƽ��1--����ߵ�ƽ
#define ULTRAVIOLET_LIGHT_SET(n)   \
     ((n) ? GPIO_SetBits(ULTRAVIOLET_LIGHT_PORT, ULTRAVIOLET_LIGHT_PIN) : GPIO_ResetBits(ULTRAVIOLET_LIGHT_PORT, ULTRAVIOLET_LIGHT_PIN))

void UlrtavioletLight_Init(void); // ������LED���ƶ˵ĳ�ʼ��

#endif


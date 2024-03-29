/*************************************************
 Copyright ? 0010. All rights reserved.
// �ļ���
@ File name: fan.h -- ���ȵ����������ͷ�ļ�
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:
            ���ȵ����������ͷ�ļ�

			ע��: ���ȵ�PWM������Ҫ����������ʱ����ʱ��Ƶ�ʺ͵��Ի�ã�
				 ��˾����ʹ���п��ܻ�Ҫ�޸����еĳ�ʼ������
                     
@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#ifndef __FAN_H
#define __FAN_H

#include "my_config.h"

enum 
{
	CUR_FAN_SPEED_CLOSE = 0,
	CUR_FAN_SPEED_MIN,
	CUR_FAN_SPEED_LOW,
	CUR_FAN_SPEED_MID,
	CUR_FAN_SPEED_MAX
};

extern volatile int cur_fan_speed; 

// =================================================================
// ====================================================
//-----------------���ȿ��ƶ˿���ض���----------------

// ���ȿ������Ŷ�Ӧ�Ķ˿�
#ifndef FAN_PORT 
#define FAN_PORT             GPIOB
#endif 
// ���ȿ������Ŷ�Ӧ�Ķ˿ڵ����ź�
#ifndef FAN_PIN
#define FAN_PIN              GPIO_Pin_3
#endif
// ���ȿ������Ŷ�Ӧ�Ķ˿ڵ�ʱ��ʹ�ܺ���
#ifndef FAN_RCC_CMD
#define FAN_RCC_CMD          RCC_AHB1PeriphClockCmd
#endif
// ���ȿ������Ŷ�Ӧ�Ķ˿ڵ�ʱ��
#ifndef FAN_RCC
#define FAN_RCC              RCC_AHB1Periph_GPIOB
#endif
// ���ȿ������Ÿ��õ�������Դ
#ifndef FAN_PIN_SOURCE
#define FAN_PIN_SOURCE       GPIO_PinSource3
#endif
// ���ȿ������Ÿ��õ�������Դ
#ifndef FAN_AF_PERIPHERAL
#define FAN_AF_PERIPHERAL    GPIO_AF_TIM2
#endif
// ���ȿ������Ŷ�Ӧ�Ķ�ʱ��
#ifndef FAN_TIM
#define FAN_TIM              TIM2
#endif
// ���ȿ������Ŷ�Ӧ�Ķ�ʱ����ʱ��ʹ�ܺ���
#ifndef FAN_TIM_RCC_CMD
#define FAN_TIM_RCC_CMD      RCC_APB1PeriphClockCmd
#endif
// ���ȿ������Ŷ�Ӧ�Ķ�ʱ����ʱ��
#ifndef FAN_TIM_RCC
#define FAN_TIM_RCC          RCC_APB1Periph_TIM2
#endif
// ���ȿ������Ŷ�Ӧ�Ķ�ʱ�������ͨ��ʹ�ܺ���
#ifndef FAN_TIM_OC_INIT
#define FAN_TIM_OC_INIT      TIM_OC2Init
#endif
// ���ȿ������Ŷ�Ӧ�Ķ�ʱ��ͨ����Ԥװ�ؼĴ���ʹ�ܺ���
#ifndef FAN_TIM_PRELOAD_CONFIG
#define FAN_TIM_PRELOAD_CONFIG TIM_OC2PreloadConfig
#endif


//-----------------���ȿ��ƶ˿���ض���----------------
// ====================================================
// =================================================================

#define FAN_SET_CLOSE()              TIM_SetCompare2(FAN_TIM, 0)  // �رշ���
#define FAN_SET_MIN_SPEED()          TIM_SetCompare2(FAN_TIM, 65) // ��������������
#define FAN_SET_LOW_SPEED()          TIM_SetCompare2(FAN_TIM, 75) // ��Щ������
#define FAN_SET_MID_SPEED()          TIM_SetCompare2(FAN_TIM, 80) // ������������
#define FAN_SET_MAX_SPEED()          TIM_SetCompare2(FAN_TIM, 99) // �������ת��

// void FAN_Init(void); // �����ܲ���ͨ���̵������Ʒ��ȵĳ���
void FAN_PWM_Init(void); // ��ʱ��ͨ��PWM���Ʒ��ȵĳ�ʼ������


// ���ݼ�¼����ת�ٵı������ڷ���ת��
void FAN_Speed_Adjust(void);
void FAN_Speed_Inc(void);
void FAN_Speed_dec(void);

#endif


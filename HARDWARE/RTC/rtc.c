#include "rtc.h"
#include "stm32f4xx.h"
#include <string.h>

#include "usart.h"

volatile uint32_t uwTimeDisplay = 0; // ��ʾRTC�����жϵı�־

void My_RTC_Init(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeTypeDef RTC_TimeStructure;

    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    // 1.��PWR����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // 2.ʹ��RTC�Ĵ�����д����
    PWR_BackupAccessCmd(ENABLE);

    // 3.ʹ��LSE�����ⲿʱ�� 32.768KHZ  ���ȸ�  ���ĵ�
    RCC_LSEConfig(RCC_LSE_ON);

    // 4.�ȴ�LSE�����ⲿʱ�Ӿ���
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        ;

    // 5.ѡ��LSE��ΪRTC�����ʱ��Դ  ���Է�Ƶ�õ�׼ȷ��1HZ
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    // 6.��RTC�����ʱ��
    RCC_RTCCLKCmd(ENABLE);

    // 7.�ȴ�RTC�Ĵ���ͬ�����
    RTC_WaitForSynchro();

    // 8.��RTC����ʱ�ӷ�Ƶ����ʼ��  32768HZ / (127+1) * (255+1) = 1HZ
    RTC_InitStructure.RTC_AsynchPrediv = 128 - 1;
    RTC_InitStructure.RTC_SynchPrediv = 256 - 1;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; // 24Сʱ��
    RTC_Init(&RTC_InitStructure);

    // 9.����RTC������  2024/2/26
    RTC_DateStructure.RTC_Year = 0x24;    // ���
    RTC_DateStructure.RTC_Month = 0x02;   // �·�
    RTC_DateStructure.RTC_Date = 0x26;    // ����
    RTC_DateStructure.RTC_WeekDay = 0x01; // ����
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

    // 10.����RTC��ʱ�� 15:51:30
    RTC_TimeStructure.RTC_H12 = RTC_H12_PM; // ����
    RTC_TimeStructure.RTC_Hours = 0x15;     // ʱ��
    RTC_TimeStructure.RTC_Minutes = 0x51;   // ����
    RTC_TimeStructure.RTC_Seconds = 0x30;   // ����
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

    // 11.���û����жϲ���  NVIC�Ĳ���
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 12.����EXTI22�ⲿ�ж� ���ӵ�RTC�����ж�
    EXTI_ClearITPendingBit(EXTI_Line22);
    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 13.���RTC�Ļ����жϵı�־
    RTC_ClearITPendingBit(RTC_IT_WUT);

    // 14.�ر�RTC���Ѷ�ʱ��
    RTC_WakeUpCmd(DISABLE);

    // 15.ѡ��RTC����ʱ��Դ  ѡ���ڲ�ʱ��(1HZ)
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

    // 16.���û��Ѽ�������ֵ ����Ϊ0����ʾ����һ�ξʹ�������
    RTC_SetWakeUpCounter(0);

    // 17.ѡ�����ж�  ѡ���ж�Դ
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    // 18.�򿪻��Ѷ�ʱ��
    RTC_WakeUpCmd(ENABLE);
}

// ͨ����⴮�ڷ��͹�������Ϣ������RTC�ڵ�ʱ��/������Ϣ
// settime-hh-mm-ss
// setdate-yy-mm-dd-weekday
void My_RTC_UpDate(void)
{
    RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeTypeDef RTC_TimeStructure;
    u8 temp = 0;

    // settime-hh-mm-ss
    if (strncmp((const char *)USART1_RX_BUF, "settime", strlen("settime")) == 0)
    {
        // ����յ���Ҫ����ʱ�������
        temp = (USART1_RX_BUF[8] - '0') * 10;
        temp += USART1_RX_BUF[9] - '0';
        if (temp > 12)
        {
            // ���Сʱ���ֵ�ʱ�����12��˵��������
            RTC_TimeStructure.RTC_H12 = RTC_H12_PM;
        }
        else
        {
            RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
        }

        RTC_TimeStructure.RTC_Hours = (temp / 10) * 16 + (temp % 10);

        temp = (USART1_RX_BUF[11] - '0') * 10;
        temp += USART1_RX_BUF[12] - '0';
        RTC_TimeStructure.RTC_Minutes = (temp / 10) * 16 + (temp % 10);

        temp = (USART1_RX_BUF[14] - '0') * 10;
        temp += USART1_RX_BUF[15] - '0';
        RTC_TimeStructure.RTC_Seconds = (temp / 10) * 16 + (temp % 10);

        RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

        // memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF) / sizeof(USART_RX_BUF[0]));
    }
    else if (strncmp((const char *)USART1_RX_BUF, "setdate", strlen("setdate")) == 0)
    {
        // setdate-yy-mm-dd-weekday
        // ����յ���Ҫ����ʱ�������
        temp = (USART1_RX_BUF[8] - '0') * 10;
        temp += USART1_RX_BUF[9] - '0';
        RTC_DateStructure.RTC_Year = (temp / 10) * 16 + (temp % 10);

        temp = (USART1_RX_BUF[11] - '0') * 10;
        temp += USART1_RX_BUF[12] - '0';
        RTC_DateStructure.RTC_Month = (temp / 10) * 16 + (temp % 10);

        temp = (USART1_RX_BUF[14] - '0') * 10;
        temp += USART1_RX_BUF[15] - '0';
        RTC_DateStructure.RTC_Date = (temp / 10) * 16 + (temp % 10);

        temp = USART1_RX_BUF[17] - '0';
        RTC_DateStructure.RTC_WeekDay = temp;
        RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

        // memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF) / sizeof(USART_RX_BUF[0]));
    }
}

// ��RTC��ʱ���������Ϣ���µ�����1
void My_RTC_ShowReflesh(void)
{
    RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeTypeDef RTC_TimeStructure;
    uint8_t aShowTime[50] = {0}; // ���ڴ洢RTC��ϵͳʱ��

    // ��ȡRTCʱ��
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

    // ��RTCʱ�����ת��
    sprintf((char *)aShowTime, "ʱ�䣺%0.2d:%0.2d:%0.2d", RTC_TimeStructure.RTC_Hours,
            RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
    printf("%s\r\n", aShowTime);
    memset(aShowTime, 0, sizeof(aShowTime) / sizeof(aShowTime[0]));

    sprintf((char *)aShowTime, "���ڣ�20%0.2d-%0.2d-%0.2d-����%0.1d", RTC_DateStructure.RTC_Year,
            RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_WeekDay);
    printf("%s\r\n", aShowTime);
    // memset(aShowTime, 0, sizeof(aShowTime) / sizeof(aShowTime[0]));

    // memset(&RTC_TimeStructure, 0, sizeof(RTC_TimeStructure));
    // memset(&RTC_DateStructure, 0, sizeof(RTC_DateStructure));
}

// RTC��������
void My_RTC_AlarmInit(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RTC_AlarmTypeDef RTC_AlarmStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1.��PWR����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // 2.ʹ��RTC�Ĵ�����д����
    PWR_BackupAccessCmd(ENABLE);

    // 3.ʹ��LSE�����ⲿʱ�� 32.768KHZ  ���ȸ�  ���ĵ�
    RCC_LSEConfig(RCC_LSE_ON);

    // 4.�ȴ�LSE�����ⲿʱ�Ӿ���
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        ;

    // 5.ѡ��LSE��ΪRTC�����ʱ��Դ  ���Է�Ƶ�õ�׼ȷ��1HZ
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    // 6.��RTC�����ʱ��
    RCC_RTCCLKCmd(ENABLE);

    // 7.�ȴ�RTC�Ĵ���ͬ�����
    RTC_WaitForSynchro();

    // �����ⲿ�ж���
    EXTI_ClearITPendingBit(EXTI_Line17); // ���RTC���ӵ��жϱ�־
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // ʹ��RTC�����ж�
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RTC_ClearFlag(RTC_FLAG_ALRAF); // �������A�ı�־

    // ��������A�����루�������ӵĲ�����
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;                     // ����
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;              // ���ӵĴ����������޹أ���ÿ�춼�ᴥ������
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date; // ���Ӵ�����������һ���й�
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x01;                            // ����
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x00;                         // Сʱ
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;                       // ����
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x00;                       // ��
    RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

    /* Set alarm A sub seconds and enable SubSec Alarm : generate 8 interrupts per Second */
    // ��������A�����룬��ʹ����������
    // RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_5);

    // ʹ������A���ж�
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);

    // �������A���жϱ�־
    RTC_ClearITPendingBit(RTC_IT_ALRA);

    // ������A
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

// ͨ�����մ��ڷ��͵���������������
// setalarm-hh-mm-ss
void My_RTC_SetAlarm(void)
{
    RTC_AlarmTypeDef RTC_AlarmStructure;
    u8 temp = 0;

    // setalarm-hh-mm-ss
    if (strncmp((const char *)USART1_RX_BUF, "setalarm", strlen("setalarm")) == 0)
    {
        // �Ƚ��RTC�Ĵ�����д����

        // 1.��PWR����ʱ��
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

        // 2.ʹ��RTC�Ĵ�����д����
        PWR_BackupAccessCmd(ENABLE);

        // 3.ʹ��LSE�����ⲿʱ�� 32.768KHZ  ���ȸ�  ���ĵ�
        RCC_LSEConfig(RCC_LSE_ON);

        // 4.�ȴ�LSE�����ⲿʱ�Ӿ���
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
            ;

        // 5.ѡ��LSE��ΪRTC�����ʱ��Դ  ���Է�Ƶ�õ�׼ȷ��1HZ
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        // 6.��RTC�����ʱ��
        RCC_RTCCLKCmd(ENABLE);

        // 7.�ȴ�RTC�Ĵ���ͬ�����
        RTC_WaitForSynchro();

        RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

        RTC_ClearFlag(RTC_FLAG_ALRAF); // �������A�ı�־

        // ����յ���Ҫ����ʱ�������
        temp = (USART1_RX_BUF[9] - '0') * 10;
        temp += USART1_RX_BUF[10] - '0';

        if (temp > 12)
        {
            RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;
        }
        else
        {
            RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
        }

        RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = (temp / 10) * 16 + (temp % 10);

        temp = (USART1_RX_BUF[12] - '0') * 10;
        temp += USART1_RX_BUF[13] - '0';
        RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = (temp / 10) * 16 + (temp % 10);

        temp = (USART1_RX_BUF[15] - '0') * 10;
        temp += USART1_RX_BUF[16] - '0';
        RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = (temp / 10) * 16 + (temp % 10);

        RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
        RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
        RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;
        RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;                     // ����
        RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;              // ���ӵĴ����������޹أ���ÿ�춼�ᴥ������
        RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date; // ���Ӵ�����������һ���й�
        RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

        // �������A���жϱ�־
        RTC_ClearITPendingBit(RTC_IT_ALRA);

        RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

        printf("set alarm success\r\n");
    }
}

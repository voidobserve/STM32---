/*************************************************
 Copyright ? 0010. All rights reserved.

// �ļ���
@ File name: main.c -- ����������λ��
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
// ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
@ Description:

            ע�⣬����ʹ��Use MicroLIB�����빤�̣��������ļ�ȩŨ�Ⱥ�PM2.5Ũ�ȵ���ֵ��ܴ�

@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"   // FreeRTOS����
#include "semphr.h" // FreeRTOS���������ź���

#include "stm32f4xx.h"
#include "usart.h" // ����1
#include "delay.h" // systick��ʱ
#include "led.h"   // �������Դ����ĸ�LED
#include "beep.h"  // ������ķ�����
#include "key.h"   // ������İ���
// #include "rtc.h"
#include "tim.h"

#include "dht11.h"             // ��ʪ�ȴ�����
#include "ultraviolet_light.h" // ������LED
#include "fan.h"               // ����
#include "ms1100.h"            // MS1100��ȩ������
#include "dust_sensor.h"
#include "oled.h" // 0.96'OLED
#include "usart3.h"
#include "esp8266.h"
#include "mqtt.h"
#include "usart2.h" // ����2��ʼ��--������ʼ��

#include "interface.h"         // ��������
#include "child_safety_lock.h" // ��ͯ��

TaskHandle_t Task_Initilize_Handle = NULL;   // ��ų�ʼ������ľ��
TaskHandle_t Task_ESP8266Init_Handle = NULL; // ���ESP8266��ʼ���Լ����Ӱ���������ľ��
TaskHandle_t Task_Watch_Handle = NULL;       // ��ż�������ľ��

TaskHandle_t Task_OLEDShow_Handle = NULL; // ���OLED��ʾ����ľ��

TaskHandle_t Task_GetDeviceInfoPeriod_Handle = NULL; // ��������Ի�ȡ�豸��Ϣ����ľ��
TaskHandle_t Task_MQTTPubilshPeriod_Handle = NULL;   // ���MQTT�����Է�����Ϣ����ľ��
TaskHandle_t Task_MQTTHeartBeatPacket_Handle = NULL; // ���MQTT�����Է�������������ľ��

void Initialize_Task(void *arg);
void ESP8266_Connect_Task(void *arg);
void OLED_Show_Task(void *arg);
void MQTT_Pubilsh_Period_Task(void *arg);
void MQTT_Send_HeartBeatPacket_Task(void *arg);

void Watch_Task(void *arg);

// Ӳ����ʼ�����񣬸�����ִֻ��һ��
void Initialize_Task(void *arg)
{
    // �ж����ȼ�����Ӧ�÷���Ӳ����ʼ��֮ǰ
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // ����ϵͳ�ж����ȼ�����2
    // 1.Ӳ����ʼ��
    uart_init(9600);
    printf("Hardware is Initing....\r\n");
    LED_Init();
    // KEY_Init();
    KEY_IT_Init();
    UlrtavioletLight_Init(); // ������LED��ʼ��
    FAN_PWM_Init();          // PWM���ȳ�ʼ��
    DHT11_Init();            // ��ʪ�ȴ�������ʼ��
    MS1100_Init();
    DustSensor_Init();
    USART3_Init(115200); // esp8266ʹ�õ��Ĵ��ڳ�ʼ��
    USART2_Init(9600);   // ������ʼ��

    OLED_Init();
    TIM6_Init();

    OLED_Clear_Graphic_MEM();
    OLED_Clear(); // OLED����

    printf("Hardware Init is OK\r\n");

    vTaskDelete(NULL); // ��ʼ����ɺ󣬽���������
}

// ESP8266��ʼ���Լ����Ӱ�����������ƽ̨�����񣬸�����ִֻ��һ��
void ESP8266_Connect_Task(void *arg)
{
    BaseType_t xReturned; // ���洴���������ķ���ֵ

    printf("ESP8266 is Initing....\r\n");

    // ����WiFi��STA����ģʽ����
    while (Esp8266_Init())
        ;

    printf("ESP8266 Init is OK\r\n");

    // ���Ӱ����Ʒ�����
    while (Mqtt_Connect_Aliyun())
        ; // ����MQTT���Ӱ�����

    // ��������
    xReturned = xTaskCreate(MQTT_Pubilsh_Period_Task,     // ����ĺ���ָ��
                            "MQTT_Pubilsh_Period_Task",   // ���������
                            256,                          // �����ջ��С
                            NULL,                         // ����Ҫ�����񴫲Σ���NULL
                            3,                            // ��������ȼ�
                            Task_MQTTPubilshPeriod_Handle // ����ľ��
    );

    // �ж������Ƿ񴴽��ɹ�
    if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xReturned)
    {
        printf("MQTT�����Է�����Ϣ���񴴽�ʧ��\r\n");
    }

    // ��������
    xReturned = xTaskCreate(MQTT_Send_HeartBeatPacket_Task,   // ����ĺ���ָ��
                            "MQTT_Send_HeartBeatPacket_Task", // ���������
                            256,                              // �����ջ��С
                            NULL,                             // ����Ҫ�����񴫲Σ���NULL
                            3,                                // ��������ȼ�
                            Task_MQTTHeartBeatPacket_Handle   // ����ľ��
    );

    // �ж������Ƿ񴴽��ɹ�
    if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xReturned)
    {
        printf("MQTT�����Է������������񴴽�ʧ��\r\n");
    }

    vTaskDelete(NULL); // ��ʼ����ɺ󣬽���������
}

// ��������
void Watch_Task(void *arg)
{
    while (1)
    {
        vTaskSuspendAll();
        if (1 == USART2_RX_FALG)
        {

            printf("�����յ�������: %s \r\n", USART2_RX_BUF);

            if (strstr((const char *)USART2_RX_BUF, "UNLOCK"))
            {
                lock = false;
            }
            else if (strstr((const char *)USART2_RX_BUF, "LOCK"))
            {
                lock = true;
            }

            USART2_RX_FALG = 0;
        }

        mqtt_msg_handle();
        FAN_Speed_Adjust(); // ���ݴ�ŷ���ת�ٵ�ȫ�ֱ��������ڷ���ת��

        // ���ݷ����Ƿ�ת��������������LED
        if (CUR_FAN_SPEED_CLOSE < cur_fan_speed)
        {
            ULTRAVIOLET_LIGHT_SET(0); // ������LED��
        }
        else
        {
            ULTRAVIOLET_LIGHT_SET(1); // ������LED��
        }

        xTaskResumeAll();

        vTaskDelay(500);
    }
}

// ͨ��MQTT������������������ƽ̨����������������
void MQTT_Send_HeartBeatPacket_Task(void *arg)
{
    while (1)
    {
        vTaskSuspendAll();
        // ÿ��һ��ʱ�䷢��������
        // ÿ6 * 5 s����һ��������
        if (tim6_update_cnt >= 6)
        {
            /*	�豸���ڱ���ʱ������(����ʱ����mqtt_connect����Ϊ60s)��������Ҫ����һ�α��ģ�����ping����
                ���ӱ���ʱ���ȡֵ��ΧΪ30��~1200�롣����ȡֵ300�����ϡ�
                ��������ƽ̨����CONNACK��ӦCONNECT��Ϣʱ����ʼ������ʱ���յ�PUBLISH��SUBSCRIBE��PING�� PUBACK��Ϣʱ�������ü�ʱ����
            */
            // ����������������Ƶ���������������������������һ��ʱ�䲻������Ӧ��Ϣ[��ѡ]

            if (!mqtt_send_heart())
            {
            }
            else
            {
                // ����Ͽ������ƣ�����������MQTT���Ӱ�����
                while (Mqtt_Connect_Aliyun())
                    ;
            }

            tim6_update_cnt = 0; // ����ֵ����
            printf("�������������\r\n");
        }

        xTaskResumeAll();

        vTaskDelay(4000);
    }
}

// �����Ի�ȡ�豸��Ϣ������
void Get_DeviceInfo_Task(void *arg)
{
    char *buf[50] = {0}; // �򴮿�1��ӡ����ʹ�õ��Ļ�����

    while (1)
    {
        vTaskSuspendAll();
        DHT11_GetData(dhtbuf); // ��ȡ��ʪ������
        sprintf((char *)buf, "�¶� = %d ��,ʪ��=%d %%RH \n\n", dhtbuf[2], dhtbuf[0]);
        printf("%s\r\n", (char *)buf);

        MS1100_Get_Mg_per_cubic_meter();

        DustSensor_GetVal();
        xTaskResumeAll();

        vTaskDelay(4000);
    }
}

// MQTT�����Է�����Ϣ������
void MQTT_Pubilsh_Period_Task(void *arg)
{
    while (1)
    {
        vTaskSuspendAll();

        // ����MQTT�ϴ���������
        mqtt_report_devices_status();

        mqtt_report_device_hcho(hcho_mg_per_cubic_meter);

        mqtt_report_device_pm2_5(pm2_5_mg_per_cubic_meter);

        mqtt_report_device_fanspeed(cur_fan_speed);

        mqtt_report_device_lock(lock);

        xTaskResumeAll();

        vTaskDelay(2000);
    }
}

// OLED��ʾ����
void OLED_Show_Task(void *arg)
{
    while (1)
    {
        OLED_Show_Interface();
        vTaskDelay(200);
    }
}

// �������
int main()
{

    BaseType_t xReturned; // ���洴���������ķ���ֵ

    // ��������
    xReturned = xTaskCreate(Initialize_Task,      // ����ĺ���ָ��
                            "Initialize_Task",    // ���������
                            128,                  // �����ջ��С��128 * 32 / 8 == 512�ֽڣ�32ΪCPU��ȣ�8����8bit
                            NULL,                 // ����Ҫ�����񴫲Σ���NULL
                            4,                    // ��������ȼ�
                            Task_Initilize_Handle // ����ľ��
    );

    // �ж������Ƿ񴴽��ɹ�
    if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xReturned)
    {
        printf("��ʼ�����񴴽�ʧ��\r\n");
    }

    // ��������
    xReturned = xTaskCreate(ESP8266_Connect_Task,   // ����ĺ���ָ��
                            "ESP8266_Connect_Task", // ���������
                            128,                    // �����ջ��С��128 * 32 / 8 == 512�ֽڣ�32ΪCPU��ȣ�8����8bit
                            NULL,                   // ����Ҫ�����񴫲Σ���NULL
                            3,                      // ��������ȼ�
                            Task_ESP8266Init_Handle // ����ľ��
    );

    // �ж������Ƿ񴴽��ɹ�
    if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xReturned)
    {
        printf("ESP8266��ʼ�����񴴽�ʧ��\r\n");
    }

    // ��������
    xReturned = xTaskCreate(OLED_Show_Task,      // ����ĺ���ָ��
                            "OLEDShowTask",      // ���������
                            256,                 // �����ջ��С
                            NULL,                // ����Ҫ�����񴫲Σ���NULL
                            3,                   // ��������ȼ�
                            Task_OLEDShow_Handle // ����ľ��
    );

    // �ж������Ƿ񴴽��ɹ�
    if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xReturned)
    {
        printf("OLED��ʾ���񴴽�ʧ��\r\n");
    }

    // ��������
    xReturned = xTaskCreate(Watch_Task,       // ����ĺ���ָ��
                            "Watch_Task",     // ���������
                            256,              // �����ջ��С
                            NULL,             // ����Ҫ�����񴫲Σ���NULL
                            3,                // ��������ȼ�
                            Task_Watch_Handle // ����ľ��
    );

    // �ж������Ƿ񴴽��ɹ�
    if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xReturned)
    {
        printf("�������񴴽�ʧ��\r\n");
    }

    // ��������
    // ��ȡ�豸��Ϣ����Ķ�ջ�ռ�Ҫ��һЩ
    xReturned = xTaskCreate(Get_DeviceInfo_Task,            // ����ĺ���ָ��
                            "GetInfoTask",                  // ���������
                            256,                            // �����ջ��С
                            NULL,                           // ����Ҫ�����񴫲Σ���NULL
                            3,                              // ��������ȼ�
                            Task_GetDeviceInfoPeriod_Handle // ����ľ��
    );

    // �ж������Ƿ񴴽��ɹ�
    if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xReturned)
    {
        printf("�����Ի�ȡ�豸��Ϣ���񴴽�ʧ��\r\n");
    }

    // ������Դ������
    vTaskStartScheduler();
}

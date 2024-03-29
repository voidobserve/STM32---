#ifndef __MQTT_H
#define __MQTT_H

//����ͷ�ļ�
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "esp8266.h"

//�궨��
//�˴��ǰ����Ʒ������Ĺ���ʵ����½����------------ע���޸�Ϊ�Լ����Ʒ����豸��Ϣ��������
#define MQTT_BROKERADDRESS		    "iot-060aayl4.mqtt.iothub.aliyuncs.com"
#define MQTT_CLIENTID				"k0w9gEec09K.air_cleaner|securemode=2,signmethod=hmacsha256,timestamp=1709975638990|"
#define MQTT_USARNAME				"air_cleaner&k0w9gEec09K"
#define MQTT_PASSWD					"7dd886579fcea81e460411ccef09cca9f3fd9a7eb1a6a16d026d678a3633f99d"
#define	MQTT_PUBLISH_TOPIC		    "/sys/k0w9gEec09K/air_cleaner/thing/event/property/post"
#define MQTT_SUBSCRIBE_TOPIC	    "/sys/k0w9gEec09K/air_cleaner/thing/service/property/set"

// ����&���ĵ�JSON���ݰ��ڵ������Ϣ
// #define MQTT_POST_MSG_METHOD        thing.service.property.set


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
#define CONNECT_MQTT_LED(x)	PCout(13)=(x)?0:1

//��������

//��������
int Mqtt_Connect_Aliyun(void);											//����MQTT���Ӱ�����
int32_t mqtt_connect(char *client_id,char *user_name,char *password);	//MQTT���ӷ�����
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);	//MQTT��Ϣ����
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);	//MQTT��Ϣ����
int32_t mqtt_send_heart(void);											//MQTT����������
void mqtt_report_devices_status(void);									//�豸״̬�ϱ�
void mqtt_report_device_hcho(float hcho_mg_per_cubic_meter);            // ������ȩŨ�ȵ���ֵ
void mqtt_report_device_pm2_5(float pm2_5_mg_per_cubic_meter);          // ����PM2.5Ũ�ȵ���ֵ
void mqtt_report_device_fanspeed(int cur_fan_speed);                    // ��������ת��
void mqtt_report_device_lock(bool lock);                                // ������ͯ����״̬

void mqtt_disconnect(void);												//MQTT�������Ͽ�
void mqtt_send_bytes(uint8_t *buf,uint32_t len);						//MQTT��������
void mqtt_msg_handle(void);												//����MQTT�·�����
uint32_t  mqtt_cjson_parse(char *pbuf);									//����MQTT�·�����
#endif

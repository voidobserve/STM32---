/*************************************************
 Copyright ? 0010. All rights reserved.

// �ļ���
@ File name: oled.h
// ���ߡ����š��汾���������
@ Author:    ya
@ ID��       0011
@ Version:
@ Date:
@ Description:
    // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
    // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
    // �ơ�˳�򡢶����������ȹ�ϵ
    ----------------------------------------------------------------
        GND   ��Դ��
        VCC   ��5V��3.3v��Դ
        SCL   ��PD6��SCL��
        SDA   ��PB7��SDA��
    ----------------------------------------------------------------
@ Others:  // �������ݵ�˵��
@ History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
|		   // �߼��޸����ݼ���
|--@ 1. Date:
|--@ Author:
|--@ ID:
|--@ Modification:
|--@ 2. ...
*************************************************/
#ifndef __OLED_H
#define __OLED_H
// #include "sys.h"
#include "stm32f4xx.h"
#include <stdlib.h>
#include <stdbool.h>

#include "my_config.h" // �Զ���������ļ�

#define OLED_MODE       0
#define SIZE            8
#define XLevelL         0x00
#define XLevelH         0x10
#define Max_Column      128
#define Max_Row         64
#define Brightness      0xFF
#define X_WIDTH         128
#define Y_WIDTH         64



//-----------------OLED IIC�˿ڶ���----------------
// =================================================================
// ====================================================
// OLED SCL �˿ڶ���
#ifndef OLED_SCL_PORT 
#define OLED_SCL_PORT           GPIOD
#endif // end OLED_SCL_PORT
// OLED SCL ���źŶ���
#ifndef OLED_SCL_PIN
#define OLED_SCL_PIN            GPIO_Pin_6
#endif // end OLED_SCL_PIN
// OLED SCL GPIOʱ��ʹ�ܺ��� ����
#ifndef OLED_SCL_RCC_CMD 
#define OLED_SCL_RCC_CMD        RCC_AHB1PeriphClockCmd
#endif // end OLED_SCL_RCC_CMD
// OLED SCL ����GPIOʱ��ʹ�ܺ����Ĳ��� ����
#ifndef OLED_SCL_RCC 
#define OLED_SCL_RCC            RCC_AHB1Periph_GPIOD
#endif // end OLED_SCL_RCC

// OLED SDA �˿ڶ���
#ifndef OLED_SDA_PORT 
#define OLED_SDA_PORT           GPIOB
#endif // end OLED_SDA_PORT
// OLED SDA ���źŶ���
#ifndef OLED_SDA_PIN
#define OLED_SDA_PIN            GPIO_Pin_7
#endif // end OLED_SDA_PIN
// OLED SDA GPIOʱ��ʹ�ܺ��� ����
#ifndef OLED_SDA_RCC_CMD 
#define OLED_SDA_RCC_CMD        RCC_AHB1PeriphClockCmd
#endif // end OLED_SDA_RCC_CMD
// OLED SDA ����GPIOʱ��ʹ�ܺ����Ĳ��� ����
#ifndef OLED_SDA_RCC 
#define OLED_SDA_RCC            RCC_AHB1Periph_GPIOB
#endif // end OLED_SDA_RCC

// ====================================================
// =================================================================


// ʹ��IO����ģ��IICʱ�򣬴Ӷ����ƴ�����
#define IIC_SDA(n)      ((n) ? GPIO_SetBits(OLED_SDA_PORT, OLED_SDA_PIN) : GPIO_ResetBits(OLED_SDA_PORT, OLED_SDA_PIN))
#define IIC_SCL(n)      ((n) ? GPIO_SetBits(OLED_SCL_PORT, OLED_SCL_PIN) : GPIO_ResetBits(OLED_SCL_PORT, OLED_SCL_PIN))

#define IIC_READ        (GPIO_ReadInputDataBit(OLED_SDA_PORT, OLED_SDA_PIN)) 

#define OLED_CMD        0  // д����
#define OLED_DATA       1  // д����

#ifndef OLED_PIXEL_X
#define OLED_PIXEL_X 128 // OLED����ֱ���
#endif
#ifndef OLED_PIXEL_Y
#define OLED_PIXEL_Y 64 // OLED����ֱ���
#endif
#ifndef OLED_PAGE_SIZE
#define OLED_PAGE_SIZE (OLED_PIXEL_Y / 8) // OLED��ҳ
#endif
extern uint8_t OLED_Graphic_MEM[OLED_PIXEL_X][OLED_PAGE_SIZE]; // OLED�Դ�

// OLED�����ú���
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_Graphic_MEM(void); // ����Դ�
void OLED_RefreshGraphic(void);

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode);

void OLED_ShowChar(u8 x, u8 y, u8 ch, u8 mode);
void OLED_ShowStr(uint8_t x, uint8_t y, char *str, uint8_t mode);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t mode);
void OLED_ShowChinese(uint8_t x, uint8_t y, char *ChineseStr, uint8_t mode);

void OLED_ShowBMP(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t BMP[]);

void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);

void fill_picture(unsigned char fill_Data);
void Picture(void);


#endif

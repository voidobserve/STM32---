/*************************************************
 Copyright ? 0010. All rights reserved.

// �ļ���
@ File name: oled.c
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
#include "oled.h"
#include <stdlib.h>
#include <string.h>
#include "oledfont.h"
#include "delay.h"

#ifndef OLED_PIXEL_X
#define OLED_PIXEL_X 128 // OLED����ֱ���
#endif
#ifndef OLED_PIXEL_Y
#define OLED_PIXEL_Y 64 // OLED����ֱ���
#endif

#ifndef OLED_PAGE_SIZE
#define OLED_PAGE_SIZE (OLED_PIXEL_Y / 8) // OLED��ҳ
#endif

#ifndef BRIGHTNESS
#define BRIGHTNESS 0xFF // Ҫ��䵽OLED�Դ��е����ݣ�ȫ�׻�ȫ�ڣ�
#endif
#ifndef OLED_Y_BASE
#define OLED_Y_BASE 0xB0 // OLEDҳ��ַ��׼
#endif
#ifndef OLED_X_BASE_L
#define OLED_X_BASE_L 0x00 // OLEDx�����صĻ�׼����λ��
#endif
#ifndef OLED_X_BASE_H
#define OLED_X_BASE_H 0x10 // OLEDx�����صĻ�׼����λ��
#endif

// Ҫ��OLED����ʾ���ַ��Ĵ�С����λ�����أ�
#ifndef OLED_FONT_WIDTH
#define OLED_FONT_WIDTH 8
#endif
#ifndef OLED_FONT_HEIGHT
#define OLED_FONT_HEIGHT 16
#endif

#define OLED_CH_FONT_HEIGHT 16
#define OLED_CH_FONE_WIDTH 16

uint8_t OLED_Graphic_MEM[OLED_PIXEL_X][OLED_PAGE_SIZE]; // OLED�Դ�

// OLED���Դ�
// ��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
/**********************************************
//IIC Start
**********************************************/

// SDA����Ϊ���ģʽ
static void IIC_SDASetOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	OLED_SDA_RCC_CMD(OLED_SDA_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // ����ٶ�
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // ���츴��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // ��������

	GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN; // ���ű��
	GPIO_Init(OLED_SDA_PORT, &GPIO_InitStructure);
}

// SDA����Ϊ����ģʽ
static void IIC_SDASetInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	OLED_SDA_RCC_CMD(OLED_SDA_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // ���ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // ��������

	GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN; // ���ű��
	GPIO_Init(OLED_SDA_PORT, &GPIO_InitStructure);
}

static void IIC_Start(void)
{
	// 1.SDA��������Ϊ���ģʽ
	IIC_SDASetOutputMode();

	// 2.ȷ��SDA��SCLΪ����״̬--�ߵ�ƽ
	IIC_SDA(1); // ������������
	IIC_SCL(1); // ��ʱ��������
	delay_us(4);

	// 3.��SCL�ߵ�ƽ�ڼ䣬��SDA��ƽ����
	IIC_SDA(0);
	delay_us(4);

	// 4.��SCLʱ���ߵĵ�ƽ����
	IIC_SCL(0);
	delay_us(4);
}

/**********************************************
//IIC Stop
**********************************************/
static void IIC_Stop(void)
{
	// 1.SDA��������Ϊ���ģʽ
	IIC_SDASetOutputMode();

	// 2.ȷ��SDA��SCL������͵�ƽ
	IIC_SCL(0);
	IIC_SDA(0);
	delay_us(4);

	// 3.��SCLʱ���ߵĵ�ƽ����
	IIC_SCL(1);
	delay_us(4);

	// 4.��SDA�����ߵĵ�ƽ����
	IIC_SDA(1);
	delay_us(4);
}

static bool IIC_Wait_Ack(void)
{
	// ��������洢Ӧ��״̬
	bool status = false;

	// 1.����SDA������Ϊ����ģʽ
	IIC_SDASetInputMode();

	// 2.��SCLʱ���ߵĵ�ƽ���ͣ���ʱ�ӻ�����׼������
	// ��һ������ʡ�ԣ���Ϊ�ú�����ǰһ����IIC����һ���ֽڵĺ�����
	// ���Ǹ�����������Ѿ���ʱ���������ˣ�����������ʱ
	// IIC_SCL(0);
	// delay_us(4);

	// 3.��SCLʱ���ߵĵ�ƽ���ߣ���ʱ�������Զ�ȡӦ������
	IIC_SCL(1);
	delay_us(4);

	// 4.��������ȡSDA�����ߵĵ�ƽ״̬
	// 0--��ʾӦ��ACK��1--��ʾ��Ӧ��NACK
	if (IIC_READ)
		status = false;
	else
		status = true;

	IIC_SCL(0);
	delay_us(4);
	return status;
}
/**********************************************
// IIC Write byte
**********************************************/

static void Write_IIC_Byte(unsigned char data)
{
	uint8_t i = 0;

	// 1.SDA��������Ϊ���ģʽ
	IIC_SDASetOutputMode();

	// 2.��SCLʱ���ߵ�ƽ����
	IIC_SCL(0);
	delay_us(4);

	// 3.ѭ������8bit����
	for (i = 0; i < 8; i++)
	{
		// 4.�������������ݵ����λ MSB
		if (data & 0x80)
			IIC_SDA(1);
		else
			IIC_SDA(0);

		data <<= 1; // ��������1bit
		delay_us(4);

		// 5.��SCLʱ���ߵĵ�ƽ���ߣ���ʱ�ӻ����Զ�ȡ
		IIC_SCL(1);
		delay_us(4);

		// 6.��SCLʱ���ߵĵ�ƽ���ͣ���ʱ��������׼������
		// �ڷ��͵�8��bit�󣬰�ʱ�������ͣ�������8bit��ʱ������
		IIC_SCL(0);
		delay_us(4);
	}
}
/**********************************************
// IIC Write Command
**********************************************/
static void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // Slave address,SA0=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00); // write command
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
static void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40); // write data
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

static void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
	if (cmd)
	{

		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		OLED_WR_Byte(0xb0 + m, 0); // page0-page1
		OLED_WR_Byte(0x00, 0);	   // low column start address
		OLED_WR_Byte(0x10, 0);	   // high column start address
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}

/**
 * @brief OLED��λ����
 *
 * @param x OLED������ 0 ~ OLED_PIXEL_X
 * @param y OLEDҳ     0 ~ OLED_PAGE_SIZE
 */
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	x += OLED_X_BASE_L; // Ҫ����OLEDx�����صĻ�׼����λ�����н�λ��˳���λ
	// д��ҳ��ַ
	OLED_WR_Byte(OLED_Y_BASE + y, OLED_CMD);
	// д��8λ��ʼ�е�ַ�ĵ�4λ
	OLED_WR_Byte(x & 0x0f, OLED_CMD);
	// д��8λ��ʼ�е�ַ�ĸ�4λ��Ҫ����OLEDx�����صĻ�׼����λ����
	OLED_WR_Byte((x & 0xf0) >> 4 | OLED_X_BASE_H, OLED_CMD);
}

/**
 * @brief OLED���㺯��
 *
 * @param x ������ 0 ~ OLED_PIXEL_X
 * @param y ������ 0 ~ OLED_PIXEL_Y
 * @param mode �Ƿ񷴰���ʾ
 */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode)
{
	uint8_t pos; // ���Ҫ����������Ӧ��OLEDҳ��λ��

	if (x >= OLED_PIXEL_X || y >= OLED_PIXEL_Y)
	{
		// ���Ҫ��������곬������Ļ
		return;
	}

	pos = y / 8;

	if (mode)
	{
		// ������Ƿ�����ʾ
		OLED_Graphic_MEM[x][pos] |= 1 << (y % 8);
	}
	else
	{
		OLED_Graphic_MEM[x][pos] &= (~(1 << (y % 8)));
	}
}

/**
 * @brief OLEDˢ�£����Դ��е�����д�뵽��Ļ�У�
 */
void OLED_RefreshGraphic(void)
{
	uint8_t i, j;

	for (i = 0; i < OLED_PAGE_SIZE; i++)
	{
		// ��λ
		OLED_Set_Pos(0, i);
		for (j = 0; j < OLED_PIXEL_X; j++)
		{
			// ��������
			OLED_WR_Byte(OLED_Graphic_MEM[j][i], OLED_DATA);
		}
	}
}

/**
 * @brief  ������ʾ.
 * @param  None
 * @retval None
 */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC����
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
/**
 * @brief  �ر���ʾ.
 * @param  None
 * @retval None
 */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC����
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

// ��������,������,������Ļ��ȫ�׵�
void OLED_Clear(void)
{
	uint8_t i, j;

	for (i = 0; i < OLED_PAGE_SIZE; i++)
	{
		for (j = 0; j < OLED_PIXEL_X; j++)
		{
			OLED_Graphic_MEM[j][i] = 0x00; // ������ʲô������ʾ
		}
	}

	// ˢ��OLED
	OLED_RefreshGraphic();
}

// ����Դ�
void OLED_Clear_Graphic_MEM(void)
{
	memset(OLED_Graphic_MEM, 0, sizeof(OLED_Graphic_MEM) / sizeof(OLED_Graphic_MEM[0][0]));
}

/**
 * @brief OLED��ʾ�ַ�
 *
 * @param x x����ʼλ�ã�0~127
 * @param y y����ʼλ�ã�0~63
 * @param ch Ҫ��ʾ���ַ�
 * @param mode �Ƿ�Ҫ������ʾ
 *             1--��������ʾ
 *             0--������ʾ
 */
void OLED_ShowChar(u8 x, u8 y, u8 ch, u8 mode)
{
	uint8_t i;				   // ѭ������ֵ
	uint8_t j;				   // ѭ������ֵ
	uint8_t y0 = y;			   // y0������¼��ǰҪ����y���꣬���ƻ�����У�ʹÿһ�е�y����ʼλ�ö�����
	uint8_t locate = ch - ' '; // ����ַ����ֿ��е�λ��

	// ����Ĵ�С����ռ���ٸ��ֽڣ�
	// ͨ����Ŀ������ж������ǿ� ������ҳ ���� ż����ҳ
	uint8_t size = OLED_FONT_WIDTH * ((OLED_FONT_HEIGHT % 8) ? (OLED_FONT_HEIGHT / 8 + 1) : (OLED_FONT_HEIGHT / 8));

	uint8_t temp; // ��ʱ���������Ҫд����ַ�

	for (i = 0; i < size; i++)
	{
		temp = ascii_0816[locate][i];

		for (j = 0; j < 8; j++)
		{
			// ͨ��ѭ����ÿ�λ� ��ǰtemp���λ�ĵ�

			if (temp & 0x01)
			{
				// ���temp��ǰ���λ��1������
				OLED_DrawPoint(x, y, mode);
			}
			else
			{
				// ���temp��ǰ���λ��0�������㣬���Դ�д��0
				OLED_DrawPoint(x, y, !mode);
			}

			temp >>= 1; // temp�Ĵε�λ��Ϊ���λ��׼������
			y++;		// �У�Ҳ���������꣬��һ

			if (y - y0 >= OLED_FONT_HEIGHT)
			{
				// ���������һ�У���Ҫ����һ����
				y = y0;
				x++; // x���һ
				break;
			}
		}
	}
}

/**
 * @brief OLED��ʾ�ַ������������x��ỻ����ʾ��
 *
 * @param x x����ʼλ��
 * @param y y����ʼλ��
 * @param str Ҫ��ʾ���ַ���
 * @param mode �Ƿ�Ҫ������ʾ
 *             1����������ʾ
 *             0��������ʾ
 */
void OLED_ShowStr(uint8_t x, uint8_t y, char *str, uint8_t mode)
{
	// �ж��ַ��Ƿ����ASCII�ķ�Χ
	while ((*str) >= ' ' && (*str) <= '~')
	{
		if (x > OLED_PIXEL_X - OLED_FONT_WIDTH)
		{
			// ���Ҫ��ʾ���ַ���������Ļ�ĺ���Χ��������ʾ
			x = 0;
			y += OLED_FONT_HEIGHT;
		}

		if (y > OLED_PIXEL_Y - OLED_FONT_HEIGHT)
		{
			// ���Ҫ��ʾ���ַ���������Ļ������Χ
			// �������������ӣ�0��0�����������ʾ
			OLED_Clear();
			x = y = 0;
		}

		OLED_ShowChar(x, y, *str++, mode);
		x += OLED_FONT_WIDTH;
	}
}

/**
 * @brief ��һ�����Ķ�η���n��m�η���
 *
 * @param n ����
 * @param m ��
 * @retval  �������������m�η�
 */
static uint64_t oled_pow(uint32_t n, uint32_t m)
{
	uint64_t ret = 1;
	while (m--)
	{
		ret *= n;
	}

	return ret;
}

/**
 * @brief OLED��ʾ���֣������Ǹ�����
 *
 * @param x x����ʼλ��
 * @param y y����ʼλ��
 * @param num Ҫ��ʾ������
 * @param len ���ֵĳ���
 * @param mode �Ƿ�Ҫ������ʾ
 *             1����������ʾ
 *             0��������ʾ
 */
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t mode)
{
	uint8_t i;			// ѭ������ֵ
	uint8_t temp;		// ��ʱ����
	uint8_t enshow = 0; // ��Ч���ֱ�־λ��1������Ч���֣�0��������Ч����

	// ͨ��ѭ����ÿһ����λ��ʾ����
	for (i = 0; i < len; i++)
	{
		// ͨ������õ���ǰ���λ����ֵ
		temp = num / oled_pow(10, len - 1 - i) % 10;

		if (temp == 0)
		{
			if (enshow == 0)
			{
				// ������λ��0�����Ҳ������������Ч����
				OLED_ShowChar(x, y, ' ', mode);
			}
			else
			{
				// �����ǰλ��0�����������������Ч����
				OLED_ShowChar(x, y, '0', mode);
			}
		}
		else
		{
			// ��������������λ
			enshow = 1;
			OLED_ShowChar(x, y, '0' + temp, mode);
		}

		x += OLED_FONT_WIDTH;
		if (x > OLED_PIXEL_X - OLED_FONT_WIDTH)
		{
			// ���Ҫ��ʾ���ַ���������Ļ�ĺ���Χ��������ʾ
			x = 0;
			y += OLED_FONT_HEIGHT;
		}
		if (y > OLED_PIXEL_Y - OLED_FONT_HEIGHT)
		{
			// ���Ҫ��ʾ���ַ���������Ļ������Χ
			// �������������ӣ�0��0�����������ʾ
			OLED_Clear();
			x = y = 0;
		}
	}
}

/**
 * @brief OLED��ʾ����
 *
 * @param x x����ʼλ��
 * @param y y����ʼλ��
 * @param pHZ Ҫ��ʾ�ĺ���
 * @param mode �Ƿ�Ҫ������ʾ
 *             1����������ʾ
 *             0��������ʾ
 */
void OLED_ShowChinese(uint8_t x, uint8_t y, char *ChineseStr, uint8_t mode)
{
	uint8_t i; // ѭ������ֵ
	uint8_t j; // ѭ������ֵ
	uint8_t k;
	uint8_t temp; // ��ʱ����
	uint8_t y0 = y;

	// ����������ֿ�������Ԫ�صĸ���
	uint8_t len = sizeof(Chinese) / sizeof(Chinese[0]);

	// ����Ĵ�С����ռ���ٸ��ֽڣ�
	// ͨ����Ŀ������ж������ǿ� ������ҳ ���� ż����ҳ
	uint8_t size = OLED_CH_FONE_WIDTH * ((OLED_CH_FONT_HEIGHT % 8) ? (OLED_CH_FONT_HEIGHT / 8 + 1) : (OLED_CH_FONT_HEIGHT / 8));

	while (*ChineseStr != '\0')
	{
		// ��ǰҪ��ʾ�ĺ��ֲ�Ϊ'\0'ʱ������ѭ��

		for (i = 0; i < len; i++)
		{
			if (ChineseStr[0] == Chinese[i].Index[0] && ChineseStr[1] == Chinese[i].Index[1])
			{
				// ������ֿ����ҵ��˸ú���

				// �жϵ�ǰ������û�г�����Ļ��ʾ
				if (x > OLED_PIXEL_X - OLED_CH_FONE_WIDTH)
				{
					// ���Ҫ��ʾ���ַ���������Ļ�ĺ���Χ��������ʾ
					x = 0;
					y += OLED_CH_FONT_HEIGHT;
					y0 = y;
				}
				if (y > OLED_PIXEL_Y - OLED_CH_FONT_HEIGHT)
				{
					// ���Ҫ��ʾ���ַ���������Ļ������Χ
					// �������������ӣ�0��0�����������ʾ
					OLED_Clear();
					x = y = 0;
				}

				for (j = 0; j < size; j++)
				{
					temp = Chinese[i].Msk[j];

					for (k = 0; k < 8; k++)
					{
						if (temp & 0x01)
						{
							// ���temp��ǰ���λ��1������
							OLED_DrawPoint(x, y, mode);
						}
						else
						{
							// ���temp��ǰ���λ��0�������㣬���Դ�д��0
							OLED_DrawPoint(x, y, !mode);
						}

						temp >>= 1; // temp�Ĵε�λ��Ϊ���λ��׼������
						y++;		// �У�Ҳ���������꣬��һ

						if (y - y0 >= OLED_CH_FONT_HEIGHT)
						{
							// ���������һ�У���Ҫ����һ����
							y = y0;
							x++; // x���һ
							break;
						}
					}
				}

				break; // ������ֿ����Ҳ�������
			}
		}

		ChineseStr += 2;
	}
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

// ͼƬҪ�� ���� + ����ʽ + ���� + ʮ������ + C51��ʽȡģ����
void OLED_ShowBMP(const uint8_t x, const uint8_t y, uint8_t width, uint8_t height, const uint8_t BMP[])
{
	uint8_t i;			// ѭ������ֵ
	uint8_t j;			// ѭ������ֵ
	uint8_t k;			// ѭ������ֵ
	uint8_t cur_x = x;	// cur_x������¼��ǰҪ����x���꣬���ƻ�����У�ʹÿһ�е�x����ʼλ�ö�����
	uint8_t cur_y = y;	// cur_y������¼��ǰҪ����y���꣬���ƻ�����У�ʹÿһ�е�y����ʼλ�ö�����
	uint8_t locate = 0; // ��¼��ǰ��ȡ��ͼƬ�ĵڼ����ֽ�
	uint8_t temp;		// ���һ�ζ�ȡͼƬ��һ���ֽ����ݵı���

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			temp = BMP[locate++]; // һ�ζ�ȡͼƬ��һ���ֽ�����
			for (k = 0; k < 8; k++)
			{
				// ��ѭ���ڣ����Դ�д��һ���ֽڵ�����
				// ͨ��ѭ����ÿ�λ� ��ǰtemp���λ�ĵ�
				if (temp & 0x01)
				{
					// ���temp��ǰ���λ��1������
					OLED_DrawPoint(cur_x, cur_y, 1);
				}
				else
				{
					// ���temp��ǰ���λ��0�������㣬���Դ�д��0
					OLED_DrawPoint(cur_x, cur_y, 0);
				}

				temp >>= 1; // temp�Ĵε�λ��Ϊ���λ��׼������
				cur_y++;	// �У�Ҳ���������꣬��һ
			}

			if (cur_y - y >= height)
			{
				// ���������һ�У���Ҫ����һ����
				cur_y = y;
				cur_x++; // x���һ
				break;
			}
		}

		if (cur_x - x >= width)
		{
			// �����ǰ��x�����곬����ͼƬ��ȣ�˵��ͼƬ�������Ѿ�ȫ��д�뵽�Դ���
			break;
		}
	}
}

// ��ʼ��SSD1306
void OLED_Init(void)
{
	// ���ú�OLED������

	// ��������
	GPIO_InitTypeDef GPIO_InitStructure;

	OLED_SCL_RCC_CMD(OLED_SCL_RCC, ENABLE);
	OLED_SDA_RCC_CMD(OLED_SDA_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // ����ٶ�
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // ���츴��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // ��������

	GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN; // ���ű��
	GPIO_Init(OLED_SCL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN; // ���ű��
	GPIO_Init(OLED_SDA_PORT, &GPIO_InitStructure);

	delay_ms(200);

	// SSD1306��ʼ������

	OLED_WR_Byte(0xAE, OLED_CMD); //--display off 						 �ر���ʾ(0xAE/0xAF �ر�/����)
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address 			 Ĭ��Ϊ 0
	OLED_WR_Byte(0X02, OLED_CMD); //									 [1:0]00:�е�ַģʽ;01:�е�ַ;10:ҳ��ַģʽ;
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0X20, OLED_CMD); // �����ڴ��ַģʽ
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address			 ������ʾ��ʼ��[5:0]
	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control					 ���öԱȶ�
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap					 �������ҷ��� (0xa0/0xa1 ��/��)
	OLED_WR_Byte(0XA4, OLED_CMD); // ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse					 ����������ʾ(0xa6/0xa7  ����/������)
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64) 		 ��������·��(1~64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty 						 Ĭ�� 0X3F	(1~64)
	OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction					 ����ɨ�跽�� (0xc0/0xc8 ���·�ת/����)
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset					 ������ʾƫ��
	OLED_WR_Byte(0x00, OLED_CMD); //

	OLED_WR_Byte(0xD5, OLED_CMD); // set osc division 					 ����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(0x80, OLED_CMD); // [3:0],								 ��Ƶ����;[7:4],��Ƶ��

	OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //

	OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period				 ����Ԥ�������
	OLED_WR_Byte(0xF1, OLED_CMD); //									 [3:0],PHASE 1;[7:4],PHASE 2;

	OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion			 ����Ӳ����������
	OLED_WR_Byte(0x12, OLED_CMD); //									 [5:4]����

	OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh							 ����VCOMH��ѹ����
	OLED_WR_Byte(0x30, OLED_CMD); // [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable				 ��ɱ�����
	OLED_WR_Byte(0x14, OLED_CMD); // 									 ����/�ر� 0x14/0x10

	OLED_WR_Byte(0XEF, OLED_CMD); // ���ȵ���0x00~0xFF (��������,Խ��Խ��)

	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel				 ������ʾ

	OLED_Clear(); // ����

	// ��λ������Ϊ 0��0 �ĵط��������´�д��
	OLED_Set_Pos(0, 0);
}

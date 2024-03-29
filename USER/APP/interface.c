#include "interface.h"
#include <stdio.h>
#include <string.h>

#include "oled.h"
#include "key.h"
#include "bmp.h"   // ���ͼƬ���ݵ��ļ�
#include "dht11.h" // ��ʪ�ȴ�����
#include "delay.h"
#include "ms1100.h"      // ʹ�õ���ȩŨ�ȵ�ȫ�ֱ���
#include "dust_sensor.h" // ʹ�õ�PM2.5Ũ�ȵ�ȫ�ֱ���
#include "fan.h"         // ʹ�õ�����ת�ٵ�ȫ�ֱ��������ڷ���ת�ٵĽӿ�

cur_index_t index = CUR_MENU; // ���������±꣬��¼OLED��ǰ��ʾ�Ľ���

// �ڲ˵�ѡ��Ĺ���
// 1--ѡ���˲�ѯ��ʪ��
// 2--ѡ���˲�ѯ��ȩŨ��
// 3--ѡ���˲�ѯPM2.5Ũ��
// 4--ѡ���˵��ڷ���ת��
int cursel = 0;
// �����˵��У�ȷ����ť�Ƿ��µı�־λ��0--δ���£�1--����
int enter_flag = 0;

static void Show_Menu(void)
{
    OLED_ShowNum(0, 0, 1, 1, 1);
    OLED_ShowChar(8, 0, '-', 1);
    OLED_ShowChinese(16, 0, "��ѯ��ʪ��", 1);

    OLED_ShowNum(0, 16, 2, 1, 1);
    OLED_ShowChar(8, 16, '-', 1);
    OLED_ShowChinese(16, 16, "��ѯ��ȩŨ��", 1);

    OLED_ShowNum(0, 32, 3, 1, 1);
    OLED_ShowChar(8, 32, '-', 1);
    OLED_ShowChinese(16, 32, "��ѯ", 1);
    OLED_ShowStr(48, 32, "PM", 1);
    OLED_ShowNum(64, 32, 2, 1, 1);
    OLED_ShowChar(72, 32, '.', 1);
    OLED_ShowNum(80, 32, 5, 1, 1);
    OLED_ShowChinese(88, 32, "Ũ��", 1);

    OLED_ShowNum(0, 48, 4, 1, 1);
    OLED_ShowChar(8, 48, '-', 1);
    OLED_ShowChinese(16, 48, "���ڷ���ת��", 1);

    // OLED_RefreshGraphic();
}

// ��ʾ��ǰѡ��Ĺ��ܣ�ֱ�ӽ���Ӧ�����ַ�����ʾ
// 1--ѡ���˲�ѯ��ʪ��
// 2--ѡ���˲�ѯ��ȩŨ��
// 3--ѡ���˲�ѯPM2.5Ũ��
// 4--ѡ���˵��ڷ���ת��
// ����--û��ѡ���Ӧ�Ĺ��ܣ���ʾĬ�ϵ�Ŀ¼
// void Show_Cursel_Fun(uint8_t lastsel, uint8_t cursel)
void Show_Cursel_Menu(uint8_t cursel)
{
    Show_Menu(); // �����Դ�����Ĭ�ϵ�Ŀ¼������

    // �ڸ���ѡ�񣬽���Ӧ���ܵ����ַ�����ʾ
    switch (cursel)
    {
    case 1:
        OLED_ShowNum(0, 0, 1, 1, 0);
        OLED_ShowChar(8, 0, '-', 0);
        OLED_ShowChinese(16, 0, "��ѯ��ʪ��", 0);
        break;
    case 2:
        OLED_ShowNum(0, 16, 2, 1, 0);
        OLED_ShowChar(8, 16, '-', 0);
        OLED_ShowChinese(16, 16, "��ѯ��ȩŨ��", 0);
        break;
    case 3:
        OLED_ShowNum(0, 32, 3, 1, 0);
        OLED_ShowChar(8, 32, '-', 0);
        OLED_ShowChinese(16, 32, "��ѯ", 0);
        OLED_ShowStr(48, 32, "PM", 0);
        OLED_ShowNum(64, 32, 2, 1, 0);
        OLED_ShowChar(72, 32, '.', 0);
        OLED_ShowNum(80, 32, 5, 1, 0);
        OLED_ShowChinese(88, 32, "Ũ��", 0);
        break;
    case 4:
        OLED_ShowNum(0, 48, 4, 1, 0);
        OLED_ShowChar(8, 48, '-', 0);
        OLED_ShowChinese(16, 48, "���ڷ���ת��", 0);
        break;
    default: // û��ѡ����
        break;
    }

    // �޸���ɺ�ˢ���Դ�
    OLED_RefreshGraphic();
}

// OLED��ʾ������Ĵ�����
void Interface_Menu(void)
{
    // ���ݼ�ֵ���ж�Ҫ������ʾ��ѡ��Ĺ��ܲ˵�
    if (key)
    {
        switch (key)
        {
        case KEY0_PRES:
            cursel = 0;
            break;
        case KEY1_PRES:
            cursel--;
            // ��cursel�������ƣ���ֹ����������������ѡ���ĸ�����
            if (cursel <= 0)
            {
                cursel = 4;
            }
            break;
        case KEY2_PRES:
            cursel++;
            // ��cursel�������ƣ���ֹ����������������ѡ���ĸ�����
            if (cursel >= 5)
            {
                cursel = 1;
            }
            break;
        case KEY3_PRES:
            // cursel = 0;
            enter_flag = 1; // ȷ�ϰ�ť������
            break;
        default:
            break;
        }
    }

    key = 0; // �����ֵ
    Show_Cursel_Menu(cursel);

    if (enter_flag)
    {
        // ���ݵ�ǰ������ʾ�Ĺ��ܣ��޸Ľ�������
        switch (cursel)
        {
        case 1:
            index = CUR_FUNCTION_SHOW_HUMITURE; // ��ǰ��ʾ�Ľ�������ʾ��ʪ��
            // Show_Humiture();
            break;
        case 2:
            index = CUR_FUNCTION_SHOW_HCHO;
            break;
        case 3:
            index = CUR_FUNCTION_SHOW_PM2_5;
            break;
        case 4:
            index = CUR_FUNCTION_SHOW_FAN_SPEED;
            break;
        default:
            break;
        }

        cursel = 0;     // �����־λ
        enter_flag = 0; // ���۴����֣�������ñ�־λ
    }
}

// ��0.96'OLED����ʾDHT11�ɼ�������ʪ��
void Show_Humiture(void)
{
    OLED_Clear_Graphic_MEM();

    OLED_ShowBMP(0, 0, 32, 32, temperature_icon);
    OLED_ShowChinese(32, 8, "�¶ȣ�", 1);

    OLED_ShowBMP(0, 32, 32, 32, humi_icon);
    OLED_ShowChinese(32, 36, "ʪ�ȣ�", 1);

    // =================================
    // ��GB2312�����£�����������ڴ��ڵ���������ȥ����������
    // ������
    // sprintf((char *)buf, "�¶� = %d ��,ʪ��=%d %%RH \n\n", dhtbuf[2], dhtbuf[0]);
    // printf("%s\r\n", buf);
    // =================================

    // ֱ����ʾ�ϴβɼ��������ݣ����������ȫ�ֱ���
    OLED_ShowNum(80, 8, dhtbuf[2], 2, 1);
    OLED_ShowChinese(104, 8, "��", 1);

    OLED_ShowNum(80, 36, dhtbuf[0], 2, 1);
    OLED_ShowStr(104, 36, "%RH", 1);

    OLED_RefreshGraphic();

    if (KEY0_PRES == key)
    {
        // ��������˷��ذ���
        // �޸Ľ���������±꣬�´ν���ʾ���˵�
        index = CUR_MENU;
        OLED_Clear_Graphic_MEM(); // ����Դ�
    }
}

// ��0.96'OLED����ʾMS1100�ɼ����ļ�ȩŨ��
void Show_HCHO(void)
{
    uint8_t buf[30] = {0};

    OLED_Clear_Graphic_MEM();

    OLED_ShowChinese(24, 0, "��ȩŨ�ȣ�", 0);

    sprintf((char *)buf, "%f", hcho_mg_per_cubic_meter);
    strcat((char *)buf, "mg/m^3");

    OLED_ShowStr(8, 24, (char *)buf, 1);

    OLED_RefreshGraphic();

    if (KEY0_PRES == key)
    {
        // ��������˷��ذ���
        // �޸Ľ���������±꣬�´ν���ʾ���˵�
        index = CUR_MENU;
        OLED_Clear_Graphic_MEM(); // ����Դ�
    }
}

// ��0.96'OLED����ʾ�۳��������ɼ�����PM2.5Ũ��
void Show_PM2_5(void)
{
    uint8_t buf[30] = {0};

    OLED_Clear_Graphic_MEM();

    OLED_ShowStr(16, 0, "PM", 0);
    OLED_ShowNum(32, 0, 2, 1, 0);
    OLED_ShowChar(40, 0, '.', 0);
    OLED_ShowNum(48, 0, 5, 1, 0);
    OLED_ShowChinese(56, 0, "Ũ�ȣ�", 0);

    sprintf((char *)buf, "%f", pm2_5_mg_per_cubic_meter);
    strcat((char *)buf, "mg/m^3");

    OLED_ShowStr(8, 24, (char *)buf, 1);

    OLED_RefreshGraphic();

    if (KEY0_PRES == key)
    {
        // ��������˷��ذ���
        // �޸Ľ���������±꣬�´ν���ʾ���˵�
        index = CUR_MENU;
        OLED_Clear_Graphic_MEM(); // ����Դ�
    }
}

void Show_FAN_Speed(void)
{
    OLED_Clear_Graphic_MEM();

    OLED_ShowBMP(0, 30, 30, 30, fan_bmp);
    OLED_ShowChinese(32, 0, "����ת��", 0);

    if (KEY1_PRES == key)
    {
        FAN_Speed_dec();
        key = 0;
    }
    else if (KEY2_PRES == key)
    {
        FAN_Speed_Inc();
        key = 0;
    }
    
    switch (cur_fan_speed)
    {
    case CUR_FAN_SPEED_CLOSE:
        OLED_ShowChinese(40, 32, "�ر�", 1);
        break;
    case CUR_FAN_SPEED_MIN:
        OLED_ShowChinese(40, 32, "�����", 1);
        break;

    case CUR_FAN_SPEED_LOW:
        OLED_ShowChinese(40, 32, "����", 1);
        break;

    case CUR_FAN_SPEED_MID:
        OLED_ShowChinese(40, 32, "����", 1);
        break;

    case CUR_FAN_SPEED_MAX:
        OLED_ShowChinese(40, 32, "�����", 1);
        break;

    default:
        break;
    }

    OLED_RefreshGraphic();

    if (KEY0_PRES == key)
    {
        // ��������˷��ذ���
        // �޸Ľ���������±꣬�´ν���ʾ���˵�
        index = CUR_MENU;
        OLED_Clear_Graphic_MEM(); // ����Դ�
    }
}

// ����Ŀ¼�����±����л�Ҫ��ʾ�Ľ���
void OLED_Show_Interface(void)
{
    if (CUR_MENU == index)
    {
        Interface_Menu();
    }
    else if (CUR_FUNCTION_SHOW_HUMITURE == index)
    {
        Show_Humiture();
    }
    else if (CUR_FUNCTION_SHOW_HCHO == index)
    {
        Show_HCHO();
    }
    else if (CUR_FUNCTION_SHOW_PM2_5 == index)
    {
        Show_PM2_5();
    }
    else
    {
        Show_FAN_Speed();
    }
}

/**
 ******************************************************************************
 * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
 * @author  MCD Application Team
 * @version V1.4.0
 * @date    04-August-2014
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include <stdio.h>
#include <stdbool.h>

#include "led.h"
#include "rtc.h"
#include "key.h"
#include "tim.h"
#include "interface.h"
#include "child_safety_lock.h"

/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
//void SVC_Handler(void)
//{
//}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
//void PendSV_Handler(void)
//{
//}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
//void SysTick_Handler(void)
//{
//    //  TimingDelay_Decrement();
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

// ÿ��1s����һ��
void RTC_WKUP_IRQHandler(void)
{
    // �ж�RTC�����ж��Ƿ���
    if (RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        uwTimeDisplay = 1;
        RTC_ClearITPendingBit(RTC_IT_WUT);   // ��������жϱ�־
        EXTI_ClearITPendingBit(EXTI_Line22); // ���EXTI22�ı�־
    }
}

void RTC_Alarm_IRQHandler(void)
{

    if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
        LED1 = 0;

        /* Clear RTC AlarmA Flags */
        RTC_ClearITPendingBit(RTC_IT_ALRA);
    }
    /* Clear the EXTI line 17 */
    EXTI_ClearITPendingBit(EXTI_Line17);
}

void EXTI0_IRQHandler(void)
{
    // ��ȡ�ⲿ�ж��ߵ��ж�״̬
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // ����ͯ��
        if (false == lock)
        {
            key = KEY0_PRES; // ����0������
        }

        // ����ж��ߵ�״̬��Ŀ���Ƿ����´ο��Լ�������
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI2_IRQHandler(void)
{
    // ��ȡ�ⲿ�ж��ߵ��ж�״̬
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        // ����ͯ��
        if (false == lock)
        {
            key = KEY1_PRES; // ����1������
        }
        // ����ж��ߵ�״̬��Ŀ���Ƿ����´ο��Լ�������
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler(void)
{
    // ��ȡ�ⲿ�ж��ߵ��ж�״̬
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        // ����ͯ��
        if (false == lock)
        {
            key = KEY2_PRES; // ����2������
        }
        // ����ж��ߵ�״̬��Ŀ���Ƿ����´ο��Լ�������
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTI4_IRQHandler(void)
{
    // ��ȡ�ⲿ�ж��ߵ��ж�״̬
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        // ����ͯ��
        if (false == lock)
        {
            key = KEY3_PRES; // ����3������
        }
        // ����ж��ߵ�״̬��Ŀ���Ƿ����´ο��Լ�������
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

void TIM6_DAC_IRQHandler(void)
{
    // ����Ƕ�ʱ��6���紥�����ж�
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        tim6_update_cnt++;

        // ����жϱ�־����ֹ����ͬ���жϴ��
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#include "usart2.h"
#include "stm32f4xx.h"

uint8_t USART2_RX_BUF[128]; // ���ջ���
uint8_t USART2_RX_CNT = 0;
uint8_t USART2_RX_FALG = 0; // ����״̬���

// baud:������
void USART2_Init(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// 1.��GPIO�˿�  PA2 PA3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// 2.�򿪴���ʱ��  USART2 -- APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// 3.ѡ�����ŵĸ��ù���
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	// 4.����GPIO���Ų�������ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // ����ٶ�
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // ���츴��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // ���ű��
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 5.����USART1�Ĳ�������ʼ��
	USART_InitStructure.USART_BaudRate = baud;										// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);

	// 6.�����жϲ�������ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  // �ж�ͨ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // ���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);

	// 7.ѡ���ж�Դ   ���յ������򴥷��ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	// 8.��USART2
	USART_Cmd(USART2, ENABLE);
}

// ���յ� '\r''\n'����һ�ν��գ�������ɱ�־λ��һ
void USART2_IRQHandler(void) // ����2�жϷ������
{
	u8 recv;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // �����ж�(���յ������ݱ�����0x0d 0x0a��β����\r\n��β)
	{
		recv = USART_ReceiveData(USART2); // ��ȡ���յ�������

		if (0 == USART2_RX_FALG) // ����δ���
		{
			USART2_RX_BUF[USART2_RX_CNT++] = recv;

			if (recv == '\n')
			{
				USART2_RX_CNT = 0;
				USART2_RX_FALG = 1; // ���һ�ν������
			}
		}
	}
}

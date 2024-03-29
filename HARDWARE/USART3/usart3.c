#include "usart3.h"
#include "stm32f4xx.h"

uint8_t Tx3Buffer[512];
volatile uint32_t Rx3Counter = 0;
volatile uint8_t Rx3Data = 0;
volatile uint8_t Rx3End = 0;
volatile uint8_t Rx3Buffer[512] = {0};


// ��ʼ��IO ����3
// bound:������
void USART3_Init(u32 bound)
{
    // GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // ʹ��GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // ʹ��USART3ʱ��

    // ����3��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); // GPIOB10����ΪUSART3
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); // GPIOB11����ΪUSART3

    // USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // GPIOB10��GPIOB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             // ���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       // �ٶ�
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           // ���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             // ����
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   // ��ʼ��PB10��PB11

    // USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;                                     // ����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             // ����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // �շ�ģʽ
    USART_Init(USART3, &USART_InitStructure);                                       // ��ʼ������3

    USART_Cmd(USART3, ENABLE); // ʹ�ܴ���3

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ��������ж�

    // Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         // ����3�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        // �����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                           // ����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}

void USART3_IRQHandler(void) // ����3�жϷ������
{

    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        /* Read one byte from the receive data register */
        Rx3Data = USART_ReceiveData(USART3);

        Rx3Buffer[Rx3Counter++] = Rx3Data;

        if (Rx3Counter >= sizeof(Rx3Buffer))
        {
            Rx3Counter = 0;
            Rx3End = 1;
        }
    }

    USART_ClearITPendingBit(USART3, USART_IT_RXNE); // ����жϱ�־
}

// ����һ���ֽڵĺ���
void USART3_SendByte(u8 Byte)
{
    USART_SendData(USART3, Byte);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ;
}

// �����ַ����ĺ���
void USART3_SendString(char *str)
{
    // ѭ�������ַ�
    while (*str != '\0')
    {
        USART_SendData(USART3, *str);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
            ;
        str++;
    }
}

/*****************  ����ָ�����ȵ��ֽ� **********************/
void USART_SendBytes(USART_TypeDef *pUSARTx, uint8_t *buf, uint32_t len)
{
    uint8_t *p = buf;

    while (len--)
    {
        USART_SendData(pUSARTx, *p);

        p++;

        // �ȴ����ݷ��ͳɹ�
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
            ;
        USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
    }
}

#include "bsp_hc05_usart.h"

extern ReceiveData BLT_USART_ReceiveData;

/// 配置USART接收中断
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = BLT_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void HC05_USART_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	BLT_USART_APBxClock_FUN(BLT_USART_CLK, ENABLE);
	BLT_USART_GPIO_APBxClock_FUN(BLT_USART_GPIO_CLK, ENABLE);

	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = BLT_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLT_USART_TX_PORT, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = BLT_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BLT_USART_RX_PORT, &GPIO_InitStructure);

	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = BLT_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(BLT_USARTx, &USART_InitStructure);

	/*	配置中断优先级 */
	NVIC_Configuration();
	/* 使能串口2接收中断 */
	USART_ITConfig(BLT_USARTx, USART_IT_RXNE, ENABLE);

	USART_Cmd(BLT_USARTx, ENABLE);
	USART_ClearFlag(BLT_USARTx, USART_FLAG_TC);
}

static void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(pUSARTx, ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

void Usart_SendStr_length(USART_TypeDef *pUSARTx, uint8_t *str, uint32_t strlen)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (k < strlen);
}

// 发送字符串
void Usart_SendString(USART_TypeDef *pUSARTx, uint8_t *str)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (*(str + k) != '\0');
}

// 中断缓存串口数据
volatile uint16_t uart_p = 0;
uint8_t uart_buff[UART_BUFF_SIZE];


// 获取接收到的数据和长度
char *get_rebuff(uint16_t *len)
{
	*len = BLT_USART_ReceiveData.datanum;
	return (char *)&BLT_USART_ReceiveData.uart_buff;
}

// 清空缓冲区
void clean_rebuff(void)
{

	uint16_t i = UART_BUFF_SIZE + 1;
	BLT_USART_ReceiveData.datanum = 0;
	BLT_USART_ReceiveData.receive_data_flag = 0;
	while (i)
		BLT_USART_ReceiveData.uart_buff[--i] = 0;
}

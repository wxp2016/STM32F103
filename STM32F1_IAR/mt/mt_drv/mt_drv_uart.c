#include "mt_drv_uart.h"
#include "mt_hal_nvic.h"



/******************************************************************************
* Name       : void MT_Drv_UART1_Init(void)
* Function   : USART1��ʼ��
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
* USART1   PA9--TXD   PA10--RXD
* USART2   PA2--TXD   PA3---RXD
* USART3   PB10-TXD   PB11--RXD
* UART4	   PC10-TXD   PC11--RXD 
* UART5	   PC12-TXD   PD2---RXD 
******************************************************************************/
void MT_Drv_USART1_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	MT_NVIC_Config(MT_PRIORITY_7);
	
	USART_InitStructure.USART_BaudRate = baud;	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		//ʹ�ܽ���
	USART_ITConfig(USART1,USART_IT_TXE,DISABLE);		//�رշ����ж�

	USART_Cmd(USART1, ENABLE);							//ʹ�ܴ���

	USART_ClearFlag(USART1, USART_FLAG_TC);     		//���������ɱ�־,��ֹ��һ���ֽ��޷�����
}



/******************************************************************************
* Name       : void USART2_Init(void)
* Function   : USART2��ʼ��
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
* USART1   PA9--TXD   PA10--RXD
* USART2   PA2--TXD   PA3---RXD
* USART3   PB10-TXD   PB11--RXD
* UART4	   PC10-TXD   PC11--RXD 
* UART5	   PC12-TXD   PD2---RXD 
******************************************************************************/
void USART2_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//PA2--TXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PA3--RXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	//PA0--DIR  �շ�ʱ�ܶ� 0-���� 1-����
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//MODBUS_485_RECE;//PA0=0 Ĭ�Ͻ���

	MT_NVIC_Config(MT_PRIORITY_4);

	USART_InitStructure.USART_BaudRate = baud;	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		
	
    //����USART2ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;                        //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;                              //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;                            //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; 
    
    USART_Init(USART2, &USART_InitStructure);
    USART_ClockInit(USART2, &USART_ClockInitStruct);
    
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART2, ENABLE);		  
}




void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//���USART1�жϴ�����λRXNE
	}
	//���-������������Ҫ�����SR�����λ,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);		//�����λ
		USART_ReceiveData(USART1);					//��DR
	} 
}







void USART2_IRQHandler(void)
{
  
}











void USART3_IRQHandler(void)
{
	
}

void UART4_IRQHandler(void)
{
	
}

void UART5_IRQHandler(void)
{
	
}


/******************************************************************************
* Name       : int fputc(int ch, FILE *f)
* Function   : �ض���fputc printf
* Input      : int ch, FILE *f
* Output:    : None
* Return     : int
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: None
******************************************************************************/
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}



/******************************************************************************
* Name       : int fgetc(FILE * f)
* Function   : �ض���getchar  scanf
* Input      : int ch, FILE *f
* Output:    : None
* Return     : int
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: None
******************************************************************************/
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  ch = USART_ReceiveData(USART1);
  return ch;
}



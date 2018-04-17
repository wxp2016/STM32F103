#include "mt_drv_iic.h"



//IIC 初始化
void MT_Drv_IIC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SDA_PORT_RCC,ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);					
	GPIO_SetBits(SDA_PORT,SDA_PIN);			

	RCC_APB2PeriphClockCmd(SCL_PORT_RCC,ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = SCL_PIN;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(SCL_PORT, &GPIO_InitStructure);					
	GPIO_SetBits(SCL_PORT,SCL_PIN);	
}

//SDA输出
void IIC_SDA_OUT(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SDA_PORT_RCC,ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);					
	GPIO_SetBits(SDA_PORT,SDA_PIN);	
}

//SDA输入
void IIC_SDA_IN(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SDA_PORT_RCC,ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);					
	GPIO_SetBits(SDA_PORT,SDA_PIN);
}


//IIC起始信号
void IIC_Start(void)
{
	IIC_SDA_OUT();
	IIC_SDA(1);
	IIC_SCL(1);
	IIC_Delay();
	IIC_SDA(0);
	IIC_Delay();
	IIC_SCL(0);
}


//IIC停止信号
void IIC_Stop(void)
{
	IIC_SDA_OUT();
	IIC_SDA(0);
	IIC_SCL(0);
	IIC_Delay();
	IIC_SDA(1);
	IIC_SCL(1);
	IIC_Delay();
}




//等待应答信号到来 1失败 0成功
uint8_t IIC_WaitAck(void)
{
	uint8_t ucErrTime = 0;
	IIC_SDA_IN();
	IIC_SDA(1);
	IIC_Delay();
	IIC_SCL(1);
	IIC_Delay();
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);
	return 0;
}


//产生ACK
void IIC_Ack(void)
{
	IIC_SCL(0);
	IIC_SDA_OUT();
	IIC_SDA(0);
	IIC_Delay();
	IIC_SCL(1);
	IIC_Delay();
	IIC_SCL(0);
}


//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	IIC_SDA_OUT();
	IIC_SDA(1);
	IIC_Delay();
	IIC_SCL(1);
	IIC_Delay();
	IIC_SCL(0);
}	




void IIC_Send_Byte(uint8_t txd)
{
	uint8_t i;
	IIC_SDA_OUT();
	IIC_SCL(0);
	for(i = 0; i < 8; i++)
	{
		IIC_SDA((txd&0x80)>>7);
		txd<<=1;
		IIC_SCL(1);
		IIC_Delay();
		IIC_SCL(0);
		IIC_Delay();
	}
}



uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	IIC_SDA_IN();
	for(i = 0; i < 8; i++)
	{
		IIC_SCL(0);
		IIC_Delay();
		IIC_SCL(1);
		receive<<=1;
		if(READ_SDA)receive++;
		IIC_Delay();
	}
	if(!ack)
		IIC_NAck();
	else
		IIC_Ack();
	return receive;
}


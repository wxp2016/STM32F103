#include "mt_drv_spi.h"


/*
VCC         3.3V
GND         ��Դ��
SCL         SPI_CLK(IIC_SCL)        P01
SDA         SPI_MOSI(IIC_SDA)       P03
SD0/SA0     SPI_MISO(IIC_SA0)       P02
CS          SPI(0=SPI Mode)         P04
INT1        �ж�1                   P29
INT2        �ж�2                   P07
ADC1        ģ������1        
ADC2        ģ������2    
ADC3        ģ������3
*/   

//SCK---SCL-----------PA5
//MISO--SD0/SA0-------PA6
//MOSI--SDA-----------PA7
//CS----CS------------PC4
//INT1--INT1----------PA0
//INT2--INT2----------PB1



/******************************************************************************
* Name       : void MT_Drv_SPI_Init(void);
* Function   : SPI��ʼ��
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void MT_Drv_SPI_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC, ENABLE);

    /*!< SPI_FLASH_SPI Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
 
  
    //SCK--PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //MISO  PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //MOSI  PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //CS  PC4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    
    SPI_CS_HIGH();

    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    
    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE);
    
    SPI_ReadWriteByte(0xff);//��������	
}




uint8_t SPI_ReadByte(void)
{
    return (SPI_SendByte(Dummy_Byte));
}




//SPI����(д)һ���ֽ�
uint8_t SPI_SendByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, byte);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}


uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
    u8 retry=0;     
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}    
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}             
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����     
}





void SPI_SetSpeed(uint8_t Speed)
{
    SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=Speed;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
}






int32_t MT_SPI_Read(void *handle,uint8_t Reg,uint8_t *Bufp,uint16_t len)
{
    uint8_t i;
    
    SPI_CS_LOW();
    
    SPI_ReadWriteByte((Reg|0x80)&0xBF);
    for(i = 0; i<len; i++)
    {
        Bufp[i] = SPI_ReadWriteByte(Dummy_Byte);
    }
    
    SPI_CS_HIGH(); 
    
    return 0;
}



int32_t MT_SPI_Write(void *handle,uint8_t Reg,uint8_t *Bufp,uint16_t len)
{
    uint8_t i;
    
    handle = handle;

    SPI_CS_LOW();  
    
    SPI_ReadWriteByte(Reg & 0x7F);
    for(i=0; i<len; i++)
    {
        SPI_ReadWriteByte(*Bufp++);
    }
    SPI_CS_HIGH();
    
    return 0;
}





























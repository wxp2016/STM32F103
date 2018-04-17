#ifndef MT_DRV_IIC_H__
#define MT_DRV_IIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "mt_drv_systick.h"
#include "stm32f10x_bitband.h"

//SDA--PB11
//SCL--PB10
  

#define SDA_PORT	    GPIOB
#define SDA_PIN		    GPIO_Pin_11
#define SDA_PORT_RCC    RCC_APB2Periph_GPIOB 
  
  
#define SCL_PORT	    GPIOB
#define SCL_PIN		    GPIO_Pin_10
#define SCL_PORT_RCC    RCC_APB2Periph_GPIOB



#define IIC_Delay()		MT_Delay_us(2)
#define IIC_SDA(a)		if(a) 	GPIO_SetBits(SDA_PORT,SDA_PIN); \
										else		GPIO_ResetBits(SDA_PORT,SDA_PIN);
										
#define IIC_SCL(a)		if(a) 	GPIO_SetBits(SCL_PORT,SCL_PIN); \
										else		GPIO_ResetBits(SCL_PORT,SCL_PIN);


#define READ_SDA	GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN)

     
void IIC_SDA_OUT(void);//SDA配置为输出
void IIC_SDA_IN(void);//SDA配置为输入




void MT_Drv_IIC_Init(void);//IIC 初始化		
void IIC_Start(void);//IIC起始信号
void IIC_Stop(void);//IIC停止信号
uint8_t IIC_WaitAck(void);//等待应答信号到来 1失败 0成功
void IIC_Ack(void);//产生ACK
void IIC_NAck(void);//不产生Ack信号

void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(uint8_t ack);



#ifdef __cplusplus
}
#endif

#endif // MT_DRV_IIC_H__

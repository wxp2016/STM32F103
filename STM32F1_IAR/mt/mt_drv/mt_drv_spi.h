#ifndef __MT_DRV_SPI_H__
#define __MT_DRV_SPI_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "stm32f10x.h"

//SCK---SCL-----------PA5
//MISO--SD0/SA0-------PA6
//MOSI--SDA-----------PA7
//CS----CS------------PC4
//INT1--INT1----------PA0
//INT2--INT2----------PB1 
   
   
#define SPI_CS_LOW()       GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define SPI_CS_HIGH()      GPIO_SetBits(GPIOC, GPIO_Pin_4)     



#define Dummy_Byte            0xFF   //虚字节     

void MT_Drv_SPI_Init(void);
int32_t MT_SPI_Read(void *handle,uint8_t Reg,uint8_t *Bufp,uint16_t len);
int32_t MT_SPI_Write(void *handle,uint8_t Reg,uint8_t *Bufp,uint16_t len);



uint8_t SPI_ReadByte(void);//SPI读字节
uint8_t SPI_SendByte(uint8_t byte);//SPI发送一个字节
uint8_t SPI_ReadWriteByte(uint8_t TxData);//SPI读写一个字节
void SPI_SetSpeed(uint8_t Speed);





#ifdef __cplusplus
}
#endif	 
	 
#endif	//__MT_DRV_SPI_H__

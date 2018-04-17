#ifndef MT_DRV_UART_H__
#define MT_DRV_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

  
 
    
void MT_Drv_USART1_Init(u32 baud);


void SendData(u8 *buf,u8 size);
void USART2_Send(unsigned char *DataToSend ,uint8_t data_num);
void USART2_Init(u32 baud);	 
void USART3_Init(u32 baud);	 
void USART4_Init(u32 baud);	 
void USART5_Init(u32 baud);	  
  
  


#ifdef __cplusplus
}
#endif

#endif // MT_DRV_UART_H__
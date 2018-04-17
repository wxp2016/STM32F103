#ifndef MT_DRV_LED_H__
#define MT_DRV_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
  
  
  



//LED1 - PE0
#define LED1_RCC_PORT   RCC_APB2Periph_GPIOE   
#define LED1_PORT       GPIOE
#define LED1_PIN        GPIO_Pin_0   
  
//LED2 - PE5  
#define LED2_RCC_PROT   RCC_APB2Periph_GPIOE
#define LED2_PORT       GPIOE
#define LED2_PIN        GPIO_Pin_5  
  
//LED3 - PB5  
#define LED3_RCC_PROT   RCC_APB2Periph_GPIOB
#define LED3_PORT       GPIOB
#define LED3_PIN        GPIO_Pin_5 




/*
#define LED2(a)			if(a) 	GPIO_SetBits(GPIOE,GPIO_Pin_5); \
										else		GPIO_ResetBits(GPIOE,GPIO_Pin_5);

#define LED3(a)			if(a) 	GPIO_SetBits(GPIOB,GPIO_Pin_5); \
										else		GPIO_ResetBits(GPIOB,GPIO_Pin_5);	
																							
*/
     
     
     
     
     
void MT_Drv_LED_Init(void);//LED GPIO≥ı ºªØ				
																							  
  


#ifdef __cplusplus
}
#endif

#endif // MT_DRV_LED_H__
#include "mt_drv_led.h"



void MT_Drv_LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(LED2_RCC_PROT, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = LED2_PIN;				   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);					 
	GPIO_SetBits(LED2_PORT,LED2_PIN);
    
    RCC_APB2PeriphClockCmd(LED3_RCC_PROT, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = LED3_PIN;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);				
	GPIO_SetBits(LED3_PORT,LED3_PIN);
}
    
    
    
    
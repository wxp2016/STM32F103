/******************************************************************************
* File       : bsp_nvic.h
* Function   : 中断配置源文件
* Description: None          
* Version    : V1.00
* Author     : WXP
* Date       : 
* History    :  
*	No.					|Date						|Author					|Description
	1					|2017-4-21					|WXP					|初次创建
******************************************************************************/
#include "mt_hal_nvic.h"


//外设中断配置函数指针
MT_Peripheral_Int_Config mt_peripheral_int = NULL;


//EXTI0-PA0
void PA_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//EXTI1-PB1
void PB_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//EXTI2-PC2
void PC_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//EXTI3-PD3
void PD_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//EXTI4-PE4
void PE_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




void TIM3_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure)
{
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}





void TIM6_NVIC_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}



void USART2_NVIC_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}



void TIM7_NVIC_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


void USART1_NVIC_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}







void MT_NVIC_Config(INT_PRIORITY_EN priority)
{
	/* 配置中断使用组合  抢占式3位(0-7)，响应式1位(0-1) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    switch(priority)
    {
        case MT_PRIORITY_0:
          mt_peripheral_int = PA_NVIC_Config;//PA_NVIC_Config
          break;
        
        case MT_PRIORITY_1:
          mt_peripheral_int = PB_NVIC_Config;//PB_NVIC_Config
          break;
          
        case MT_PRIORITY_2:
          mt_peripheral_int = PD_NVIC_Config;
          break;
          
        case MT_PRIORITY_3:
          mt_peripheral_int = PE_NVIC_Config;
          break;
          
        case MT_PRIORITY_4:
          mt_peripheral_int = USART2_NVIC_Config;
          break;
          
        case MT_PRIORITY_5:
          //mt_peripheral_int
          break;
          
        case MT_PRIORITY_6:
          mt_peripheral_int = TIM7_NVIC_Config;
            
          break;
          
        case MT_PRIORITY_7:
          mt_peripheral_int = USART1_NVIC_Config;
          break;
    }
    (*mt_peripheral_int)();
}


 
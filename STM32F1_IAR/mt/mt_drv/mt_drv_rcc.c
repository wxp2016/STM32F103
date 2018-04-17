#include "mt_drv_rcc.h"




/******************************************************************************
* Name       : void MT_Drv_RCC_Init(void)
* Function   : 系统时钟初始化
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void MT_Drv_RCC_Init(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();									//RCC寄存器缺省置
	RCC_HSEConfig(RCC_HSE_ON);						//打开外部高速晶振HSE=8MHZ  			
	HSEStartUpStatus = RCC_WaitForHSEStartUp();		//等待HSE起振
	if(HSEStartUpStatus == SUCCESS)        			//SUCCESS：HSE晶振稳定且就绪
	{
		/*设置AHB时钟*/
		RCC_HCLKConfig(RCC_SYSCLK_Div1);  			//RCC_SYSCLK_Div1--AHB时钟=系统时钟=72MHZ
		/* 设置高速AHB时钟（PCLK2）*/
		RCC_PCLK2Config(RCC_HCLK_Div1);   			//RCC_HCLK_Div1--APB2时钟=HCLK=72MHZ
		/*设置低速AHB时钟（PCLK1）*/   
		RCC_PCLK1Config(RCC_HCLK_Div2);   			//RCC_HCLK_Div2--APB1时钟=HCLK/2=36MHZ
		/*设置FLASH存储器延时时钟周期数*/
		FLASH_SetLatency(FLASH_Latency_2); //FLASH_Latency_2  2延时周期
   
		/*选择FLASH预取指缓存的模式*/  
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);       //预取指缓存使能
		/*设置PLL时钟源及倍频系数*/
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     	//PLL9倍频 SYSCLK=9*8=72MHZ
		
		/*使能PLL */
		RCC_PLLCmd(ENABLE);
		/*检查指定的RCC标志位(PLL准备好标志)设置与否*/   
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)     		//等待PLL工作 
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);					//设置系统时钟	
		while(RCC_GetSYSCLKSource() != 0x08)        				//判断PLL是否是系统时钟
		{
		}
	}
}
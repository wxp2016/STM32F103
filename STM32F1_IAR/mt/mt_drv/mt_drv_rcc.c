#include "mt_drv_rcc.h"




/******************************************************************************
* Name       : void MT_Drv_RCC_Init(void)
* Function   : ϵͳʱ�ӳ�ʼ��
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
	RCC_DeInit();									//RCC�Ĵ���ȱʡ��
	RCC_HSEConfig(RCC_HSE_ON);						//���ⲿ���پ���HSE=8MHZ  			
	HSEStartUpStatus = RCC_WaitForHSEStartUp();		//�ȴ�HSE����
	if(HSEStartUpStatus == SUCCESS)        			//SUCCESS��HSE�����ȶ��Ҿ���
	{
		/*����AHBʱ��*/
		RCC_HCLKConfig(RCC_SYSCLK_Div1);  			//RCC_SYSCLK_Div1--AHBʱ��=ϵͳʱ��=72MHZ
		/* ���ø���AHBʱ�ӣ�PCLK2��*/
		RCC_PCLK2Config(RCC_HCLK_Div1);   			//RCC_HCLK_Div1--APB2ʱ��=HCLK=72MHZ
		/*���õ���AHBʱ�ӣ�PCLK1��*/   
		RCC_PCLK1Config(RCC_HCLK_Div2);   			//RCC_HCLK_Div2--APB1ʱ��=HCLK/2=36MHZ
		/*����FLASH�洢����ʱʱ��������*/
		FLASH_SetLatency(FLASH_Latency_2); //FLASH_Latency_2  2��ʱ����
   
		/*ѡ��FLASHԤȡָ�����ģʽ*/  
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);       //Ԥȡָ����ʹ��
		/*����PLLʱ��Դ����Ƶϵ��*/
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     	//PLL9��Ƶ SYSCLK=9*8=72MHZ
		
		/*ʹ��PLL */
		RCC_PLLCmd(ENABLE);
		/*���ָ����RCC��־λ(PLL׼���ñ�־)�������*/   
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)     		//�ȴ�PLL���� 
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);					//����ϵͳʱ��	
		while(RCC_GetSYSCLKSource() != 0x08)        				//�ж�PLL�Ƿ���ϵͳʱ��
		{
		}
	}
}
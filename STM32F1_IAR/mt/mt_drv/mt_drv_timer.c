#include "mt_drv_timer.h"

#include "mt_hal_led.h"
#include "mt_hal_nvic.h"




void MT_TIM2_Init(void)
{
    
}

void MT_TIM3_Init(void)
{

}

void MT_TIM4_Init(void)  
{

}

void MT_TIM5_Init(void)  
{

}

void MT_TIM6_Init(void)  
{

}

void MT_TIM7_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/*��ʼ��ΪĬ��ֵ*/
	TIM_DeInit(TIM7);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	MT_NVIC_Config(MT_PRIORITY_6);
	
	TIM_TimeBaseStructure.TIM_Period = 10000-1;		               //����ֵ:1000
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;    	           //Ԥ��Ƶ,����:72      1us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //ʱ�ӷ�Ƶ����Ϊ1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //���ϼ���
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	                // Time base configuration

	TIM_ARRPreloadConfig(TIM7, ENABLE);//ʹ��Ԥװ��
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM7, ENABLE);	
}


void TIM7_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�ж��Ƿ���TIM7�����ж�
   {
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);//���TIM7���жϴ�����λ
        
        
   }
}

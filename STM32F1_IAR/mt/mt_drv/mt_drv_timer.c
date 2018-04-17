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
	/*初始化为默认值*/
	TIM_DeInit(TIM7);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	MT_NVIC_Config(MT_PRIORITY_6);
	
	TIM_TimeBaseStructure.TIM_Period = 10000-1;		               //计数值:1000
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;    	           //预分频,除数:72      1us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //时钟分频因子为1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	                // Time base configuration

	TIM_ARRPreloadConfig(TIM7, ENABLE);//使能预装载
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM7, ENABLE);	
}


void TIM7_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//判断是否发生TIM7更新中断
   {
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);//清除TIM7的中断待处理位
        
        
   }
}

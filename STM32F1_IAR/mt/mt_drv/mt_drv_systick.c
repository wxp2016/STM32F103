#include "mt_drv_systick.h"




//systick 1us计数值变化量 usTicks=72=71999/1000
volatile uint32_t usTicks = 0;

//系统ms时间戳(从开机到现在运行的ms数)
volatile uint32_t sysTickUptime = 0;
 

/******************************************************************************
* Name       : void Delay_us(uint32_t us)
* Function   : us级延时
* Input      : uint32_t us 延时的us数
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 2017-4-21
* Description: 
******************************************************************************/
void MT_Delay_us(uint32_t us)
{
	uint32_t now = GetSysTime_us();
	while (GetSysTime_us() - now < us);
}


/******************************************************************************
* Name       : void Delay_ms(uint32_t ms)
* Function   : ms级延时
* Input      : uint32_t us 延时的ms数
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void MT_Delay_ms(uint32_t ms)
{
	while (ms--)
        MT_Delay_us(1000);
}



/******************************************************************************
* Name       : uint32_t GetSysTime_us(void)
* Function   : 获取系统运行的us时间戳 
* Input      : None
* Output:    : None
* Return     : 返回系统运行到现在的us数
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
1.SYSTICK为向下计数,从高往低 71999---0
2.ucTick*1000=1ms计数值=71999
3.(71999-当前计数值)/usTicks=当前1ms中已运行的us数
4.已运行的us数+前面的us数=系统当前运行的us数
******************************************************************************/
uint32_t GetSysTime_us(void) 
{
	register uint32_t ms, cycle_cnt;
	do{
		ms = sysTickUptime;												//系统从开机运行到现在的ms数
		cycle_cnt = SysTick->VAL;										//获取当前SysTick-24位计数器计数值
     }while (ms != sysTickUptime);										//
	return ((ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks);      //usTicks*1000表示1ms的计数值
}



/******************************************************************************
* Name       : void SysTick_Init(void)
* Function   : SYSTICK初始化  1ms中断1次
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
SystemCoreClock/1000    1ms中断一次
SystemCoreClock/100000  10us中断一次
SystemCoreClock/1000000 1us中断一次
******************************************************************************/
void MT_Drv_SysTick_Init(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
    usTicks = clocks.SYSCLK_Frequency / 1000000;//ucTicks=72000000/100000=72
	SysTick_Config(SystemCoreClock /1000);  	//滴答时钟定时周期为1ms
}	






extern uint8_t TIME_TASK_SYNC_FLAG;



/******************************************************************************
* Name       : void SysTick_Handler(void)
* Function   : systick中断处理函数
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 每中断1次sysTickUptime+1
******************************************************************************/
void SysTick_Handler(void)
{
	sysTickUptime++;
    
    if(!(sysTickUptime % 10))
    {
        TIME_TASK_SYNC_FLAG = 1;
    }
}




/******************************************************************************
* Name       : void Error_Handler(void)
* Function   : 错误处理含糊 此处直接阻塞
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
******************************************************************************/
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}
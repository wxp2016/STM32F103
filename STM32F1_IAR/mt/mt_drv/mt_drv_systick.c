#include "mt_drv_systick.h"




//systick 1us����ֵ�仯�� usTicks=72=71999/1000
volatile uint32_t usTicks = 0;

//ϵͳmsʱ���(�ӿ������������е�ms��)
volatile uint32_t sysTickUptime = 0;
 

/******************************************************************************
* Name       : void Delay_us(uint32_t us)
* Function   : us����ʱ
* Input      : uint32_t us ��ʱ��us��
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
* Function   : ms����ʱ
* Input      : uint32_t us ��ʱ��ms��
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
* Function   : ��ȡϵͳ���е�usʱ��� 
* Input      : None
* Output:    : None
* Return     : ����ϵͳ���е����ڵ�us��
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
1.SYSTICKΪ���¼���,�Ӹ����� 71999---0
2.ucTick*1000=1ms����ֵ=71999
3.(71999-��ǰ����ֵ)/usTicks=��ǰ1ms�������е�us��
4.�����е�us��+ǰ���us��=ϵͳ��ǰ���е�us��
******************************************************************************/
uint32_t GetSysTime_us(void) 
{
	register uint32_t ms, cycle_cnt;
	do{
		ms = sysTickUptime;												//ϵͳ�ӿ������е����ڵ�ms��
		cycle_cnt = SysTick->VAL;										//��ȡ��ǰSysTick-24λ����������ֵ
     }while (ms != sysTickUptime);										//
	return ((ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks);      //usTicks*1000��ʾ1ms�ļ���ֵ
}



/******************************************************************************
* Name       : void SysTick_Init(void)
* Function   : SYSTICK��ʼ��  1ms�ж�1��
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: 
SystemCoreClock/1000    1ms�ж�һ��
SystemCoreClock/100000  10us�ж�һ��
SystemCoreClock/1000000 1us�ж�һ��
******************************************************************************/
void MT_Drv_SysTick_Init(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
    usTicks = clocks.SYSCLK_Frequency / 1000000;//ucTicks=72000000/100000=72
	SysTick_Config(SystemCoreClock /1000);  	//�δ�ʱ�Ӷ�ʱ����Ϊ1ms
}	






extern uint8_t TIME_TASK_SYNC_FLAG;



/******************************************************************************
* Name       : void SysTick_Handler(void)
* Function   : systick�жϴ�����
* Input      : None
* Output:    : None
* Return     : None
* Version    : V1.00
* Author     : WXP
* Date       : 
* Description: ÿ�ж�1��sysTickUptime+1
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
* Function   : �������� �˴�ֱ������
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
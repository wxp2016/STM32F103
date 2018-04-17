#ifndef MT_DRV_SYSTICK_H__
#define MT_DRV_SYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
  
 
void MT_Drv_SysTick_Init(void);

void MT_Delay_ms(uint32_t ms);//ms级延时
void MT_Delay_us(uint32_t us);//us级延时
uint32_t GetSysTime_us(void);//获取当前运行us值 
void Error_Handler(void);//错误处理函数 
  
  


#ifdef __cplusplus
}
#endif

#endif // MT_DRV_SYSTICK_H__
#ifndef MT_DRV_SYSTICK_H__
#define MT_DRV_SYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
  
 
void MT_Drv_SysTick_Init(void);

void MT_Delay_ms(uint32_t ms);//ms����ʱ
void MT_Delay_us(uint32_t us);//us����ʱ
uint32_t GetSysTime_us(void);//��ȡ��ǰ����usֵ 
void Error_Handler(void);//�������� 
  
  


#ifdef __cplusplus
}
#endif

#endif // MT_DRV_SYSTICK_H__
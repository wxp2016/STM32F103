#ifndef __MT_APP_LED_H__
#define __MT_APP_LED_H__

#include "stm32f10x.h"






//LED循环任务
void App_Led_Loop(void);

//读取IO电平任务
void App_Led_Task_Init(void);
void App_Led_Task(void *ptmr, void *parg);




#endif






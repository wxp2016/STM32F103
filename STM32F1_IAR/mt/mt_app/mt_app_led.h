#ifndef __MT_APP_LED_H__
#define __MT_APP_LED_H__

#include "stm32f10x.h"






//LEDѭ������
void App_Led_Loop(void);

//��ȡIO��ƽ����
void App_Led_Task_Init(void);
void App_Led_Task(void *ptmr, void *parg);




#endif






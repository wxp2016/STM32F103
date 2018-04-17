#ifndef __MT_APP_GESTURE_H__
#define __MT_APP_GESTURE_H__

#include "stm32f10x.h"







void App_Gesture_Loop(void);

void App_Gesture_Task_Init(void);
void App_Gesture_Task(void *ptmr, void *parg);




#endif
#ifndef MT_APP_H__
#define MT_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "app_softwaretimer.h"
#include "mt_app_led.h"
#include "mt_app_gesture.h"
 

  
  

void MT_App_Init(void);
void App_Time_Task_Loop(void);
  
  
  


#ifdef __cplusplus
}
#endif

#endif // MT_APP_H__
#ifndef MT_HAL_LED_H__
#define MT_HAL_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
  
  
  
enum LED_NUMBER_T 
{
    LED_NUM_1 = 1,
    LED_NUM_2,
    LED_NUM_3,
};




    
     
		
void MT_Hal_LED_On(enum LED_NUMBER_T led_num);
void MT_Hal_LED_Off(enum LED_NUMBER_T led_num);
void MT_Hal_LED_Toggle(enum LED_NUMBER_T led_num);	
																							  
  


#ifdef __cplusplus
}
#endif

#endif // MT_HAL_LED_H__
#ifndef __MT_HAL_NVIC_H__
#define __MT_HAL_NVIC_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "misc.h"
#include <stdio.h>
   
typedef enum
{
   MT_PRIORITY_0 = 0,
   MT_PRIORITY_1,
   MT_PRIORITY_2,
   MT_PRIORITY_3,
   MT_PRIORITY_4,
   MT_PRIORITY_5,
   MT_PRIORITY_6,
   MT_PRIORITY_7,
}INT_PRIORITY_EN;
   

void MT_NVIC_Config(INT_PRIORITY_EN priority);			
typedef void (*MT_Peripheral_Int_Config)(void);

#ifdef __cplusplus
}
#endif
	 
#endif	//__MT_HAL_NVIC_H__

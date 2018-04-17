#ifndef __MT_DRV_EXTI_H__
#define __MT_DRV_EXTI_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
   
    

   
void MT_EXTI_Init(void);
void MT_EXTI_PA0_Config(void);
void MT_EXTI_PB1_Config(void);
void MT_EXTI_PC2_Config(void);
void MT_EXTI_PD3_Config(void);
void MT_EXTI_PE4_Config(void);


     
void ENABLE_EXTI0(void);      
void DISABLE_EXTI0(void); 
void ENABLE_EXTI1(void);      
void DISABLE_EXTI1(void);      
     





#ifdef __cplusplus
}
#endif
	 
#endif	//__MT_DRV_EXTI_H__

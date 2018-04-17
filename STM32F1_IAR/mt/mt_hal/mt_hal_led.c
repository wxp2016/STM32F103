#include "mt_hal_led.h"
#include "mt_drv_led.h"



void MT_Hal_LED_On(enum LED_NUMBER_T led_num)
{
    enum LED_NUMBER_T l_number = led_num;
    
    switch(l_number)
    {
        case LED_NUM_1:
            GPIO_ResetBits(LED1_PORT, LED1_PIN);
            break;
        case LED_NUM_2:
            GPIO_ResetBits(LED2_PORT, LED2_PIN);
            break;
        case LED_NUM_3:
            GPIO_ResetBits(LED3_PORT, LED3_PIN);
            break;    
    } 
}    



void MT_Hal_LED_Off(enum LED_NUMBER_T led_num)
{
    enum LED_NUMBER_T l_number = led_num;
    
    switch(l_number)
    {
        case LED_NUM_1:
            GPIO_SetBits(LED1_PORT, LED1_PIN);
            break;
        case LED_NUM_2:
            GPIO_SetBits(LED2_PORT, LED2_PIN);
            break;
        case LED_NUM_3:
            GPIO_SetBits(LED3_PORT, LED3_PIN);
            break;    
    } 
}



void MT_Hal_LED_Toggle(enum LED_NUMBER_T led_num)
{
    enum LED_NUMBER_T l_number = led_num;
    
    switch(l_number)
    {
        case LED_NUM_1:
            GPIO_WriteBit(LED1_PORT, LED1_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN)));
            break;
        case LED_NUM_2:
            GPIO_WriteBit(LED2_PORT, LED2_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(LED2_PORT,LED2_PIN)));
            break;
        case LED_NUM_3:
            GPIO_WriteBit(LED3_PORT, LED3_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(LED3_PORT,LED3_PIN)));
            break;    
    } 
}


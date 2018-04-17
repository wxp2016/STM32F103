#include "mt_drv.h"

//Peripheral
#include "mt_drv_rcc.h"
#include "mt_drv_led.h"
#include "mt_drv_uart.h" 
#include "mt_drv_systick.h"
#include "mt_drv_exti.h"
#include "mt_drv_timer.h"
#include "mt_drv_iic.h"
#include "mt_drv_spi.h"

//Module
#include "mt_module.h"
#include "apds_9960.h"

void MT_Drv_Init(void)
{
	//ÍâÉè
	MT_Drv_RCC_Init(); 
    MT_Drv_SysTick_Init();
    MT_Drv_USART1_Init(115200);
    MT_EXTI_Init();
    //MT_TIM7_Init();
    MT_Drv_LED_Init();
	//MT_Drv_IIC_Init();
    MT_Drv_SPI_Init();


	//Ä£¿é

	
}


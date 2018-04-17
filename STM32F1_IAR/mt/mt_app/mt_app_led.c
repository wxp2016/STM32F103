#include "mt_app_led.h"
#include "app_softwaretimer.h"
#include "mt_hal_led.h"


#include "apds_9960.h"


//#define APP_LED_DEBUG







//APP_LED TIMER
#define APP_LED_TMR_DLY		(0)
#define APP_LED_TMR_PERIOD	(50)
WXP_TMR	*app_led_tmr = (WXP_TMR*)0;







void App_Led_Task_Init(void)
{

	uint8_t ret;
	uint8_t err;

	app_led_tmr = WXPTmrCreate(	APP_LED_TMR_DLY,
								APP_LED_TMR_PERIOD,
								WXP_TMR_OPT_PERIODIC,
								(WXP_TMR_CALLBACK)App_Led_Task,
								(void*)0,
								"app_led_tmr",
								&err
							   );

	if(app_led_tmr == (WXP_TMR *)0)
	{
#ifdef APP_LED_DEBUG
		printf("create app_led_tmr err\r\n");
		printf("err num: %d\r\n",err);
#endif
		while(1);
	}
										
	ret = WXPTmrStart(app_led_tmr,&err);

	if(ret == WXP_FALSE)
	{
#ifdef APP_LED_DEBUG	
		printf("start app_led_tmr err\r\n");
		printf("err num: %d\r\n",err);
#endif		
		while(1);
	}
}



void App_Led_Task(void *ptmr, void *parg)
{
#ifdef APP_LED_DEBUG
	printf("%s\r\n",__func__);
#endif
	App_Led_Loop();
}





void App_Led_Loop(void)
{
	MT_Hal_LED_Toggle(LED_NUM_2);
    MT_Hal_LED_Toggle(LED_NUM_3);  
}


















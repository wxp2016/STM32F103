#include "mt_app_gesture.h"
#include "app_softwaretimer.h"


#include "apds_9960.h"


//#define APP_GESTURE_DEBUG

//APP_GESTURE_TIMER
#define APP_GESTURE_TMR_DLY		(0)
#define APP_GESTURE_TMR_PERIOD	(1)
WXP_TMR	*app_gesture_tmr = (WXP_TMR*)0;







void App_Gesture_Task_Init(void)
{

	uint8_t ret;
	uint8_t err;

	app_gesture_tmr = WXPTmrCreate(	APP_GESTURE_TMR_DLY,
								APP_GESTURE_TMR_PERIOD,
								WXP_TMR_OPT_PERIODIC,
								(WXP_TMR_CALLBACK)App_Gesture_Task,
								(void*)0,
								"app_gesture_tmr",
								&err
							   );

	if(app_gesture_tmr == (WXP_TMR *)0)
	{
#ifdef APP_GESTURE_DEBUG
		printf("create app_gesture_tmr err\r\n");
		printf("err num: %d\r\n",err);
#endif
		while(1);
	}
										
	ret = WXPTmrStart(app_gesture_tmr,&err);

	if(ret == WXP_FALSE)
	{
#ifdef APP_GESTURE_DEBUG	
		printf("start app_gesture_tmr err\r\n");
		printf("err num: %d\r\n",err);
#endif		
		while(1);
	}
}



void App_Gesture_Task(void *ptmr, void *parg)
{
#ifdef APP_GESTURE_DEBUG
	printf("%s\r\n",__func__);
#endif
	App_Gesture_Loop();
}





void App_Gesture_Loop(void)
{
    HandleGesture();
}


















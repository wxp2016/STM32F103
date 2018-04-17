#include "mt_app.h"
#include "mt_drv.h"
#include "mt_hal.h"

#include "mt_module.h"
#include "mt_drv_systick.h"

uint8_t TIME_TASK_SYNC_FLAG = 0;

uint8_t isr_flag = 0;

void MT_App_Init(void)
{
    MT_Drv_Init();
    MT_Hal_Init();
    WXPTmr_Init();
    App_Led_Task_Init();
    //App_Gesture_Task_Init();
}


void App_Time_Task_Loop(void)
{
	if(TIME_TASK_SYNC_FLAG)
    {
        WXPTmr_Task();
        TIME_TASK_SYNC_FLAG = 0;
    }	
}
/******************************************************************************
* File       : main.c
* Function   : ���ļ�
* Description: None          
* Version    : V1.00
* Author     : WXP
* Date       : 
* History    :  
******************************************************************************/
#include <stdio.h>
#include "mt_app.h"
  



int main(void)
{		
    MT_App_Init();
    printf("Init Finish...\n");
    
    while(1)
    { 
        App_Time_Task_Loop();//��ʱ����ִ��
    }
} 




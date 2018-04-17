#ifndef __APP_SOFTWARE_TIMER_H__
#define __APP_SOFTWARE_TIMER_H__



#include "stm32f10x.h"

//��ʱ��״̬
#define  	WXP_TMR_STATE_UNUSED           0u					//δʹ��
#define  	WXP_TMR_STATE_STOPPED          1u					//ֹͣ
#define  	WXP_TMR_STATE_COMPLETED        2u					//�������
#define  	WXP_TMR_STATE_RUNNING          3u					//����


//��ʱ������ѡ��
#define  	WXP_TMR_OPT_NONE           	   0u					//�޲���  
#define  	WXP_TMR_OPT_ONE_SHOT           1u  					//���ζ�ʱ��
#define  	WXP_TMR_OPT_PERIODIC           2u 					//���ڶ�ʱ��
#define  	WXP_TMR_OPT_CALLBACK           3u  					//ֹͣ��ʱ����ԭ���Ļص���������
#define  	WXP_TMR_OPT_CALLBACK_ARG       4u  					//ֹͣ��ʱ�����µĻص���������


//��ʱ������
#define  	WXP_TMR_EN                 		1u    				//ʹ�������ʱ��
#define  	WXP_TMR_CFG_MAX           		16u 				//�����ʱ�������Ŀ   			
#define  	WXP_TMR_CFG_WHEEL_SIZE     		8u   				//�����ʱ�������ߴ�
#define  	WXP_TMR_CFG_TICKS_PER_SEC 		10u					//�����ʱ����������Ƶ��    
#define		WXP_TASK_TMR_STK_SIZE			128u				//��ʱ������ջ��С


//��ʱ������
#define 	WXP_TMR_TYPE                  	1u					//��ʱ������
#define  	WXP_TMR_LINK_DLY       			0u					//����
#define  	WXP_TMR_LINK_PERIODIC  			1u					//����


//������
#define 	WXP_ERR_TMR_INVALID_DLY        	130u
#define 	WXP_ERR_TMR_INVALID_PERIOD     	131u
#define 	WXP_ERR_TMR_INVALID_OPT        	132u
#define 	WXP_ERR_TMR_INVALID_NAME       	133u
#define 	WXP_ERR_TMR_NON_AVAIL          	134u
#define 	WXP_ERR_TMR_INACTIVE           	135u
#define 	WXP_ERR_TMR_INVALID_DEST       	136u
#define 	WXP_ERR_TMR_INVALID_TYPE       	137u
#define 	WXP_ERR_TMR_INVALID            	138u
#define 	WXP_ERR_TMR_ISR                	139u
#define 	WXP_ERR_TMR_NAME_TOO_LONG      	140u
#define 	WXP_ERR_TMR_INVALID_STATE      	141u
#define 	WXP_ERR_TMR_STOPPED            	142u
#define 	WXP_ERR_TMR_NO_CALLBACK        	143u
#define 	WXP_ERR_NONE					0u


#define 	WXP_TRUE						1u
#define 	WXP_FALSE						0u



//�ص�����ָ��
typedef  void (*WXP_TMR_CALLBACK)(void *ptmr, void *parg);


//WXP_TIMER
typedef struct wxp_tmr
{
	uint8_t 				WXPTmrType;				//����
	uint8_t*				WXPTmrName;				//��ʱ������
	WXP_TMR_CALLBACK		WXPTmrCallback;			//�ص�����ָ��
	void*					WXPTmrCallbackArg;		//�ص���������
	void*					WXPTmrNext;				//˫������--next
	void*					WXPTmrPrev;				//˫������--prev
	uint32_t  				WXPTmrMatch;			//��ʱʱ�䵽ƥ��ֵ ��TICK�Ƚ�
	uint32_t 				WXPTmrDly;				//���ζ�ʱ����ֵ�������ڶ�ʱ���ĵ�һ�ζ�ʱֵ
	uint32_t 				WXPTmrPeriod;			//���ڶ�ʱ��������
	uint8_t 				WXPTmrOpt;				//��������
	uint8_t 				WXPTmrState;			//��ʱ��״̬
}WXP_TMR;


typedef struct wxp_tmr_wheel
{
	WXP_TMR*	WXPTmrFirst;						//ָ�������еĵ�һ����ʱ��
	uint16_t 	WXPTmrEntries;						//������ʱ����Ŀ
}WXP_TMR_WHEEL;



/*
WXP_TMR			         	WXPTmrTbl[WXP_TMR_CFG_MAX]; 				//��ʱ����
WXP_TMR*					WXPTmrFreeList;           					//ָ����ж�ʱ�������ָ��
uint16_t           			WXPTmrTaskStk[WXP_TASK_TMR_STK_SIZE];		//��ʱ��������ջ��С
WXP_TMR_WHEEL      			WXPTmrWheelTbl[WXP_TMR_CFG_WHEEL_SIZE];		//��ʱ���ֱ�
*/


void WXP_MemClr(uint8_t* pdest,uint16_t size);//���
void WXPTmr_Init(void);//�����ʱ����ʼ��		
void WXPTmr_Task(void);//��ʱ������


//����һ����ʱ��
WXP_TMR* WXPTmrCreate(		uint32_t 			dly,
							uint32_t			period,
							uint8_t 			opt,
							WXP_TMR_CALLBACK	callback,
							void*				callback_arg,
							uint8_t 			*pname,
							uint8_t				*perr	);
uint8_t	WXPTmrDel(WXP_TMR* ptmr,uint8_t* perr);//ɾ��һ����ʱ��


uint8_t WXPTmrStart(WXP_TMR* ptmr,uint8_t *perr);//����һ����ʱ��
uint8_t WXPTmrStop(WXP_TMR* 		ptmr,
						uint8_t 	opt,
						void*		callback_arg,
						uint8_t*	perr);



static WXP_TMR* WXPTmr_Alloc(void);		//�ӿ��ж�ʱ���б��з���һ����ʱ��
static void	WXPTmr_Free(WXP_TMR* ptmr);//�ͷ�һ����ʱ�������ж�ʱ������

static void WXPTmr_Link(WXP_TMR* ptmr,uint8_t type);//����һ��������ʱ����
static void WXPTmr_Unlink(WXP_TMR* ptmr);//�Ӷ�ʱ�������Ƴ�һ����ʱ��

#endif







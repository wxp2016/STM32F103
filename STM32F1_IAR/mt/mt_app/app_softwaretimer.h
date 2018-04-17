#ifndef __APP_SOFTWARE_TIMER_H__
#define __APP_SOFTWARE_TIMER_H__



#include "stm32f10x.h"

//定时器状态
#define  	WXP_TMR_STATE_UNUSED           0u					//未使用
#define  	WXP_TMR_STATE_STOPPED          1u					//停止
#define  	WXP_TMR_STATE_COMPLETED        2u					//运行完成
#define  	WXP_TMR_STATE_RUNNING          3u					//运行


//定时器操作选项
#define  	WXP_TMR_OPT_NONE           	   0u					//无操作  
#define  	WXP_TMR_OPT_ONE_SHOT           1u  					//单次定时器
#define  	WXP_TMR_OPT_PERIODIC           2u 					//周期定时器
#define  	WXP_TMR_OPT_CALLBACK           3u  					//停止定时器用原来的回调函数参数
#define  	WXP_TMR_OPT_CALLBACK_ARG       4u  					//停止定时器用新的回调函数参数


//定时器配置
#define  	WXP_TMR_EN                 		1u    				//使能软件定时器
#define  	WXP_TMR_CFG_MAX           		16u 				//软件定时器最大数目   			
#define  	WXP_TMR_CFG_WHEEL_SIZE     		8u   				//软件定时器辐条尺寸
#define  	WXP_TMR_CFG_TICKS_PER_SEC 		10u					//软件定时器管理任务频率    
#define		WXP_TASK_TMR_STK_SIZE			128u				//定时器任务栈大小


//定时器类型
#define 	WXP_TMR_TYPE                  	1u					//定时器类型
#define  	WXP_TMR_LINK_DLY       			0u					//单次
#define  	WXP_TMR_LINK_PERIODIC  			1u					//周期


//错误码
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



//回调函数指针
typedef  void (*WXP_TMR_CALLBACK)(void *ptmr, void *parg);


//WXP_TIMER
typedef struct wxp_tmr
{
	uint8_t 				WXPTmrType;				//类型
	uint8_t*				WXPTmrName;				//定时器名称
	WXP_TMR_CALLBACK		WXPTmrCallback;			//回调函数指针
	void*					WXPTmrCallbackArg;		//回调函数参数
	void*					WXPTmrNext;				//双向链表--next
	void*					WXPTmrPrev;				//双向链表--prev
	uint32_t  				WXPTmrMatch;			//定时时间到匹配值 与TICK比较
	uint32_t 				WXPTmrDly;				//单次定时器初值或者周期定时器的第一次定时值
	uint32_t 				WXPTmrPeriod;			//周期定时器的周期
	uint8_t 				WXPTmrOpt;				//操作类型
	uint8_t 				WXPTmrState;			//定时器状态
}WXP_TMR;


typedef struct wxp_tmr_wheel
{
	WXP_TMR*	WXPTmrFirst;						//指向链表中的第一个定时器
	uint16_t 	WXPTmrEntries;						//辐条定时器数目
}WXP_TMR_WHEEL;



/*
WXP_TMR			         	WXPTmrTbl[WXP_TMR_CFG_MAX]; 				//定时器表
WXP_TMR*					WXPTmrFreeList;           					//指向空闲定时器链表的指针
uint16_t           			WXPTmrTaskStk[WXP_TASK_TMR_STK_SIZE];		//定时管理任务栈大小
WXP_TMR_WHEEL      			WXPTmrWheelTbl[WXP_TMR_CFG_WHEEL_SIZE];		//定时器轮表
*/


void WXP_MemClr(uint8_t* pdest,uint16_t size);//清空
void WXPTmr_Init(void);//软件定时器初始化		
void WXPTmr_Task(void);//定时器任务


//创建一个定时器
WXP_TMR* WXPTmrCreate(		uint32_t 			dly,
							uint32_t			period,
							uint8_t 			opt,
							WXP_TMR_CALLBACK	callback,
							void*				callback_arg,
							uint8_t 			*pname,
							uint8_t				*perr	);
uint8_t	WXPTmrDel(WXP_TMR* ptmr,uint8_t* perr);//删除一个定时器


uint8_t WXPTmrStart(WXP_TMR* ptmr,uint8_t *perr);//启动一个定时器
uint8_t WXPTmrStop(WXP_TMR* 		ptmr,
						uint8_t 	opt,
						void*		callback_arg,
						uint8_t*	perr);



static WXP_TMR* WXPTmr_Alloc(void);		//从空闲定时器列表中分配一个定时器
static void	WXPTmr_Free(WXP_TMR* ptmr);//释放一个定时器到空闲定时器链表

static void WXPTmr_Link(WXP_TMR* ptmr,uint8_t type);//插入一个链表到定时器轮
static void WXPTmr_Unlink(WXP_TMR* ptmr);//从定时器轮中移除一个定时器

#endif







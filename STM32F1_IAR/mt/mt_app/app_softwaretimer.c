#include "app_softwaretimer.h"

uint16_t           			WXPTmrFree;                					//定时器池中的空闲数目
uint16_t            		WXPTmrUsed;                					//已使用的定时器数目
uint32_t            		WXPTmrTime;                					//当前计数值


WXP_TMR			         	WXPTmrTbl[WXP_TMR_CFG_MAX]; 				//定时器池
WXP_TMR*					WXPTmrFreeList;           					//指向空闲定时器列表的指针
uint16_t           			WXPTmrTaskStk[WXP_TASK_TMR_STK_SIZE];		//定时管理任务栈大小
WXP_TMR_WHEEL      			WXPTmrWheelTbl[WXP_TMR_CFG_WHEEL_SIZE];		//定时器轮表









//创建一个定时器
WXP_TMR* WXPTmrCreate(	uint32_t 				dly,
							uint32_t			period,
							uint8_t 			opt,
							WXP_TMR_CALLBACK	callback,
							void*				callback_arg,
							uint8_t 			*pname,
							uint8_t				*perr)
{
	WXP_TMR 	*ptmr;

	switch(opt)
	{
		case WXP_TMR_OPT_PERIODIC:
			if(period == 0)
			{
				*perr = WXP_ERR_TMR_INVALID_PERIOD;
				return ((WXP_TMR *)0);
			}
			break;
		case WXP_TMR_OPT_ONE_SHOT:
			if(dly == 0)
			{
				*perr = WXP_ERR_TMR_INVALID_DLY;
				return ((WXP_TMR *)0);
			}
                        break;
		default:
			*perr = WXP_ERR_TMR_INVALID_OPT;
			return ((WXP_TMR *)0);
	}

	ptmr = WXPTmr_Alloc();
	
	if(ptmr == (WXP_TMR *)0)
	{
		*perr = WXP_ERR_TMR_NON_AVAIL;
		return ((WXP_TMR *)0);
	}

	ptmr->WXPTmrState 		= WXP_TMR_STATE_STOPPED;
	ptmr->WXPTmrType		= WXP_TMR_TYPE;
	ptmr->WXPTmrDly			= dly;
	ptmr->WXPTmrPeriod  	= period;
	ptmr->WXPTmrOpt			= opt;
	ptmr->WXPTmrCallback	= callback;
	ptmr->WXPTmrCallbackArg = callback_arg;
	ptmr->WXPTmrName		= pname;

	*perr = WXP_ERR_NONE;

	return (ptmr);

}




//删除一个定时器
uint8_t	WXPTmrDel(WXP_TMR* ptmr,uint8_t* perr)
{
	if(ptmr == (WXP_TMR *)0)
	{
		*perr = WXP_ERR_TMR_INVALID;
		return WXP_FALSE;
	}

	if(ptmr->WXPTmrType != WXP_TMR_TYPE)
	{
		*perr = WXP_ERR_TMR_INVALID_TYPE;
		return WXP_FALSE;
	}

	switch(ptmr->WXPTmrState)
	{
		case WXP_TMR_STATE_RUNNING:
			WXPTmr_Unlink(ptmr);
			WXPTmr_Free(ptmr);
			*perr = WXP_ERR_NONE;
			return WXP_TRUE;
			
		case WXP_TMR_STATE_STOPPED:
		case WXP_TMR_STATE_COMPLETED:
			WXPTmr_Free(ptmr);
			*perr = WXP_ERR_NONE;
			return WXP_TRUE;
			
		case WXP_TMR_STATE_UNUSED:
			*perr = WXP_ERR_TMR_INACTIVE;
			return WXP_FALSE;

		default:
			*perr = WXP_ERR_TMR_INVALID_STATE;
			return WXP_FALSE;
	}
}






//启动一个定时器
uint8_t WXPTmrStart(WXP_TMR* ptmr,uint8_t *perr)
{
	if(ptmr == (WXP_TMR *)0)
	{
		*perr = WXP_ERR_TMR_INVALID;
		return WXP_FALSE;
	}
	if(ptmr->WXPTmrType != WXP_TMR_TYPE)
	{
		*perr = WXP_ERR_TMR_INVALID_TYPE;
		return WXP_FALSE;
	}
	switch(ptmr->WXPTmrState)
	{
		case WXP_TMR_STATE_RUNNING:
			WXPTmr_Unlink(ptmr);
			WXPTmr_Link(ptmr,WXP_TMR_LINK_DLY);
			*perr = WXP_ERR_NONE;
			return WXP_TRUE;
			
		case WXP_TMR_STATE_STOPPED:
		case WXP_TMR_STATE_COMPLETED:
			WXPTmr_Link(ptmr,WXP_TMR_LINK_DLY);
			*perr = WXP_ERR_NONE;
			return WXP_TRUE;
			
		case WXP_TMR_STATE_UNUSED:
			*perr = WXP_ERR_TMR_INACTIVE;
			return WXP_FALSE;

		default:
			*perr = WXP_ERR_TMR_INVALID_STATE;
			return WXP_FALSE;
	}
}



//停止一个定时器
uint8_t WXPTmrStop(WXP_TMR* 		ptmr,
						uint8_t 	opt,
						void*		callback_arg,
						uint8_t*	perr)
{
	WXP_TMR_CALLBACK	pfnct;

	if(ptmr == (WXP_TMR *)0)
	{
		*perr = WXP_ERR_TMR_INVALID;
		return WXP_FALSE;
	}

	if(ptmr->WXPTmrType != WXP_TMR_TYPE)
	{
		*perr = WXP_ERR_TMR_INVALID_TYPE;
		return WXP_FALSE;
	}

	switch(ptmr->WXPTmrState)
	{
		case WXP_TMR_STATE_RUNNING:
			WXPTmr_Unlink(ptmr);
			*perr = WXP_ERR_NONE;
			switch(opt)
			{
				case WXP_TMR_OPT_CALLBACK:
					pfnct = ptmr->WXPTmrCallback;
					if(pfnct != (WXP_TMR_CALLBACK)0)
					{
						(*pfnct)((void *)ptmr,ptmr->WXPTmrCallbackArg);//停止定时器之前执行回调 用旧的参数
					}
					else
					{
						*perr = WXP_ERR_TMR_NO_CALLBACK;
					}
					break;

				case WXP_TMR_OPT_CALLBACK_ARG:
					pfnct = ptmr->WXPTmrCallback;
					if(pfnct != (WXP_TMR_CALLBACK)0)
					{
						(*pfnct)((void *)ptmr,callback_arg);//停止定时器之前执行回调用新的参数
					}
					else
					{
						*perr = WXP_ERR_TMR_NO_CALLBACK;
					}
					break;

				case WXP_TMR_OPT_NONE:
                      break;

                 default:
                     *perr = WXP_ERR_TMR_INVALID_OPT;
                     break;	
			}
			return WXP_TRUE;
		
		case WXP_TMR_STATE_COMPLETED:
		case WXP_TMR_STATE_STOPPED:
			*perr = WXP_ERR_NONE;
			return WXP_TRUE;
			
		case WXP_TMR_STATE_UNUSED:
			*perr = WXP_ERR_TMR_INACTIVE;
			return WXP_FALSE;

		default:
			*perr = WXP_ERR_TMR_INVALID_STATE;
			return WXP_FALSE;
	}
	
}




//从空闲定时器列表中分配一个定时器
static WXP_TMR* WXPTmr_Alloc(void)
{
	WXP_TMR *ptmr;
	if(WXPTmrFreeList == (WXP_TMR*)0)
	{
		return ((WXP_TMR*)0);
	}
	ptmr				=	(WXP_TMR*)WXPTmrFreeList;			
	WXPTmrFreeList		=	(WXP_TMR*)ptmr->WXPTmrNext;
	ptmr->WXPTmrNext	=	(WXP_TMR*)0;
	ptmr->WXPTmrPrev	=	(WXP_TMR*)0;
	WXPTmrUsed++;	
	WXPTmrFree--;
	return (ptmr);
}



//释放一个定时器到空闲定时器链表
static void	WXPTmr_Free(WXP_TMR* ptmr)
{
	ptmr->WXPTmrState 		=	WXP_TMR_STATE_UNUSED;			//状态------>未使用
	ptmr->WXPTmrOpt			= 	WXP_TMR_OPT_NONE;				//操作选项-->None
	ptmr->WXPTmrPeriod		=	0;
	ptmr->WXPTmrMatch		=	0;
	ptmr->WXPTmrCallback	=	(WXP_TMR_CALLBACK)0;
	ptmr->WXPTmrName		=	(uint8_t*)(void*)"?";
	ptmr->WXPTmrPrev		=	(WXP_TMR*)0;
	ptmr->WXPTmrNext		=	WXPTmrFreeList;
	WXPTmrFreeList			=	ptmr;
	WXPTmrUsed--;
	WXPTmrFree++;
}



void WXPTmr_Init(void)
{
	uint16_t ix;
	uint16_t ix_next;
	WXP_TMR* ptmr1;
	WXP_TMR* ptmr2;

	WXP_MemClr((uint8_t *)&WXPTmrTbl[0],sizeof(WXPTmrTbl));//清空WXPTmrTbl
	WXP_MemClr((uint8_t *)&WXPTmrWheelTbl[0],sizeof(WXPTmrWheelTbl));//清空WXPTmrWheelTbl
	
	for(ix = 0; ix < (WXP_TMR_CFG_MAX - 1); ix++)
	{
		ix_next = ix + 1;
		ptmr1 = &WXPTmrTbl[ix];
		ptmr2 = &WXPTmrTbl[ix_next];
		ptmr1->WXPTmrType 	= WXP_TMR_TYPE;
		ptmr1->WXPTmrState 	= WXP_TMR_STATE_UNUSED;
		ptmr1->WXPTmrNext 	= (void *)ptmr2;
		ptmr1->WXPTmrName 	= (uint8_t *)(void *)"?";
	}
	//WXPTmrTbl[WXP_TMR_CFG_MAX]  init
	ptmr1  				= &WXPTmrTbl[ix];
	ptmr1->WXPTmrType	= WXP_TMR_TYPE;
	ptmr1->WXPTmrState  = WXP_TMR_STATE_UNUSED;
	ptmr1->WXPTmrNext	= (void *)0;
	ptmr1->WXPTmrName 	= (uint8_t *)(void *)"?";

	WXPTmrTime 		= 0;
	WXPTmrUsed 		= 0;
	WXPTmrFree 		= WXP_TMR_CFG_MAX;
	WXPTmrFreeList 	= &WXPTmrTbl[0];//空闲链表指向定时器池首地址
	
	
}



//插入一个链表到定时器轮
static void WXPTmr_Link(WXP_TMR* ptmr,uint8_t type)
{
	WXP_TMR *ptmr1;//定时器指针
	WXP_TMR_WHEEL *pspoke;//定时器辐条指针
	uint16_t spoke;

	ptmr->WXPTmrState = WXP_TMR_STATE_RUNNING;

	
	if(type == WXP_TMR_LINK_PERIODIC)//周期型
	{
		ptmr->WXPTmrMatch = ptmr->WXPTmrPeriod + WXPTmrTime;//定时器的匹配值=周期值+当前定时器计数值
	}
	else//单次型
	{
		if(ptmr->WXPTmrDly == 0u)//初值为0
		{
			ptmr->WXPTmrMatch = ptmr->WXPTmrPeriod + WXPTmrTime;//匹配值=周期+WXPTmrTime
		}
		else//初值不为0
		{
			ptmr->WXPTmrMatch = ptmr->WXPTmrDly + WXPTmrTime;//匹配值=初值+WXPTmrTime
		}
	}

	spoke  = (uint16_t)(ptmr->WXPTmrMatch % WXP_TMR_CFG_WHEEL_SIZE);//定时器轮插入的位置根据匹配值与定时器轮的余数而定
	pspoke = &WXPTmrWheelTbl[spoke];//获取要插入轮辐的位置

	if(pspoke->WXPTmrFirst == (WXP_TMR *)0)//定时器轮辐为空(定时器轮辐中所有定时器组均为空)
	{
		pspoke->WXPTmrFirst 	= ptmr;
		ptmr->WXPTmrNext		= (WXP_TMR *)0;
		pspoke->WXPTmrEntries 	= 1u;
	}
	else//定时器辐条不为空 即该辐条存在定时器	
	{
		ptmr1					= pspoke->WXPTmrFirst;//获取辐条头指针后的第一个定时器
		pspoke->WXPTmrFirst		= ptmr;//辐条头指针指向刚插入辐条的定时器
		ptmr->WXPTmrNext		= (void *)ptmr1;
		ptmr1->WXPTmrPrev		= (void *)ptmr;
		pspoke->WXPTmrEntries++;
	}
	ptmr->WXPTmrPrev = (void *)0;
}


//从定时器轮中移除一个定时器
static void WXPTmr_Unlink(WXP_TMR* ptmr)
{
	WXP_TMR			*ptmr1;
	WXP_TMR			*ptmr2;
	WXP_TMR_WHEEL 	*pspoke;//定时器辐条指针
	uint16_t 		spoke;

	spoke  = (uint16_t)(ptmr->WXPTmrMatch % WXP_TMR_CFG_WHEEL_SIZE);
	pspoke = &WXPTmrWheelTbl[spoke];//要移除定时器的辐条位置

	if(pspoke->WXPTmrFirst == ptmr)//移除的是该辐条的第一个定时器
	{
		ptmr1				= (WXP_TMR *)ptmr->WXPTmrNext;
		pspoke->WXPTmrFirst = (WXP_TMR *)ptmr1;
		if(ptmr1 != (WXP_TMR *)0)
		{
			ptmr1->WXPTmrPrev = (void *)0;
		}

	}
	else//非辐条第一个定时器
	{
		ptmr1				= (WXP_TMR *)ptmr->WXPTmrPrev;
		ptmr2				= (WXP_TMR *)ptmr->WXPTmrNext;
		ptmr1->WXPTmrNext 	= ptmr2;
		if(ptmr2 != (WXP_TMR *)0)
		{
			ptmr2->WXPTmrPrev = (void *)ptmr1;
		}
	}

	ptmr->WXPTmrState = WXP_TMR_STATE_STOPPED;
	ptmr->WXPTmrNext  = (void *)0;
	ptmr->WXPTmrPrev  = (void *)0;
	pspoke->WXPTmrEntries--;
	
}






//定时器任务 10ms/次
void WXPTmr_Task(void)
{
	WXP_TMR				*ptmr;
	WXP_TMR				*ptmr_next;
	WXP_TMR_CALLBACK 	pfnct;
	WXP_TMR_WHEEL		*pspoke;
	uint16_t 			spoke;


	WXPTmrTime++;
	spoke 	= (uint16_t)(WXPTmrTime % WXP_TMR_CFG_WHEEL_SIZE);
	pspoke 	= &WXPTmrWheelTbl[spoke];
	ptmr	= pspoke->WXPTmrFirst;
	while(ptmr != (WXP_TMR *)0)
	{
		ptmr_next = (WXP_TMR *)ptmr->WXPTmrNext;

		if(WXPTmrTime == ptmr->WXPTmrMatch)
		{
			WXPTmr_Unlink(ptmr);
			if(ptmr->WXPTmrOpt == WXP_TMR_OPT_PERIODIC)
			{
				WXPTmr_Link(ptmr,WXP_TMR_LINK_PERIODIC);
			}
			else
			{
				ptmr->WXPTmrState = WXP_TMR_STATE_COMPLETED;
			}
			pfnct = ptmr->WXPTmrCallback;
			if(pfnct != (WXP_TMR_CALLBACK)0)
			{
				(*pfnct)((void *)ptmr,ptmr->WXPTmrCallbackArg);//执行回调
			}
		}
		ptmr = ptmr_next;
	}
}






//清空表
void WXP_MemClr(uint8_t* pdest,uint16_t size)
{
	 while (size > 0) 
	 {
        *pdest++ = (uint8_t)0;
        size--;
    }
}







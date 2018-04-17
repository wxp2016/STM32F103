#include "app_softwaretimer.h"

uint16_t           			WXPTmrFree;                					//��ʱ�����еĿ�����Ŀ
uint16_t            		WXPTmrUsed;                					//��ʹ�õĶ�ʱ����Ŀ
uint32_t            		WXPTmrTime;                					//��ǰ����ֵ


WXP_TMR			         	WXPTmrTbl[WXP_TMR_CFG_MAX]; 				//��ʱ����
WXP_TMR*					WXPTmrFreeList;           					//ָ����ж�ʱ���б��ָ��
uint16_t           			WXPTmrTaskStk[WXP_TASK_TMR_STK_SIZE];		//��ʱ��������ջ��С
WXP_TMR_WHEEL      			WXPTmrWheelTbl[WXP_TMR_CFG_WHEEL_SIZE];		//��ʱ���ֱ�









//����һ����ʱ��
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




//ɾ��һ����ʱ��
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






//����һ����ʱ��
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



//ֹͣһ����ʱ��
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
						(*pfnct)((void *)ptmr,ptmr->WXPTmrCallbackArg);//ֹͣ��ʱ��֮ǰִ�лص� �þɵĲ���
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
						(*pfnct)((void *)ptmr,callback_arg);//ֹͣ��ʱ��֮ǰִ�лص����µĲ���
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




//�ӿ��ж�ʱ���б��з���һ����ʱ��
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



//�ͷ�һ����ʱ�������ж�ʱ������
static void	WXPTmr_Free(WXP_TMR* ptmr)
{
	ptmr->WXPTmrState 		=	WXP_TMR_STATE_UNUSED;			//״̬------>δʹ��
	ptmr->WXPTmrOpt			= 	WXP_TMR_OPT_NONE;				//����ѡ��-->None
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

	WXP_MemClr((uint8_t *)&WXPTmrTbl[0],sizeof(WXPTmrTbl));//���WXPTmrTbl
	WXP_MemClr((uint8_t *)&WXPTmrWheelTbl[0],sizeof(WXPTmrWheelTbl));//���WXPTmrWheelTbl
	
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
	WXPTmrFreeList 	= &WXPTmrTbl[0];//��������ָ��ʱ�����׵�ַ
	
	
}



//����һ��������ʱ����
static void WXPTmr_Link(WXP_TMR* ptmr,uint8_t type)
{
	WXP_TMR *ptmr1;//��ʱ��ָ��
	WXP_TMR_WHEEL *pspoke;//��ʱ������ָ��
	uint16_t spoke;

	ptmr->WXPTmrState = WXP_TMR_STATE_RUNNING;

	
	if(type == WXP_TMR_LINK_PERIODIC)//������
	{
		ptmr->WXPTmrMatch = ptmr->WXPTmrPeriod + WXPTmrTime;//��ʱ����ƥ��ֵ=����ֵ+��ǰ��ʱ������ֵ
	}
	else//������
	{
		if(ptmr->WXPTmrDly == 0u)//��ֵΪ0
		{
			ptmr->WXPTmrMatch = ptmr->WXPTmrPeriod + WXPTmrTime;//ƥ��ֵ=����+WXPTmrTime
		}
		else//��ֵ��Ϊ0
		{
			ptmr->WXPTmrMatch = ptmr->WXPTmrDly + WXPTmrTime;//ƥ��ֵ=��ֵ+WXPTmrTime
		}
	}

	spoke  = (uint16_t)(ptmr->WXPTmrMatch % WXP_TMR_CFG_WHEEL_SIZE);//��ʱ���ֲ����λ�ø���ƥ��ֵ�붨ʱ���ֵ���������
	pspoke = &WXPTmrWheelTbl[spoke];//��ȡҪ�����ַ���λ��

	if(pspoke->WXPTmrFirst == (WXP_TMR *)0)//��ʱ���ַ�Ϊ��(��ʱ���ַ������ж�ʱ�����Ϊ��)
	{
		pspoke->WXPTmrFirst 	= ptmr;
		ptmr->WXPTmrNext		= (WXP_TMR *)0;
		pspoke->WXPTmrEntries 	= 1u;
	}
	else//��ʱ��������Ϊ�� ���÷������ڶ�ʱ��	
	{
		ptmr1					= pspoke->WXPTmrFirst;//��ȡ����ͷָ���ĵ�һ����ʱ��
		pspoke->WXPTmrFirst		= ptmr;//����ͷָ��ָ��ղ�������Ķ�ʱ��
		ptmr->WXPTmrNext		= (void *)ptmr1;
		ptmr1->WXPTmrPrev		= (void *)ptmr;
		pspoke->WXPTmrEntries++;
	}
	ptmr->WXPTmrPrev = (void *)0;
}


//�Ӷ�ʱ�������Ƴ�һ����ʱ��
static void WXPTmr_Unlink(WXP_TMR* ptmr)
{
	WXP_TMR			*ptmr1;
	WXP_TMR			*ptmr2;
	WXP_TMR_WHEEL 	*pspoke;//��ʱ������ָ��
	uint16_t 		spoke;

	spoke  = (uint16_t)(ptmr->WXPTmrMatch % WXP_TMR_CFG_WHEEL_SIZE);
	pspoke = &WXPTmrWheelTbl[spoke];//Ҫ�Ƴ���ʱ���ķ���λ��

	if(pspoke->WXPTmrFirst == ptmr)//�Ƴ����Ǹ÷����ĵ�һ����ʱ��
	{
		ptmr1				= (WXP_TMR *)ptmr->WXPTmrNext;
		pspoke->WXPTmrFirst = (WXP_TMR *)ptmr1;
		if(ptmr1 != (WXP_TMR *)0)
		{
			ptmr1->WXPTmrPrev = (void *)0;
		}

	}
	else//�Ƿ�����һ����ʱ��
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






//��ʱ������ 10ms/��
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
				(*pfnct)((void *)ptmr,ptmr->WXPTmrCallbackArg);//ִ�лص�
			}
		}
		ptmr = ptmr_next;
	}
}






//��ձ�
void WXP_MemClr(uint8_t* pdest,uint16_t size)
{
	 while (size > 0) 
	 {
        *pdest++ = (uint8_t)0;
        size--;
    }
}







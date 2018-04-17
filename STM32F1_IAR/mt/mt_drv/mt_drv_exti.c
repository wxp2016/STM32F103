#include "mt_drv_exti.h"
#include "mt_hal_nvic.h"


extern uint8_t isr_flag;

//PA0-INT0
void EXTI0_IRQHandler(void)
{
    //printf("INT0.....\n");
    isr_flag = 1;
    EXTI_ClearITPendingBit(EXTI_Line0);
}

//PB1-INT1
void EXTI1_IRQHandler(void)
{
    printf("INT1.....\n");
    EXTI_ClearITPendingBit(EXTI_Line1);
}


//PC2-INT2
void EXTI2_IRQHandler(void)
{
    printf("INT2.....\n");
    EXTI_ClearITPendingBit(EXTI_Line2);
}

//PD3-INT3
void EXTI3_IRQHandler(void)
{
    printf("INT3.....\n");
    EXTI_ClearITPendingBit(EXTI_Line3);
}

//PE4-INT5
void EXTI4_IRQHandler(void)
{
    printf("INT4.....\n");
    EXTI_ClearITPendingBit(EXTI_Line4);
}




void ENABLE_EXTI0(void)
{
    EXTI->IMR |= 0x01;
}


void DISABLE_EXTI0(void)
{
    EXTI->IMR &= 0xFE;
}    


void ENABLE_EXTI1(void)
{
    EXTI->IMR |= 0x02;
}


void DISABLE_EXTI1(void)
{
    EXTI->IMR &= 0xFD;
}



void MT_EXTI_Init(void)
{
    MT_EXTI_PA0_Config();
    //MT_EXTI_PB1_Config();
    //MT_EXTI_PC2_Config();
    //MT_EXTI_PD3_Config();
    //MT_EXTI_PE4_Config();
}




//PA0
void MT_EXTI_PA0_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO,ENABLE);

	MT_NVIC_Config(MT_PRIORITY_0);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

//PB1
void MT_EXTI_PB1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO,ENABLE);

	MT_NVIC_Config(MT_PRIORITY_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}


void MT_EXTI_PC2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);

	MT_NVIC_Config(MT_PRIORITY_2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}


//PD3
void MT_EXTI_PD3_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD| RCC_APB2Periph_AFIO,ENABLE);

	MT_NVIC_Config(MT_PRIORITY_3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource3); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}
  

//PE4
void MT_EXTI_PE4_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO,ENABLE);

	MT_NVIC_Config(MT_PRIORITY_4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void HCSR_Init(void)
{
	//����GPIOA��TIM3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//��ʼ��trig����
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//��ʼ��echo����
	GPIO_InitTypeDef GPIO_InitStructe;
	GPIO_InitStructe.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructe.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructe.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructe);
	//����ʱ��Դ
	TIM_InternalClockConfig(TIM3);
	//����ʱ�ӻ�Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65536-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	//�������벶��PWMIģʽ
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0XF;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_PWMIConfig(TIM3,&TIM_ICInitStruct);
	//������ʱ�ж�
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);
	//����NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel =TIM3_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	//����ʱ��Դ
	TIM_Cmd(TIM3,ENABLE);
	
}
 
float count=0;
float distance=0;
 
//trig�ڷ����ź�
void HCSR_Start(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	Delay_us(15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}
 
//���ߵ�ƽ����ʱ��ת��Ϊ���룬����=340m / s��
float HCSR_GetDistance(void)
{
	distance = count / 10000 * 340 / 2;
	return distance;
}
//TIM3�ж�
void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1))
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
		TIM_SetCounter(TIM3,0);
		
	}
		if(TIM_GetITStatus(TIM3,TIM_IT_CC2))
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_CC2);
		count = TIM_GetCounter(TIM3);
	}
 
}

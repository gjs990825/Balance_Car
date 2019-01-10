#include "timer.h"
#include "buzzer.h"

vu16 currentMs1 = 0, currentMs2 = 0, currentMs3 = 0;
vu8 tim3_int_flag = 0;
vs16 currentSpeed = 0;

void TIM3_TimerInit(uint16_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE); //ʹ��TIMx����
}

#define DEBUG_PIN LED1

void TIM3_IRQHandler(void) //TIM3�ж�
{
	uint16_t speed;
	DEBUG_PIN = 0;
	tim3_int_flag = 1;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //�����־λ
		Angle_Calcu();
		currentSpeed = TIM4_EncoderRead() - TIM2_EncoderRead();			//�Ծ���΢�ֵõ��ٶ�
		speed = -(velocity(-currentSpeed) + balance(Angle, Angle_dot)); //�����ٶ�
		Motor_SpeedControl(speed, speed);								//���Ƶ�����
	}
	DEBUG_PIN = 0;
	currentMs1 += 12;
	currentMs2 += 12;
	currentMs3 += 12;
}

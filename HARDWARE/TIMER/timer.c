#include "timer.h"
#include "buzzer.h"

vu16 currentMs1 = 0, currentMs2 = 0, currentMs3 = 0;
vu8 tim3_int_flag = 0;
vs16 currentSpeed = 0;

void TIM3_TimerInit(uint16_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);							  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE); //使能TIMx外设
}

#define DEBUG_PIN LED1

void TIM3_IRQHandler(void) //TIM3中断
{
	uint16_t speed;
	DEBUG_PIN = 0;
	tim3_int_flag = 1;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除标志位
		Angle_Calcu();
		currentSpeed = TIM4_EncoderRead() - TIM2_EncoderRead();			//对距离微分得到速度
		speed = -(velocity(-currentSpeed) + balance(Angle, Angle_dot)); //计算速度
		Motor_SpeedControl(speed, speed);								//控制电机输出
	}
	DEBUG_PIN = 0;
	currentMs1 += 12;
	currentMs2 += 12;
	currentMs3 += 12;
}

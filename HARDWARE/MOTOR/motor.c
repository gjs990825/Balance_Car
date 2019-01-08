#include "motor.h"

#define TURN_SPEED_R 250	//Turnning speed set
#define TURN_SPEED_L 50

#define STEP_LENTH	5	//越小越平滑，但响应速度变慢

#define CORRECT	20


void Motor_Right(uint8_t dir)
{
	if(dir == GO)
	{
		MR_1 = 1;
		MR_2 = 0;
	}
	else if(dir == BACK)
	{
		MR_1 = 0;
		MR_2 = 1;
	}
	else if(dir == STOP)
	{
		MR_1 = 1;
		MR_2 = 1;
	}
}

void Motor_Left(uint8_t dir)
{
	if(dir == GO)
	{
		ML_1 = 1;
		ML_2 = 0;
	}
	else if(dir == BACK)
	{
		ML_1 = 0;
		ML_2 = 1;
	}
	else if(dir == STOP)
	{
		ML_1 = 1;
		ML_2 = 1;
	}
}

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_4 | GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	PWM_L = 1;
//	PWM_R = 1;
	
	MR_1 = 1;
	MR_2 = 0;
	ML_1 = 1;
	ML_2 = 0;
	
}

void Motor_SpeedControl(int16_t speed_l, int16_t speed_r)
{
	static int16_t Smooth_L = 0,Smooth_R = 0;
	static uint8_t LastCmd = 0x00;
	int16_t sl,sr;
	
	if(speed_l > 900)
		sl = 900;
	else if(speed_l < -900)
		sl = -900;
	else
		sl = speed_l;
	
	if(speed_r > 900)
		sr = 900;
	else if(speed_r < -900)
		sr = -900;
	else
		sr = speed_r;
	
	if(CMD == CMD_LEFT)//Excute left or right cmd
	{
		if(Smooth_L < TURN_SPEED_L)
		{
			Smooth_L += STEP_LENTH;
		}
		if(Smooth_R < TURN_SPEED_R)
		{
			Smooth_R += STEP_LENTH;
		}
		
		sr += Smooth_R;
		sl -= Smooth_L;
		
		LastCmd = CMD_LEFT;
	}
	else if(CMD == CMD_RIGHT)
	{
		if(Smooth_L < TURN_SPEED_L)
		{
			Smooth_L += STEP_LENTH;
		}
		if(Smooth_R < TURN_SPEED_R)
		{
			Smooth_R += STEP_LENTH;
		}
		
		sr -= Smooth_R;
		sl += Smooth_L;
		
		LastCmd = CMD_RIGHT;
	}
	else
	{
		if(Smooth_R + Smooth_L <= 0)
		{
			LastCmd = 0x00;
		}
		else
		{
			if(Smooth_L >= 0)
			{
				Smooth_L -= STEP_LENTH;
			}
			if(Smooth_R >= 0)
			{
				Smooth_R -= STEP_LENTH;
			}
			
			if(LastCmd == CMD_LEFT)
			{
				sr += Smooth_R;
				sl -= Smooth_L;
			}
			if(LastCmd == CMD_RIGHT)
			{
				sr -= Smooth_R;
				sl += Smooth_L;
			}
		}
		
		if(CMD == CMD_GO)//CORRECT error
		{
			sl += CORRECT;
		}
		
	}
	
	if(sl == 0)
	{
		Motor_Left(STOP);
		TIM_SetCompare1(TIM1, 900);
	}
	else
	{
		Motor_Left((sl > 0)?GO:BACK);
		TIM_SetCompare1(TIM1, (sl > 0)?sl:(-sl));
	}
	
	if(sr == 0)
	{
		Motor_Right(STOP);
		TIM_SetCompare4(TIM1, 900);
	}
	else
	{
		Motor_Right((sr > 0)?GO:BACK);
		TIM_SetCompare4(TIM1, (sr > 0)?sr:(-sr));
	}
}

void TIM1_PWMInit(uint16_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void TIM1_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);		
	}
}


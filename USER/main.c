//Includes
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "motor.h"
#include "encoder.h"
#include "mpu6050.h"
#include "kalman_filter.h"
#include "pid.h"



int main(void)
{
	//System initialize
	uart_init(115200);
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//Peripheral initialize
	LED_Init(); //LED initialize
	Motor_Init(); //Motor initialize
	TIM1_PWMInit(900, 0); //TIM1 PWM output initialize
	TIM2_EncoderInit(0xffff, 0); //TIM2 Encoder initialize(for right wheel)
	TIM4_EncoderInit(0xffff, 0); //TIM2 Encoder initialize(for left wheel)
	delay_ms(300);
	MPU6050_Init();
	
	TIM3_Int_Init(100 - 1, 7200 - 1); //10ms PID control
	
	while(1)
	{
		;
	}
}

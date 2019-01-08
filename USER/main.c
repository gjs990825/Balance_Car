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
#include "buzzer.h"
#include "nrf24l01.h"
#include "spi.h"


int main(void)
{
	char buf[6];
	uint8_t i = 0;
	const uint16_t reportTime = 1000;
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
	Buzzer_Init();
	Init();//SPI Init
	NRF_Init(MODEL_RX2, 80);
	
	printf("NRF Init %s!\r\n", (NRF_Check()) ? "OK" : "FAIL!");
	
	TIM3_TimerInit(100 - 1, 7200 - 1); //10ms PID control
	
	buzzer = 1;

	printf("System Init OK\r\n");
	while(1)
	{
		if (currentMs > reportTime) {
			NRF_TxPacket_AP((uint8_t *)"System running...", 18);
			printf("System running...\r\n");
			currentMs = 0;
		}
		if (rx_len != 0) {
			rx_len = 0;
			Data_Analize();
		}
	}
}

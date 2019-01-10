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
#include "Protocol.h"
#include "spi.h"
#include "iic.h"
#include "mpu6050.h"


int main(void)
{
	const uint16_t reportTime = 1000;
	//System initialize
	Uart_init(115200);
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//Peripheral initialize
	LED_Init();
	Motor_Init();
	TIM1_PWMInit(900, 0); //TIM1 PWM output initialize
	TIM2_EncoderInit(0xffff, 0); //TIM2 Encoder initialize(for right wheel)
	TIM4_EncoderInit(0xffff, 0); //TIM2 Encoder initialize(for left wheel)
	TIM3_TimerInit(100 - 1, 7200 - 1); //10ms PID control
	delay_ms(100);
	
	Buzzer_Init();
	Hard_SPIInit();//SPI initialize peripheral and gpio
	NRF_Init(MODEL_RX2, 80);//NRF24L01 module initialize set mode RX2
	
	printf("NRF Init %s!\r\n", (NRF_Check()) ? "OK" : "FAIL!");
	printf("System Init OK\r\n");
	
	TIM_Cmd(TIM3, ENABLE);// Timer3 enable

	I2C_Soft_Init();
	MPU6050_Init(125, 42);
	
	while(1)
	{
		LED = !LED;
		if (currentMs > reportTime) {
			NRF_TxPacket_AP((uint8_t *)"System running...", 18);
			printf("System running...\r\n");
			printf("angle = %7.4f, dot  = %7.4f\r\n", Angle, Angle_dot);
			currentMs = 0;
		}
		if (rx_len != 0) {
			rx_len = 0;
			Data_Analize();
		}
	}
}

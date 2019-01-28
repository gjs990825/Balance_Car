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
#include "hcsr04.h"
#include "string.h"
#include "task.h"

#define BUFFERSIZE 32

const uint16_t CheckDistanceTime = 50;
const uint16_t SendToRemoteTime = 500;
const uint16_t ReportToSerialTime = 1000;

int main(void)
{
	//System initialize
	Uart_init(115200);
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//Peripheral initialize
	LED_Init();
	Motor_Init();
	TIM1_PWMInit(900, 0);			   //TIM1 PWM output initialize
	TIM2_EncoderInit(0xffff, 0);	   //TIM2 Encoder initialize(for right wheel)
	TIM4_EncoderInit(0xffff, 0);	   //TIM2 Encoder initialize(for left wheel)
	TIM3_TimerInit(100 - 1, 7200 - 1); //10ms PID control
	delay_ms(100);

//	Buzzer_Init();
	Hard_SPIInit();			 //SPI initialize peripheral and GPIO
	NRF_Init(MODEL_RX2, 80); //NRF24L01 module initialize set mode RX2

	printf("NRF Init %s!\r\n", (NRF_Check()) ? "OK" : "FAIL!");
	printf("System Init OK\r\n");

	TIM_Cmd(TIM3, ENABLE); // Timer3 enable

	I2C_Soft_Init();
	MPU6050_Init(); //Initialize mpu6050, sampling rate 125Hz

	HCSR04_Init(0, 150); //Initialize HC-SR04 and set distance range from 0 to 150 cm

	while (1)
	{
		LED = !LED;
		if (currentMs1 > CheckDistanceTime)
		{
			Detect_Distance();
			currentMs1 = 0;
		}
		if (currentMs2 > SendToRemoteTime)
		{
			Send_ToRemote();
			currentMs2 = 0;
		}
		if (currentMs3 > ReportToSerialTime)
		{
			Report_ToSerial();
			currentMs3 = 0;
		}
		if (rx_len != 0) //Check NRF24L01 rx data
		{
			rx_len = 0;
			Data_Analize();
		}
	}
}

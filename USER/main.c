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

#define BUFFERSIZE 32

int main(void)
{
	const uint16_t reportTime = 400;
	char buf[BUFFERSIZE] = {0};
	int distance = 0;
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
	MPU6050_Init(125, 42);//Set mpu6050 sampling rate to 125

	HCSR04_Init(0, 400);//Initialize HC-SR04 and set distance range from 0 to 40 cm
	
	while(1)
	{
		LED = !LED;
		if (currentMs > reportTime) {
			distance = HCSR04_distanceInMillimeters();
			if (distance > 0) { //If no interrupt or out range
				sprintf(buf, "Distance = %2d", distance);
				NRF_TxPacket_AP((uint8_t *)buf, strlen(buf));
				memset(buf, 0, sizeof(buf[0]) * BUFFERSIZE); //clear buf
			}
			printf("System running...\r\n"); //serial print
			currentMs = 0;
			// currentMs -= reportTime; //for missing time
		}
		if (rx_len != 0) {
			rx_len = 0;
			Data_Analize();
		}
	}
}

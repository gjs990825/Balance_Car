#include "HCSR04.h"
#include "nrf24l01.h"
#include "timer.h"

#define ECHO_TIMEOUT 65536u

int _minRange = -1;
int _maxRange = -1;

void Soft_delay_us(u32 nTimer)
{
	u32 i=0;
	for(i=0;i<nTimer;i++){
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

void Soft_delay_ms(u32 nTimer)
{
	u32 i=1000*nTimer;
	Soft_delay_us(i);
}

void HCSR04_Init(int minRange, int maxRange)//init
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_HCSR04, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = HCSR04_PIN_TRIGGER;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_HCSR04, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = HCSR04_PIN_ECHO;
    GPIO_Init(GPIO_HCSR04, &GPIO_InitStructure);

    GPIO_ResetBits(GPIO_HCSR04, HCSR04_PIN_ECHO | HCSR04_PIN_TRIGGER);
    
	_minRange = minRange;
    _maxRange = maxRange;
}

unsigned int HCSR04_echoInMicroseconds(void)
{
    uint32_t waitT = 0;
    TRIGGER_L();
    Soft_delay_us(5);
    TRIGGER_H();
    Soft_delay_us(10);
    TRIGGER_L();  
    
    while(ECHO_READ() == 0){
        Soft_delay_us(1);
        waitT++;
        if(waitT > 0xFF) return 0;
    }

    waitT = 0;

    while(ECHO_READ() == 1) {
        Soft_delay_us(1);
        waitT++;
        if(waitT > ECHO_TIMEOUT) return 0;
    }

    return waitT;
}

int HCSR04_distanceInMillimeters(void)
{
	int duration = 0, distance = 0;
	
    tim3_int_flag = 0;
    nrf_int_flag = 0;
    duration = HCSR04_echoInMicroseconds();
    
    if (tim3_int_flag || nrf_int_flag) {
        return HCSR04_ERROR;
    }
    
    if (duration == 0) {
        return HCSR04_TIMEOUT;
    }
    
    // Given the speed of sound in air is 332m/s = 0.0332cm/us).
    distance = duration * 0.0332;
	
	if (_minRange == -1 && _maxRange == -1)
	{
		return distance;
	}
	
	if (distance > _minRange && distance < _maxRange)
	{
		return distance;
	}	
	
	return HCSR04_OUTRANGE;
}

#include "HCSR04.h"
#include "nrf24l01.h"
#include "timer.h"
#include "delay.h"

//限制时间提升检测成功率
#define ECHO_TIMEOUT 1920u // 60cm * 32us/cm

int _minRange = -1;
int _maxRange = -1;


#if defined(USE_SOFT_DELAY)

void HCSR04_delay_us(u32 nTimer)
{
    u32 i = 0;
    for (i = 0; i < nTimer; i++)
    {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    }
}

void HCSR04_delay_ms(u32 nTimer)
{
    u32 i = 1000 * nTimer;
    HCSR04_delay_us(i);
}

#else

#define HCSR04_delay_us(us) delay_us(us)
#define HCSR04_delay_ms(ms) delay_ms(ms)

#endif // USE_SOFT_DELAY




void HCSR04_Init(int minRange, int maxRange)
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

int HCSR04_echoInMicroseconds(void)
{
    int waitT = 0;
    TRIGGER_L();
    HCSR04_delay_us(5);
    TRIGGER_H();
    HCSR04_delay_us(10);
    TRIGGER_L();

    while (ECHO_READ() == 0)
    {
        HCSR04_delay_us(1);
        waitT++;
        if (waitT > 0xFF)
            return HCSR04_ERROR;
    }

    waitT = 0;

    while (ECHO_READ() == 1)
    {
        HCSR04_delay_us(1);
        waitT++;
        if (waitT > ECHO_TIMEOUT)
            return HCSR04_TIMEOUT;
    }

    return waitT;
}

int HCSR04_distanceInMillimeters(void)
{
    int duration = 0, distance = 0;

    tim3_int_flag = 0;
    nrf_int_flag = 0;
    duration = HCSR04_echoInMicroseconds();
    //printf("duration = %d\r\n", duration); //调试用

    if (tim3_int_flag || nrf_int_flag)
    {
        return HCSR04_INTFAULT;
    }
    if (duration == HCSR04_ERROR)
    {
        return HCSR04_ERROR;
    }
    if (duration == HCSR04_TIMEOUT)
    {
        return HCSR04_TIMEOUT;
    }

    // Given the speed of sound in air is 332m/s = 0.0332cm/us).
    distance = duration * 0.0332;

    if (distance > _minRange && distance < _maxRange)
    {
        return distance;
    }
    else
    {
        return HCSR04_OUTRANGE;
    }
}

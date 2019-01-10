#ifndef HCSR04_h
#define HCSR04_h

#include "sys.h"

#define GPIO_HCSR04	        GPIOB
#define HCSR04_PIN_ECHO		GPIO_Pin_13
#define HCSR04_PIN_TRIGGER	GPIO_Pin_12
#define RCC_HCSR04	        RCC_APB2Periph_GPIOB

#define TRIGGER_H() GPIO_SetBits(GPIO_HCSR04, HCSR04_PIN_TRIGGER)
#define TRIGGER_L() GPIO_ResetBits(GPIO_HCSR04, HCSR04_PIN_TRIGGER)
#define ECHO_READ() GPIO_ReadInputDataBit(GPIO_HCSR04, HCSR04_PIN_ECHO)

void Soft_delay_us(u32 nTimer);
void Soft_delay_ms(u32 nTimer);

void HCSR04_Init(int minRange, int maxRange);
int HCSR04_echoInMicroseconds(void);
int HCSR04_distanceInMillimeters(void);

extern int _maxRange;

typedef enum
{
    HCSR04_INTFAULT = -4,
    HCSR04_ERROR = -3,
    HCSR04_TIMEOUT = -2,
    HCSR04_OUTRANGE = -1
} HCSR04_Status_TypeDef;
//HCSR04 ×´Ì¬¶¨Òå

#endif

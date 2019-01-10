#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "led.h"
#include "usart.h"
#include "encoder.h"
#include "pid.h"

extern vu16 currentMs1, currentMs2, currentMs3;
extern vs16 currentSpeed;
extern vu8 tim3_int_flag;

void TIM3_TimerInit(uint16_t arr, uint16_t psc);

#endif

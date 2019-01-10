#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "led.h"
#include "usart.h"
#include "encoder.h"
#include "pid.h"

extern vu16 currentMs;
extern vu8 tim3_int_flag;

void TIM3_TimerInit(u16 arr,u16 psc);

#endif

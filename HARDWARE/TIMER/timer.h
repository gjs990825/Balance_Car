#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "led.h"
#include "usart.h"
#include "encoder.h"
#include "pid.h"

void TIM3_Int_Init(u16 arr,u16 psc);

#endif

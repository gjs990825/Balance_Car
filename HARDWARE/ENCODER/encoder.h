#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "sys.h"

//TIM4 Left wheel
//TIM2 Right wheel

void TIM4_EncoderInit(uint16_t arr, uint16_t psc);
void TIM2_EncoderInit(uint16_t arr, uint16_t psc);

int16_t TIM4_EncoderRead(void);
int16_t TIM2_EncoderRead(void);

#endif

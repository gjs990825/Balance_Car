#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "sys.h"

//TIM4 Left wheel
//TIM2 Right wheel

void TIM4_EncoderInit(uint16_t arr, uint16_t psc);
void TIM2_EncoderInit(uint16_t arr, uint16_t psc);

void TIM4_EncoderWrite(int data);
int TIM4_EncoderRead(void);

void TIM2_EncoderWrite(int data);
int TIM2_EncoderRead(void);



#endif

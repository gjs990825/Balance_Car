#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "sys.h"
#include "usart.h"

#define ML_1 PBout(14)//A15
#define ML_2 PAout(12)
#define MR_1 PBout(15)//B3
#define MR_2 PBout(4)

#define STOP	0
#define GO		1
#define BACK	2


//#define PWM_L PAout(8)
//#define PWM_R PAout(11)


void TIM1_PWMInit(uint16_t arr, uint16_t psc);

void Motor_Init(void);
void Motor_SpeedControl(int16_t speed_l, int16_t speed_r);
void Motor_Right(uint8_t dir);
void Motor_Left(uint8_t dir);

#endif

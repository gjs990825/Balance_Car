#ifndef _PID_H_
#define _PID_H_

#include "sys.h"
#include "motor.h"
#include "encoder.h"
#include "kalman_filter.h"

void PID(float setPoint);
void PID_balance(float vertical);
int Incremental_PI(int Encoder,int Target);
int Position_PI (int Encoder,int Target);
int balance(float Angle, float gy);
uint16_t Position_Con(uint16_t speed);
int velocity(int16_t Speed_left, int16_t Speed_right);

#endif

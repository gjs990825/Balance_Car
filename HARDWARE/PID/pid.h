#ifndef _PID_H_
#define _PID_H_

#include "sys.h"
#include "motor.h"
#include "encoder.h"
#include "kalman_filter.h"


int balance(float Angle, float gy);
int velocity(int16_t curSpeed);

#endif

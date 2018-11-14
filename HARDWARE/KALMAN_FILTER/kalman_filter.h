#ifndef _KALMAN_FILTER_H_
#define _KALMAN_FILTER_H_

#include "sys.h"
#include "mpu6050.h"
#include "math.h"

#define PI 3.14159


//角度参数
extern float Angle, Angle_dot;   //小车最终倾斜角度、角速度
extern float Angle_aYZ;          //由Y轴Z轴上的加速度传感器测得的数值，计算出倾斜角度
extern float Angle_gX;           //由X轴的陀螺仪传感器测得的数值，计算出角速度
//卡尔曼参数		
extern float  Q_angle;            
extern float  Q_gyro;              
extern float  R_angle;	
extern float  dt;	       //dt为kalman滤波器采样时间;
extern char   C_0;
extern float  Q_bias, Angle_err;
extern float  PCt_0, PCt_1, E;
extern float  K_0, K_1, t_0, t_1;
extern float  Pdot[4];
extern float  PP[2][2];


void Kalman_Filter(float Accel,float Gyro);
void Angle_Calcu(void);


#endif

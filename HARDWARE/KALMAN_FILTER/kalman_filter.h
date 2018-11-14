#ifndef _KALMAN_FILTER_H_
#define _KALMAN_FILTER_H_

#include "sys.h"
#include "mpu6050.h"
#include "math.h"

#define PI 3.14159


//�ǶȲ���
extern float Angle, Angle_dot;   //С��������б�Ƕȡ����ٶ�
extern float Angle_aYZ;          //��Y��Z���ϵļ��ٶȴ�������õ���ֵ���������б�Ƕ�
extern float Angle_gX;           //��X��������Ǵ�������õ���ֵ����������ٶ�
//����������		
extern float  Q_angle;            
extern float  Q_gyro;              
extern float  R_angle;	
extern float  dt;	       //dtΪkalman�˲�������ʱ��;
extern char   C_0;
extern float  Q_bias, Angle_err;
extern float  PCt_0, PCt_1, E;
extern float  K_0, K_1, t_0, t_1;
extern float  Pdot[4];
extern float  PP[2][2];


void Kalman_Filter(float Accel,float Gyro);
void Angle_Calcu(void);


#endif

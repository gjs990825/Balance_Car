#include "kalman_filter.h"


//角度参数
float Angle, Angle_dot;   //小车最终倾斜角度、角速度
float Angle_aYZ;          //由Y轴Z轴上的加速度传感器测得的数值，计算出倾斜角度
float Angle_gX;           //由X轴的陀螺仪传感器测得的数值，计算出角速度
//卡尔曼参数		
float  Q_angle=0.002;            
float  Q_gyro=0.01;              
float  R_angle=0.003;	
float  dt=0.005;	       //dt为kalman滤波器采样时间;
char   C_0 = 1;
float  Q_bias, Angle_err;
float  PCt_0, PCt_1, E;
float  K_0, K_1, t_0, t_1;
float  Pdot[4] ={0,0,0,0};
float  PP[2][2] = { { 1, 0 },{ 0, 1 } };
// 卡尔曼滤波
void Kalman_Filter(float Accel,float Gyro)
{
	Angle+=(Gyro - Q_bias) * dt; //先验估计

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	Angle_dot = Gyro - Q_bias; //输出值(后验估计)的微分=角速度
}
//倾角计算（卡尔曼融合）
void Angle_Calcu(void)
{
	//------根据加速度分量测得角速度--------------------------
    //不自测，加速度传感器范围设置  0  ±2g	 16384 LSB/g
	Angle_aYZ = atan2((Out_Data(ACCEL_YOUT_H)), (Out_Data(ACCEL_ZOUT_H))) *180/PI ;
	//Angle_aYZ = atan2((Out_Data(ACCEL_YOUT_H) - 300), (Out_Data(ACCEL_ZOUT_H)-(16384-16450))) *180/PI ;   //去除零点偏移,计算得到角度（弧度）
	//弧度转换为度,

    //-------角速度-------------------------
	//不自测，陀螺仪测量范围设置	 0  ±250°/s  131LSB/(°/s)	0.00763358 (°/s)/LSB
    Angle_gX = (Out_Data(GYRO_XOUT_H)-0) *0.00763358 ;//0为补偿量，在静止是测得的角速度为0LSB；
		
	//-------卡尔曼滤波融合-----------------------
	Kalman_Filter(Angle_aYZ - 0, Angle_gX - 0);       //卡尔曼滤波计算倾角,减去零点偏移，
}


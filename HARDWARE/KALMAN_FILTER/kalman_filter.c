#include "kalman_filter.h"


//�ǶȲ���
float Angle, Angle_dot;   //С��������б�Ƕȡ����ٶ�
float Angle_aYZ;          //��Y��Z���ϵļ��ٶȴ�������õ���ֵ���������б�Ƕ�
float Angle_gX;           //��X��������Ǵ�������õ���ֵ����������ٶ�
//����������		
float  Q_angle=0.002;            
float  Q_gyro=0.01;              
float  R_angle=0.003;	
float  dt=0.005;	       //dtΪkalman�˲�������ʱ��;
char   C_0 = 1;
float  Q_bias, Angle_err;
float  PCt_0, PCt_1, E;
float  K_0, K_1, t_0, t_1;
float  Pdot[4] ={0,0,0,0};
float  PP[2][2] = { { 1, 0 },{ 0, 1 } };
// �������˲�
void Kalman_Filter(float Accel,float Gyro)
{
	Angle+=(Gyro - Q_bias) * dt; //�������

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	Angle_dot = Gyro - Q_bias; //���ֵ(�������)��΢��=���ٶ�
}
//��Ǽ��㣨�������ںϣ�
void Angle_Calcu(void)
{
	//------���ݼ��ٶȷ�����ý��ٶ�--------------------------
    //���Բ⣬���ٶȴ�������Χ����  0  ��2g	 16384 LSB/g
	Angle_aYZ = atan2((Out_Data(ACCEL_YOUT_H)), (Out_Data(ACCEL_ZOUT_H))) *180/PI ;
	//Angle_aYZ = atan2((Out_Data(ACCEL_YOUT_H) - 300), (Out_Data(ACCEL_ZOUT_H)-(16384-16450))) *180/PI ;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
	//����ת��Ϊ��,

    //-------���ٶ�-------------------------
	//���Բ⣬�����ǲ�����Χ����	 0  ��250��/s  131LSB/(��/s)	0.00763358 (��/s)/LSB
    Angle_gX = (Out_Data(GYRO_XOUT_H)-0) *0.00763358 ;//0Ϊ���������ھ�ֹ�ǲ�õĽ��ٶ�Ϊ0LSB��
		
	//-------�������˲��ں�-----------------------
	Kalman_Filter(Angle_aYZ - 0, Angle_gX - 0);       //�������˲��������,��ȥ���ƫ�ƣ�
}


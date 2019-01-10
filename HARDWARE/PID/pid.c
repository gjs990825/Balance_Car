#include "pid.h"
#include "Protocol.h"
#include "timer.h"
#include "task.h"
#include "buzzer.h"

//����ң�ص��н��ٶ�
const int Movement_S = 130;

//��ʼ�����ٶȵľ���
const uint16_t DistanceThreshold = 50;

//�ٶȻ��Ĳ���
int PWM = 0;
float Speed_r_l = 0, Speed = 0, Position = 0;
int16_t Movement = 0; //Ŀ���ٶ�

/***************************************************************************
 *   ֱ����PD����(����΢��)       ���ڸ�����ҪѸ��������Ӧ��ҪD(΢�ֿ���)
 *   �Ƕȣ����ٶ�                 �ǶȻ�����С��ֱ��
 ***************************************************************************/
int balance(float Angle, float gy)
{
	//	float Bias, Kp = 75, Kd = 15;
	float Bias, Kp = 75, Kd = 15;
	int balance; //ֱ��PWM�ķ���ֵ

	if (Angle < -40 || Angle > 40)
	{
		return 0;
	}

	Bias = Angle + 2.54;		   //ֱ����ƫ��
	balance = Kp * Bias + gy * Kd; //����ֱ����PWM

	return balance;
}

/***************************************************************************
 *   �ٶ�PI����(��������)      PI ��������һ�����Կ������������ݸ���ֵ��ʵ�����ֵ���ɿ���ƫ
                               ���ƫ��ı�����P���ͻ��֣�I��ͨ��������Ϲ��ɿ��������Ա���
                               ������п��ơ�
 *   ���ұ�������ֵ            
 ***************************************************************************/
int velocity(int16_t curSpeed) //ƽ��С���Ŀ���ϵͳ��������--��С����һ�����ٶ�
{							   //���е�ʱ������Ҫ��С��ͣ������С����Ҫ��ʻ������ٶ�ȥ ��׷��
	float Kp = 13, Ki = 0.081;

	if (CMD == CMD_GO) //����ָ��ִ��
	{
		Movement = Movement_S;
	}
	else if (CMD == CMD_BACK)
	{
		Movement = -Movement_S;
	}
	else
	{
		Movement = 0;
	}

	if (Yoffset != 0) //ң��ָ��ִ��
	{
		if ((currentSpeed + 10 > currentDistance) && currentDistance < DistanceThreshold) //�����ٶȣ��ٽ��ٶ�10
		{
			Movement += (Yoffset > 0) ? -(currentSpeed + DistanceThreshold - currentDistance) : Yoffset;
			Buzzer_Set(1);
		}
		else
		{
			Movement += Yoffset;
			Buzzer_Set(0);
		}
	}

	Speed_r_l = curSpeed;

	Speed = Speed * 0.8;			 //��ͨ�˲���Ŀ����Ϊ�˼����ٶ�ֵ�ı仯����ֹ�ٶȿ��ƶ�ֱ����ɸ��ţ�
	Speed = Speed + Speed_r_l * 0.2; //��Ϊƽ��С��ϵͳ���棬ֱ����������Ҫ�ģ��������ƶ���ֱ����˵����һ�ָ���

	Position += Speed; //ƫ����ֵõ�С����λ��

	Position += Movement;

	if (Position > 100000)
		Position = 100000; //===�����޷�
	if (Position < -100000)
		Position = -100000; //===�����޷�

	PWM = Kp * Speed + Ki * Position; //�ٶȺ�λ��

	if (Angle < -40 || Angle > 40) //�Ƕȹ��󣬹رյ�����������㣬ָ�λ
	{
		Position = 0;
		CMD = 0x00;
		return 0;
	}

	return PWM;
}

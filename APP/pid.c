#include "pid.h"
#include "Protocol.h"
#include "timer.h"
#include "task.h"
#include "buzzer.h"

//蓝牙遥控的行进速度
const int16_t Movement_S = 120;

//开始限制速度的距离
const uint8_t DistanceThreshold = 45;
const uint8_t dodgeDistance = 20;

//速度环的参数
int PWM = 0;
float Speed_r_l = 0, Speed = 0, Position = 0;
int16_t Movement = 0; //目标速度

/***************************************************************************
 *   直立的PD控制(比例微分)       对于干扰需要迅速做出反应需要D(微分控制)
 *   角度，角速度                 角度环控制小车直立
 ***************************************************************************/
int balance(float Angle, float gy)
{
	//	float Bias, Kp = 75, Kd = 15;
	float Bias, Kp = 75, Kd = 15;
	int balance; //直立PWM的返回值

	if (Angle < -40 || Angle > 40)
	{
		return 0;
	}

	Bias = Angle + 2.54;		   //直立的偏差
	balance = Kp * Bias + gy * Kd; //计算直立的PWM

	return balance;
}

/*
 PI 调节器是一种线性控制器，它根据给定值与实际输出值构成控制偏
 差，将偏差的比例（P）和积分（I）通过线性组合构成控制量，对被控
 对象进行控制。
 */
//速度PI控制(比例积分)  //参数：当前速度
int velocity(int16_t curSpeed) //平衡小车的控制系统是正反馈
{
	float Kp = 13, Ki = 0.081;

	if (CMD == CMD_GO) //蓝牙指令执行
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

	if (Yoffset != 0) //遥控指令执行
	{
		if ((currentSpeed + 10 > currentDistance) && currentDistance < DistanceThreshold) //限制速度，临界速度10
		{
			Movement += (Yoffset > -10) ? -(currentSpeed + DistanceThreshold - currentDistance) : Yoffset;
			Buzzer_Set(1);
		}
		else
		{
			Movement += Yoffset;
			Buzzer_Set(0);
		}
	}

	if (currentDistance < dodgeDistance) //自行远离障碍物
	{
		Movement -= 10;
	}

	Speed_r_l = curSpeed;

	Speed = Speed * 0.8;			 //低通滤波，目的是为了减缓速度值的变化，防止速度控制对直立造成干扰，
	Speed = Speed + Speed_r_l * 0.2; //因为平衡小车系统里面，直立控制是主要的，其他控制对于直立来说都是一种干扰

	Position += Speed; //偏差积分得到小车的位移

	Position += Movement;

	if (Position > 100000)
		Position = 100000; //===积分限幅
	if (Position < -100000)
		Position = -100000; //===积分限幅

	PWM = Kp * Speed + Ki * Position; //速度和位置

	if (Angle < -40 || Angle > 40) //角度过大，关闭电机，积分清零，指令复位
	{
		Position = 0;
		CMD = 0x00;
		return 0;
	}

	return PWM;
}

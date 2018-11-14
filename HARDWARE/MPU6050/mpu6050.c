#include "mpu6050.h"


void MPU6050_Init(void)
{
	IIC_Init();
	MPU6050_WriteOneByte(PWR_MGMT_1, 0x00); //Wakeup
	MPU6050_WriteOneByte(SMPLRT_DIV, 0x07); //Sampling Rate 125HZ
	MPU6050_WriteOneByte(CONFIG, 0x06); //config
	MPU6050_WriteOneByte(ACCEL_CONFIG, 0x00); //No self checking, sccelerometer range set 0  ±2g	 16384 LSB/g
//	MPU6050_WriteOneByte(GYRO_CONFIG, 0x00); //不自测，陀螺仪测量范围设置	 0  ±250°/s  131LSB/(°/s)
	MPU6050_WriteOneByte(GYRO_CONFIG, 0x08); //不自测，陀螺仪测量范围设置	 1  ±500°/s  65.5LSB/(°/s)
}

//Write assigned register data
void MPU6050_WriteOneByte(uint8_t REG_Adress, uint8_t REG_Data)
{
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);
	IIC_Wait_Ack();
	IIC_Send_Byte(REG_Adress);
	IIC_Wait_Ack();
	IIC_Send_Byte(REG_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

uint8_t MPU6050_ReadOneByte(u8 REG_Address)
{
  uint8_t REG_data = 0;
	IIC_Start();				  //开始信号
	IIC_Send_Byte(SlaveAddress);  //发送设备地址+写信号 
	IIC_Wait_Ack();
	IIC_Send_Byte(REG_Address);   //发送寄存器地址
	IIC_Wait_Ack();

	IIC_Start();				  //开始信号
	IIC_Send_Byte(SlaveAddress + 1);  //发送设备地址+读信号 
	IIC_Wait_Ack();
	REG_data = IIC_Read_Byte(0);   //读完一个字节后，发送NACK
	IIC_Stop();                   //产生一个停止条件
	return REG_data;
}

//数据合成
int16_t Out_Data(u8 REG_Address)		  //返回值，有符号整型16位
{
  uint8_t H, L;
	H = MPU6050_ReadOneByte(REG_Address);
	L = MPU6050_ReadOneByte(REG_Address + 1);
	return (H<<8) + L;
}

//ACCEL 加速度
//Out_Data(ACCEL_XOUT_H),
// Out_Data(ACCEL_YOUT_H), 
//Out_Data(ACCEL_ZOUT_H);

//GYRO 角速度
//Out_Data(GYRO_XOUT_H),
//Out_Data(GYRO_YOUT_H), 
//Out_Data(GYRO_ZOUT_H);

//TEMP 温度
//Out_Data(TEMP_OUT_H)



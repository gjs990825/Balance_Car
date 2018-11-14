#include "mpu6050.h"


void MPU6050_Init(void)
{
	IIC_Init();
	MPU6050_WriteOneByte(PWR_MGMT_1, 0x00); //Wakeup
	MPU6050_WriteOneByte(SMPLRT_DIV, 0x07); //Sampling Rate 125HZ
	MPU6050_WriteOneByte(CONFIG, 0x06); //config
	MPU6050_WriteOneByte(ACCEL_CONFIG, 0x00); //No self checking, sccelerometer range set 0  ��2g	 16384 LSB/g
//	MPU6050_WriteOneByte(GYRO_CONFIG, 0x00); //���Բ⣬�����ǲ�����Χ����	 0  ��250��/s  131LSB/(��/s)
	MPU6050_WriteOneByte(GYRO_CONFIG, 0x08); //���Բ⣬�����ǲ�����Χ����	 1  ��500��/s  65.5LSB/(��/s)
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
	IIC_Start();				  //��ʼ�ź�
	IIC_Send_Byte(SlaveAddress);  //�����豸��ַ+д�ź� 
	IIC_Wait_Ack();
	IIC_Send_Byte(REG_Address);   //���ͼĴ�����ַ
	IIC_Wait_Ack();

	IIC_Start();				  //��ʼ�ź�
	IIC_Send_Byte(SlaveAddress + 1);  //�����豸��ַ+���ź� 
	IIC_Wait_Ack();
	REG_data = IIC_Read_Byte(0);   //����һ���ֽں󣬷���NACK
	IIC_Stop();                   //����һ��ֹͣ����
	return REG_data;
}

//���ݺϳ�
int16_t Out_Data(u8 REG_Address)		  //����ֵ���з�������16λ
{
  uint8_t H, L;
	H = MPU6050_ReadOneByte(REG_Address);
	L = MPU6050_ReadOneByte(REG_Address + 1);
	return (H<<8) + L;
}

//ACCEL ���ٶ�
//Out_Data(ACCEL_XOUT_H),
// Out_Data(ACCEL_YOUT_H), 
//Out_Data(ACCEL_ZOUT_H);

//GYRO ���ٶ�
//Out_Data(GYRO_XOUT_H),
//Out_Data(GYRO_YOUT_H), 
//Out_Data(GYRO_ZOUT_H);

//TEMP �¶�
//Out_Data(TEMP_OUT_H)



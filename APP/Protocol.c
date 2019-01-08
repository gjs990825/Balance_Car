#include "Protocol.h"
#include "nrf24l01.h"
#include "stdio.h"
#include "motor.h"
#include "usart.h"

int16_t Xoffset = 0, Yoffset = 0;
const uint16_t threshold = 100;
const uint8_t X_scale = 12, Y_scale = 22;

uint8_t Get_Checksum(uint8_t mydata[])
{
	uint8_t i;
    uint8_t checksum=0;
	uint8_t length = mydata[3];

    for(i = 0; i < length + 2; i++)
    {
        checksum ^= (mydata[3+i]&0xFF);
    }
    return checksum;
}

void RC_SendBack(uint8_t direction)
{
	uint8_t	data[DATA_LENTH] = {0};
	
	//��ͷ
	data[0] = '$';
	data[1] = 'M';
	
	//���ݷ�������
	data[2] = direction;

	//���ݳ���
	data[3] = 9;

	//����֡
	data[4] = MSP_CONTROL;

	
	if(direction == '<')//����ң��
	{
		//У��
		data[15] = Get_Checksum(data);
		NRF_TxPacket_AP(data, 16);
	}
	else if(direction == '>')//������λ��
	{
		printf("NO DATA\r\n");
	}
}

void Data_Analize(void)
{
	int16_t X, Y;
	if (Get_Checksum(RxBuf) != RxBuf[15]) {
		printf("Data is corruped!\r\n");
		return;
	}
	
	if (RxBuf[0] != '$') return;
	if (RxBuf[1] != 'M') return;
	if (RxBuf[2] != '<') return;

	X = (int16_t)((RxBuf[5] << 8)|(RxBuf[6])) - 2048;
	Y = (int16_t)((RxBuf[7] << 8)|(RxBuf[8])) - 2048;
	
	if ((X < -threshold)||(X > threshold)) {
		Xoffset = X / X_scale;
	}
	else {
		Xoffset = 0;
	}
	
	if ((Y < -threshold)||(Y > threshold)) {
		Yoffset = Y / Y_scale;
	}
	else {
		Yoffset = 0;
	}
	
	printf("x = %d, y = %d\r\n", Xoffset, Yoffset);
	printf("CMD = %d\r\n", CMD);
}





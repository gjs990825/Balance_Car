#include "task.h"
#include "hcsr04.h"
#include "stdio.h"
#include "string.h"
#include "nrf24l01.h"
#include "timer.h"

uint16_t currentDistance = 0; //��ǰ�ϰ������

const uint8_t maxIntErrorTimes = 15;    //��������жϴ������
const uint8_t maxErrorTimes = 6;        //�������������
const uint8_t numOfSampling = 3;        //��������

// �������� = numOfSampling
void Detect_Distance(void)
{
    int distance = 0, tmp = 0;
    uint8_t times = numOfSampling;
    uint8_t errortimes = 0, int_errortimes = 0;

    while (times)
    {
        tmp = HCSR04_distanceInMillimeters();
        //printf("d = %d\r\n", tmp); //�����ã����HCSR04_distanceInMillimeters()����ֵ
        if (tmp > 0)
        {
            distance += tmp;
            times--;
        }
        else if((tmp == HCSR04_OUTRANGE) || (tmp == HCSR04_TIMEOUT))
        {
            distance += _maxRange;
            times--;
        }
        else if (tmp == HCSR04_INTFAULT)
        {
            int_errortimes++;
            if (int_errortimes >= maxIntErrorTimes)
            {
                printf("Too many Interrupt!\r\n"); //���ڱ���
                return;
            }
        }
        else
        {
            errortimes++;
            if (errortimes >= maxErrorTimes)
            {
                printf("Detect_Distance FAIL!\r\n"); //���ڱ���
                return;
            }
        }
    }
    currentDistance = distance / numOfSampling;
}

// ���β���
// void Detect_Distance(void)
// {
//     int distance = 0;

//     distance = HCSR04_distanceInMillimeters();
//     if (distance > 0)
//     {
//         currentDistance = distance;
//     }
// }

//���͸�ң��
void Send_ToRemote(void)
{
    char buf[20] = {0};
    sprintf(buf, "Distance = %d", currentDistance);
    NRF_TxPacket_AP((uint8_t *)buf, strlen(buf));
    //printf("%s\r\n", buf);

    sprintf(buf, "Speed = %d", currentSpeed);
    NRF_TxPacket_AP((uint8_t *)buf, strlen(buf));
    //printf("%s\r\n", buf);

    memset(buf, 0, sizeof(buf)); //clear buf
}

//���͸����ڣ���λ����
void Report_ToSerial(void)
{
    printf("Systen running...\r\n");
}

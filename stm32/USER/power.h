#ifndef POWER_H
#define POWER_H
#include "stm32f10x_it.h"


#define DEALY_OFF_TIME 8000//�޲����ػ�ʱ��
 
#define BKP_DR2_FLAG 0X1111//���Ź���λ���
#define BKP_DR1_FLAG 0X1111//RTC�жϻ��ѱ��
#define BKP_DR3_FLAG 0X1111//RTC��ʼ�����

#define NO_ALARM 1
#if !NO_ALARM
#define S_WAKE 3600 //1��Сʱ 
#endif
void power_off_now(void);
void power_off_delay(void);
extern void system_bkp_init(void);
#endif

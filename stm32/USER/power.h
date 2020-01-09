#ifndef POWER_H
#define POWER_H
#include "stm32f10x_it.h"


#define DEALY_OFF_TIME 8000//无操作关机时间
 
#define BKP_DR2_FLAG 0X1111//看门狗复位标记
#define BKP_DR1_FLAG 0X1111//RTC中断唤醒标记
#define BKP_DR3_FLAG 0X1111//RTC初始化标记

#define NO_ALARM 1
#if !NO_ALARM
#define S_WAKE 3600 //1个小时 
#endif
void power_off_now(void);
void power_off_delay(void);
extern void system_bkp_init(void);
#endif

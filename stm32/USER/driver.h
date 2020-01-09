#ifndef driver_h

#define driver_h
#include "stm32f10x_it.h" 
#include "delay.h"
#include "gpio.h"
#include "flash.h"
#include "adc.h"
#include "iic.h"
#include "usart1.h"
#include "usart3.h"
#include "timer.h"
#include "wdg.h"
#include "rtc.h"
#include "power.h"

#include "systemString.h"
//data length
#define PWD_LEN_6 6 
#define TIME_DATA_LEN 6
#define RANDOM_PWD_LEN 8 //随机密码位数
//flash
#define IS_LOCKNO_ENABLE_ADDR						(ROM_BASE+SCORE_SIZE*0)
#define IS_LOCK_LOCKED				(ROM_BASE+SCORE_SIZE*1)
#define PWD_ADDR			(ROM_BASE+SCORE_SIZE*2)//正常六位数密码
#define RECORD_ADDR						(ROM_BASE+SCORE_SIZE*3)




extern void driver_init(void);
#endif

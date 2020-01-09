#include "power.h"
#include "wdg.h"
#include "usart1.h"
#include "interrupt.h"
void iwdg_init(FEED_TIME t);
/*
 *函数功能：系统进入standby模式
 *
*/
void enter_standby(void)
{
	#if !NO_ALARM
	 RTC_SetAlarm(RTC_GetCounter() + S_WAKE);//设置闹钟开启时间
	 RTC_WaitForLastTask(); 
	#endif
	 //记录数据
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问
	BKP_WriteBackupRegister(BKP_DR1, BKP_DR1_FLAG);	//写入记

	//强制看门狗复位，在复位程序里面进入standby模式
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问
	BKP_WriteBackupRegister(BKP_DR2, BKP_DR2_FLAG);	//写入记号，复位后不初始化看门狗
	rst_sys();
}
void enter_standby_nodog(void)
{
	BKP_WriteBackupRegister(BKP_DR2, 0X0000);	//写入记号，下次唤醒初始化看门狗
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能
	PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式 	
}

//函数功能：判断进入休眠还是喂狗
void feed_dog_or_standby(void)
{
	if(BKP_ReadBackupRegister(BKP_DR2) == BKP_DR2_FLAG)	//不初始化看门狗
	{
		enter_standby_nodog();
	}else
	{
		iwdg_init(_6S);
	}
}
//初始化后备寄存器
void system_bkp_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问
	feed_dog_or_standby();
}

extern void lock(void);
extern userTask user;
void power_off_now(void){
	
	#if DEBUGE
	return;
	#else
	if(user.is_auto_lock == TRUE){
		lock();
	}
	enter_standby();
	
	#endif
}
extern void set_systemTick(u32 t);
void power_off_delay(void){
	set_systemTick(0);
}

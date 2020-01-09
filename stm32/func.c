开锁关锁部分
/*
*功能：获取开锁时，电机的转动时间（ms）
*
*输入：无
*返回：转动时间
*/
u16 systemUnlockPin_get_openLockTime(void)
{
	u16 added=0;
	u8 battery = get_battery();//获取电池量
	added=3*(100-battery);//0--300
	return added+300;	//300-600
}
/*
*功能：获取关锁时，电机的转动时间（ms）
*
*输入：无
*返回：转动时间
*/
u16 systemUnlockPin_get_closeLockTime(void)
{
	return systemUnlockPin_get_openLockTime();	
}

/*
*功能：电机转动用于关锁（ms）
*
*输入：无
*返回：转动时间
*/
void systemUnlockPin_closeLock(void)
{
	__disable_irq();
	LOCK_PIN_LEFT = 0x01;
	LOCK_PIN_RIGHT = 0x00;//关
	system_delay_ms(systemUnlockPin_get_closeLockTime());
	LOCK_PIN_LEFT = 0x00;
	LOCK_PIN_RIGHT = 0x00;//无响应
	__enable_irq();
	systemUnlockPin_setLockIsOpened(FALSE);
}
/*
*功能：电机转动用于开锁
*
*输入：无
*返回：转动时间
*/
void systemUnlockPin_openLock(void)
{
	__disable_irq();
	LOCK_PIN_LEFT = 0x00;
	LOCK_PIN_RIGHT = 0x01;//关
	system_delay_ms(systemUnlockPin_get_openLockTime());
	LOCK_PIN_LEFT = 0x00;
	LOCK_PIN_RIGHT = 0x00;//无响应
	__enable_irq();
	systemUnlockPin_setLockIsOpened(TRUE);
}
//flash
/*******************************************************************************
*
*地址分配
*code size 48Kbytes 0x08000000 -- 0x0800BFFF
*memo size 16Kbytes 0x0800C000 -- 0x0800FFFF
*******************************************************************************/

//lock0
#define LOCKNO_ADDR						(ROM_BASE+SCORE_SIZE*0)//serial
#define IS_LOCK_LOCKED				(ROM_BASE+SCORE_SIZE*1)
#define PWD_N_IS_LOCKED(x)		(ROM_BASE+SCORE_SIZE*7+FLASH_FLAG_ELN2*x)//密码（用户）n的临时密码被禁用
#define TIMELESS_PWD_ADDR			(ROM_BASE+SCORE_SIZE*3)//时效密码0
#define SUPER_PWD_ADDR				(ROM_BASE+SCORE_SIZE*4)//超级密码
#define Renovation_ADDR				(ROM_BASE+SCORE_SIZE*5)//装修密码
#define RECORD_ADDR						(ROM_BASE+SCORE_SIZE*6)







//timer
typedef struct{
	BOOL timerForKey;
	u32 timerForKeyNum;
}TIMER;
static TIMER timerFor={
	FALSE,0,
};
//对timerForKey操作
BOOL systemTimer_get_timerForKey(void){
	return timerFor.timerForKey;
}
void systemTimer_set_timerForKey(BOOL b){
	timerFor.timerForKey = b;
}
//对 timerForKeyNum操作
u32 systemTimer_get_timerForKeyNum(void){
	return timerFor.timerForKeyNum;
}
void systemTimer_set_timerForKeyNum(u32 d){
	timerFor.timerForKeyNum = d;
}
void systemTimer_set_timerForKeyNum_selfAdd1(void){
	timerFor.timerForKeyNum+=1;
}


#define TIMER_GAP 30
#define LONG_PRESS_TIME 600
extern BOOL systemTimer_get_timerForKey(void);
extern void systemTimer_set_timerForKey(BOOL b);
extern u32 systemTimer_get_timerForKeyNum(void);
extern void systemTimer_set_timerForKeyNum(u32 d);
extern void systemTimer_set_timerForKeyNum_selfAdd1(void);


//power
///////////////////////////////////////
//外部函数
void poweroff_delay(void)
{
	systemTick.timer_power=0;
}
extern void Tcmd_upload_lock_sta_gate(void);
void poweroff_now(void)
{
	Tcmd_upload_lock_sta_gate();
	if(systemUnlockPin_getLockIsOpened() == TRUE)//意外导致没有关锁
	{
		systemUnlockPin_closeLock();
	}
	#if DEBUGE
	p1("POWEROFF_OVERTIME\r\n");
	#endif
	enter_standby();
}
/*
*功能：根据按键输入值，做相应操作
*
*输入：	u8 button_val
	1：清空寄存器值
	0：设备待机
*返回：无
*/
void exe_button_value(u8 button_val)
{
	if(button_val == 1)//检查按键
	{
		BKP_WriteBackupRegister(BKP_DR1, 0);	
	}else
	if(BKP_ReadBackupRegister(BKP_DR1) == BKP_DR1_FLAG)	//表示RTC中断唤醒
	{
		poweroff_now();
	}	
}

//rtc
//十进制转为16进制
//输入：十进制
//输出：十六进制
//举例：输入20；输出：0x20；输入：39：输出：0x39
u8 dec_hex_rtc(u8 i){
	u8 a,b;
	a=i/10;//十位
	b=i%10;//个位
	return (a*16+b);
}
//16进制转为10进制
//输入：16进制
//输出：10进制
//举例：输入0x20；输出：20；输入：0x39：输出：39
u8 hex_dec_rtc(u8 i){
	u8 a,b;
	a=i/16;//
	b=i%16;
	return (a*10+b);
}


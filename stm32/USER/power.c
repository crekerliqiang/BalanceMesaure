#include "power.h"
#include "wdg.h"
#include "usart1.h"
#include "interrupt.h"
void iwdg_init(FEED_TIME t);
/*
 *�������ܣ�ϵͳ����standbyģʽ
 *
*/
void enter_standby(void)
{
	#if !NO_ALARM
	 RTC_SetAlarm(RTC_GetCounter() + S_WAKE);//�������ӿ���ʱ��
	 RTC_WaitForLastTask(); 
	#endif
	 //��¼����
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������
	BKP_WriteBackupRegister(BKP_DR1, BKP_DR1_FLAG);	//д���

	//ǿ�ƿ��Ź���λ���ڸ�λ�����������standbyģʽ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������
	BKP_WriteBackupRegister(BKP_DR2, BKP_DR2_FLAG);	//д��Ǻţ���λ�󲻳�ʼ�����Ź�
	rst_sys();
}
void enter_standby_nodog(void)
{
	BKP_WriteBackupRegister(BKP_DR2, 0X0000);	//д��Ǻţ��´λ��ѳ�ʼ�����Ź�
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 	
}

//�������ܣ��жϽ������߻���ι��
void feed_dog_or_standby(void)
{
	if(BKP_ReadBackupRegister(BKP_DR2) == BKP_DR2_FLAG)	//����ʼ�����Ź�
	{
		enter_standby_nodog();
	}else
	{
		iwdg_init(_6S);
	}
}
//��ʼ���󱸼Ĵ���
void system_bkp_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������
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

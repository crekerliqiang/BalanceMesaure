#include "rtc.h" 		
#include "delay.h"
#include "systemString.h"
_calendar_obj calendar;
//ʮ����תΪ16����
//���룺ʮ����
//�����ʮ������
//����������20�������0x20�����룺39�������0x39
u8 dec_hex_rtc(u8 i){
	u8 a,b;
	a=i/10;//ʮλ
	b=i%10;//��λ
	return (a*16+b);
}
//16����תΪ10����
//���룺16����
//�����10����
//����������0x20�������20�����룺0x39�������39
u8 hex_dec_rtc(u8 i){
	u8 a,b;
	a=i/16;//
	b=i%16;
	return (a*10+b);
}



//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year){
	if(year%4==0){ 
		if(year%100==0){ 
			if(year%400==0)return 1;	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

static u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec){
	u16 t;
	u32 seccount=0;
	if(syear<2017||syear>2117)
		return 1;	   
	for(t=2016;t<syear;t++){
		if(Is_Leap_Year(t))
			seccount+=31622400;//�����������
		else 
			seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++){
		seccount+=(u32)mon_table[t]*86400;
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;   
	}
	seccount+=(u32)(sday-1)*86400;
	seccount+=(u32)hour*3600;
  seccount+=(u32)min*60;	
	seccount+=sec;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetCounter(seccount);

	RTC_WaitForLastTask();
	return 0;	    
}

//�õ���ǰ��ʱ�� ������ʱ����
//����ֵ:0,�ɹ�;����:�������.

static u8 RTC_Get(void){
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp){	  
		daycnt=temp;
		temp1=2016;	//��2016�꿪ʼ
		while(temp>=365){
			if(Is_Leap_Year(temp1)){
				if(temp>=366){
					temp-=366;
				}else{
					temp1++;
					break;
				}  
			}else{
			temp-=365;
			}
			temp1++;  
		}
		calendar.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28){
			if(Is_Leap_Year(calendar.w_year)&&temp1==1){
				if(temp>=29)
					temp-=29;//�����������
				else
					break; 
			}else {
				if(temp>=mon_table[temp1])
					temp-=mon_table[temp1];//ƽ��
				else
					break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//�õ��·�
		calendar.w_date=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������   	   
	calendar.hour=temp/3600;     	//Сʱ
	calendar.min=(temp%3600)/60; 	//����	
	calendar.sec=(temp%3600)%60; 	//����
	return 0;
}
void write_time(u8 buffer_time[6])
{
	u8 write_buf[6]={0};
	my_hexncpy(write_buf,0,buffer_time,0,6);//��������
	//�겹��ǰ��λ
	calendar.w_year=2000+write_buf[0];
	calendar.w_month=write_buf[1];
	calendar.w_date=write_buf[2];
	calendar.hour=write_buf[3];
	calendar.min=write_buf[4];
	calendar.sec=write_buf[5];
	RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,\
		calendar.hour,calendar.min,calendar.sec);	
}
void read_time(u8 buffer_time[6])
{
	RTC_Get();
	//�õ�ʮ����
	buffer_time[0]=calendar.w_year%100;
	buffer_time[1]=calendar.w_month;
	buffer_time[2]=calendar.w_date;
	buffer_time[3]=calendar.hour;
	buffer_time[4]=calendar.min;
	buffer_time[5]=calendar.sec;
}
/*
*���ܣ��ѵ�ǰʱ��ת��Ϊ��������2016�꿪ʼ�㣩
*
*���룺u8 *time:17 1 12 12 12 00 ֧�ֵ�2099�� ʮ����
*���أ�ת��������� ����0��ʾ���ݷǷ�
*/
u32 time_to_SEC(const u8 *time)
{
	u16 syear;
	u8 smon,sday,hour,min,sec;
	const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	u16 t;
	u32 seccount=0;
	if(time[0]<17)return 0;
	if(time[0]>99)return 0;
	syear=time[0];smon=time[1];sday=time[2];
	hour=time[3];min=time[4];sec=time[5];
	syear=syear+2000;
	for(t=2016;t<syear;t++)	//��������ݵ��������
	{
			if(Is_Leap_Year(t))
				seccount+=31622400;//�����������
			else
				seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
			seccount+=(u32)mon_table[t]*86400;//�·����������
			if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ����������
	seccount+=(u32)hour*3600;//Сʱ������
	seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
  return seccount;	
}




u8 rtc_init(void)
{
	//����ǲ��ǵ�һ������ʱ��
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x26F0)		//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{
		RCC_LSEConfig(RCC_LSE_ON);	//�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)	//���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			system_delay_ms(10);
		}
		if(temp>=250)
			return 1;//��ʼ��ʱ��ʧ��,����������	  
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
			
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
			
//		RTC_ITConfig(RTC_IT_ALR, ENABLE);		//ʹ��RTC�����ж�
		RTC_WaitForLastTask();			
		RTC_ClearFlag(RTC_FLAG_SEC);
		while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);

		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������	
		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ExitConfigMode(); //�˳�����ģʽ 
//		RTC_Set(2017,7,22,9,39,30);  //����ʱ��	
		BKP_WriteBackupRegister(BKP_DR1, 0X26F0);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	}		     
	return 0; //ok
}
#if !NO_ALARM

void RTC_IRQHandler(void)
{
 if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
 {
  RTC_ClearITPendingBit(RTC_IT_ALR);  //�������ж�
 }
 RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);  //�������ж�
 RTC_WaitForLastTask();
}
#endif

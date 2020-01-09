#include "rtc.h" 		
#include "delay.h"
#include "systemString.h"
_calendar_obj calendar;
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



//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year){
	if(year%4==0){ 
		if(year%100==0){ 
			if(year%400==0)return 1;	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

static u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec){
	u16 t;
	u32 seccount=0;
	if(syear<2017||syear>2117)
		return 1;	   
	for(t=2016;t<syear;t++){
		if(Is_Leap_Year(t))
			seccount+=31622400;//闰年的秒钟数
		else 
			seccount+=31536000;			  //平年的秒钟数
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

//得到当前的时间 年月日时分秒
//返回值:0,成功;其他:错误代码.

static u8 RTC_Get(void){
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp){	  
		daycnt=temp;
		temp1=2016;	//从2016年开始
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
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28){
			if(Is_Leap_Year(calendar.w_year)&&temp1==1){
				if(temp>=29)
					temp-=29;//闰年的秒钟数
				else
					break; 
			}else {
				if(temp>=mon_table[temp1])
					temp-=mon_table[temp1];//平年
				else
					break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟	
	calendar.sec=(temp%3600)%60; 	//秒钟
	return 0;
}
void write_time(u8 buffer_time[6])
{
	u8 write_buf[6]={0};
	my_hexncpy(write_buf,0,buffer_time,0,6);//复制数据
	//年补上前两位
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
	//得到十进制
	buffer_time[0]=calendar.w_year%100;
	buffer_time[1]=calendar.w_month;
	buffer_time[2]=calendar.w_date;
	buffer_time[3]=calendar.hour;
	buffer_time[4]=calendar.min;
	buffer_time[5]=calendar.sec;
}
/*
*功能：把当前时间转化为秒数（从2016年开始算）
*
*输入：u8 *time:17 1 12 12 12 00 支持到2099年 十进制
*返回：转化后的秒数 返回0表示数据非法
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
	for(t=2016;t<syear;t++)	//把所有年份的秒钟相加
	{
			if(Is_Leap_Year(t))
				seccount+=31622400;//闰年的秒钟数
			else
				seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
			seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
			if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加
	seccount+=(u32)hour*3600;//小时秒钟数
	seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去
  return seccount;	
}




u8 rtc_init(void)
{
	//检查是不是第一次配置时钟
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x26F0)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{
		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			system_delay_ms(10);
		}
		if(temp>=250)
			return 1;//初始化时钟失败,晶振有问题	  
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
			
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		//等待RTC寄存器同步  
			
//		RTC_ITConfig(RTC_IT_ALR, ENABLE);		//使能RTC闹钟中断
		RTC_WaitForLastTask();			
		RTC_ClearFlag(RTC_FLAG_SEC);
		while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);

		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ExitConfigMode(); //退出配置模式 
//		RTC_Set(2017,7,22,9,39,30);  //设置时间	
		BKP_WriteBackupRegister(BKP_DR1, 0X26F0);	//向指定的后备寄存器中写入用户程序数据
	}		     
	return 0; //ok
}
#if !NO_ALARM

void RTC_IRQHandler(void)
{
 if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
 {
  RTC_ClearITPendingBit(RTC_IT_ALR);  //清闹钟中断
 }
 RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);  //清闹钟中断
 RTC_WaitForLastTask();
}
#endif

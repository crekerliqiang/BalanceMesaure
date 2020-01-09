//
//  decryptPwdV2.c
//  arithmetic
//
//  Created by liqiang on 2018/4/25.
//  Copyright © 2018 李强. All rights reserved.
//

#include "decryptPwdV2.h"
#include "decryptPwd.h"
#include "passwordTool.h"
#include "usart1.h"
	//判断是否是闰年函数
	//月份   1  2  3  4  5  6  7  8  9  10 11 12
	//闰年   31 29 31 30 31 30 31 31 30 31 30 31
	//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
	//输入:年份
	//输出:该年份是不是闰年.1,是.0,不是
	static BOOL Is_Leap_Year(int year)
	{			  
		if(year%4==0) //必须能被4整除
		{ 
			if(year%100==0) 
			{ 
				if(year%400==0)return TRUE;//如果以00结尾,还要能被400整除 	   
				else return FALSE;   
			}else return TRUE;   
		}else return FALSE;	
	}	 		
	/*
	*功能：把当前时间转化为分钟（从2018年开始算）
	*
	*输入：int [] time:17 1 12 12 12 00 支持到2099年 十进制
	*返回：转化后的分钟数 返回0表示数据非法
	*/
	static int time_to_minute(const int * time)
	{
		int syear;
		int smon,sday,hour,min,sec;
		const int mon_table[] = {31,28,31,30,31,30,31,31,30,31,30,31};
		int t;
		int seccount=0;
		if(time[0]<2017)return 0;
		if(time[0]>2099)return 0;
		syear=time[0];smon=time[1];sday=time[2];
		hour=time[3];min=time[4];sec=time[5];
		for(t=2018;t<syear;t++)	//把所有年份的秒钟相加
		{
			if(Is_Leap_Year(t))
				seccount+=31622400;//闰年的秒钟数
			else
				seccount+=31536000;			  //平年的秒钟数
		}
		smon-=1;
		for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
		{
				seccount+=mon_table[t]*86400;//月份秒钟数相加
				if(Is_Leap_Year(syear)&&t==TRUE)seccount+=86400;//闰年2月份增加一天的秒钟数
		}
		seccount+=(sday-1)*86400;//把前面日期的秒钟数相加
		seccount+=hour*3600;//小时秒钟数
		seccount+=min*60;	 //分钟秒钟数
		seccount+=sec;//最后的秒钟加上去
	  return seccount/60;	
	}
		/**
	 * 判断密码是否存在跨年的可能性(比如18年申请的两小时密码，19年才过期）
	 * 如果get方法输入的时间小于1月1日2点，则说明申请的时间是前一年
	 */
	BOOL isTimeOverYear(const int * time){
		if(time[1] == 1 && time[2] == 1 && time[3] < 2){
			return TRUE;
		}
		return FALSE;
	}
/*
 * 校验数据
 * 输入：const int [] realTime；锁内的时间的日时分,6位数组
 * 输出：int * deadTimeOut ;该密码对应的失效时间
 * 返回值
 * -1：时间不合法
 * -2：数据校验错误
 * -3：密码已经过期
 * -4：密码正确时间合法，但是超出2小时范围
 * 1：密码正确
 */
int checkDataV2(const int* keyInputPwd,const int * lockPwd,const int * realTime,int * deadTimeOut) {
		int time0[6] = {0};
		int minutes = 0;
		int time_minute[6]={0};
		//1.5
		
		if(TRUE == isTimeOverYear(realTime)){
			time0[0]=realTime[0] - 1;
			
			#if DEBUGE
			p1("decode pwd  pwd over year----\r\n");
			#endif
			
		}else{
			time0[0]=realTime[0];
		}
		time0[1]=1;
		time0[2]=1;
		time0[3]=0;
		time0[4]=0;
		time0[5]=0;
		//2
		minutes = time_to_minute(realTime) - time_to_minute(time0);
		//3
		time_minute[0]=minutes/100000;
		time_minute[1]=(minutes%100000)/10000;
		time_minute[2]=(minutes%10000)/1000;
		time_minute[3]=(minutes%1000)/100;
		time_minute[4]=(minutes%100)/10;
		time_minute[5]=minutes%10;
	
		#if DEBUGE
		p1("=====================\r\n");
		p1("realTime ");pln(realTime,6);
		p1("time0 ");pln(time0,6);
		p1("minutes %d \r\n",minutes);
		p1("time_minute ");pln(time_minute,6);
		
		
		p1("=====================\r\n");
		#endif
		return checkDataV2_Father(keyInputPwd,lockPwd,time_minute,deadTimeOut);

}

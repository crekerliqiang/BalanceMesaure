#include "randomPwd.h"
#include "lock.h"
#include "decryptPwd.h"
#include "decryptPwdV2.h"
/*函数功能：把数组变成数据
*输入：u8 *pwd:数组的首地址 ，如：1,2,3,4,5,6,7,8;pwd的数据必须为单数据，不能有0x12这样子的
				u8 len;需要转化的数组长度，4--8
*返回：int数据 int a = 12345678
*注：只让函数judge_renovation_legal使用
*/
int single_arr_to_int(const u8 *pwd,u8 len)
{
	u8 i;
	int sum=0;
	for(i=0;i<len;i++)
	{
		sum+=(pow(10,i)*pwd[len-1-i]);
	}
	return sum;
}

/*
 函数功能：判断时间数据是否合法
 输入：int time：表示时间的日时分(11250)
 输出：无
 返回：1，数据合法；0，数据不合法
 */
u8 judge_time_legal(int time)
{
    int time_buf=time;
    u8 time_arr[3]={0};
    time_arr[0]=time_buf/10000;//日
    time_arr[1]=(time_buf/100)%100;//时
    time_arr[2]=time_buf%100;//分
    if((time_arr[0]>31)&&(time_arr[0]==0))//日
    {
        return 0;
    }
    if(time_arr[1]>24)//时
    {
        return 0;
    }
    if(time_arr[2]>59)//分
    {
        return 0;
    }
    return 1;
}
//judge_TemporaryPwd_legal 内部函数
//输入：int time:日时分，121212
//输出：u8 *whole_time:年月日时分秒，十进制：17 1 12 12 12 00
void Complete_time(int time,u8 *whole_time)
{
	u8 time_arr[3];
	time_arr[0]=time/10000;
	time_arr[1]=time%10000/100;
	time_arr[2]=time%100;
	read_time(whole_time);
	my_hexncpy(whole_time,2,time_arr,0,3);
}





/*
*功能：判断临时密码是否合法
*
*输入：	u8 *pwd，临时密码数组首地址
*返回：
	PWD_WRONG,密码错误
	PWD_OK_OVERTUE,密码过期（前提是正确）
	PWD_OK,密码正确
*/
//PWD_STA judge_TemporaryPwd_legal(const int *pwd)
//{
//	u32 user_Input,pwd_time,knowledge_Pwd,end_time;
//	PWD_STA sta=PWD_WRONG;
//	u8 arr_time_password[12]={0};
//	int arr_time_password_int[12] ={0};
//	u8 end_time_Hole[TIME_DATA_LEN]={0};
//	u8 crc=0;
//	u8 i=0;
//	u8 real_time_arr[TIME_DATA_LEN]={0},real_time_use[3]={0};
//	u8 lock_pwd[PWD_LEN_6]={0};
//	u32 real_time;
//	int x_time=0;//时间差
//	u8 pwd_u8[RANDOM_PWD_LEN] = {0};
//	
//	for(i=0;i<RANDOM_PWD_LEN;i++)
//		pwd_u8[i] = pwd[i];
//	user_Input = single_arr_to_int(pwd_u8,RANDOM_PWD_LEN);//键盘输入的密码，转为int型
//	
//	read_time(real_time_arr);
//	
//	#if DEBUGE
//	p1("real time ");
//	for(i=0;i<6;i++)
//		p1("%d ",real_time_arr[i]);
//	p1("\r\n");
//	#endif
//	
//	if(real_time_arr[0]<17){
//		
//			#if DEBUGE
//			p1("real time illegal======\r\n");
//			#endif
//		
//		
//		return PWD_NOTIMEDATA;
//	}
//	real_time_use[0]=real_time_arr[2];//日   十进制
//	real_time_use[1]=real_time_arr[3];//时
//	real_time_use[2]=real_time_arr[4];//分
//	real_time=real_time_use[0]*10000+real_time_use[1]*100+real_time_use[2];//当前时间，日 时 分
//	
//		get_unlock_code(lock_pwd);
//		knowledge_Pwd=single_arr_to_int(lock_pwd,PWD_LEN_6);//假设锁里的时效密码，int型
//	
//		#if DEBUGE
//		p1("lock pwd is   %d  \r\n",knowledge_Pwd);
//		#endif
//	
//		pwd_time=user_Input/100;//去掉密码中的CRC校验码部分
//		end_time=pwd_time^knowledge_Pwd;//根据已知的密码获取结束时间
//	
//		#if DEBUGE
//		p1("endtime is   %d  \r\n",end_time);
//		#endif	
//		
//		if(judge_time_legal(end_time)==1)//判断结束时间是否合法
//		{
//				double_int_to_arr(end_time,knowledge_Pwd,arr_time_password);
//				for(i=0;i<12;i++){
//					arr_time_password_int[i] = arr_time_password[i];
//				}
//				crc=GetCRC8(arr_time_password_int,12)%100;
//				if(crc==(user_Input%100))
//				{
//					//根据结束时间dead_time 补全时间
//					Complete_time(end_time,end_time_Hole);
//					//把当前时间和结束时间转化为S值
//					end_time = time_to_SEC(end_time_Hole);
//					real_time = time_to_SEC(real_time_arr);
//					x_time=end_time - real_time;
//					
//					#if DEBUGE
//					p1("end_time %d real_time %d x_time   %d  mins \r\n",end_time,real_time,x_time/60);
//					p1("end_time_Hole ");
//					for(i=0;i<6;i++)
//						p1("%d ",end_time_Hole[i]);
//					p1("\r\n");
//					#endif
//					
//					if(x_time >= 0)//校验是否过期
//					{
//						sta=PWD_OK;
//					}else
//					{
//						
//						#if DEBUGE
//						p1("PWD_OK_OVERTUE  \r\n");
//						#endif
//						
//						sta=PWD_OK_OVERTUE;
//					}
//				}else
//				{
//					#if DEBUGE
//					p1("crc Wrong  %d   %d\r\n",crc,user_Input%100);
//					#endif
//					sta = PWD_WRONG;
//				}
//		}else
//		{
//			sta = PWD_WRONG;
//		}
//	return sta;
//}

PWD_STA judge_TemporaryPwd_legal(const int *keyInputPwd){
	PWD_STA	sta,overtueExist;
	u8 lockPwd[PWD_LEN_6]={0},realTime[TIME_DATA_LEN]={0};//所内的密码，包括了超级密码(只取六位)
	int lockPwd_int[PWD_LEN_6]={0};  
	int deadTime[6]={0};
	int realTime_Full[6]={0};//{2018,1,12,12,12,12};  V2算法用 
	int k=0;
	int ack=0;
			
	
	read_time(realTime);
		
	#if DEBUGE 
	p1("input pwd   ");pln(keyInputPwd,9);
	p1("real time    ");plnu8(realTime,6);
	#endif
		get_unlock_code(lockPwd);
		for(k=0;k<PWD_LEN_6;k++)
			lockPwd_int[k]=lockPwd[k];
		for(k=0;k<6;k++){
			if(k == 0)
				realTime_Full[k] = realTime[k] + 2000;
			else
				realTime_Full[k] = realTime[k];
		}
		ack=checkDataV2(keyInputPwd,lockPwd_int,realTime_Full,deadTime);//V1算法用readTime_int V2用readTime_Full
		
		if(ack == 1){
				#if DEBUGE 
				p1("ack == 1 OK\r\n");
				plnu8(lockPwd,6);p1("TemporaryPwd OK\r\n");
				#endif	
			//临时密码判断成功后直接跳出
				sta=PWD_OK;				
		}else
		if(ack == -1 || ack == -2){
			sta=PWD_WRONG;
			#if DEBUGE 
			plnu8(lockPwd,6);
			if(ack == -1)
				p1("-1 TemporaryPwd 	WRONG timeIllegal\r\n");
			if(ack == -2)
				p1("-2 TemporaryPwd 	WRONG crc wrong\r\n");
			#endif				
		}else
		if(ack == -3 || ack == -4){
			sta=PWD_OK_OVERTUE;
			overtueExist=PWD_OK_OVERTUE;
			#if DEBUGE 
			plnu8(lockPwd,6);p1("%d TemporaryPwd 	PWD_OK_OVERTUE\r\n",ack);
			#endif	
		}else{
			sta=PWD_WRONG;
			#if DEBUGE 
			p1("TemporaryPwd 	CANNT BE!!!!\r\n");
			#endif
		}
	
	if(sta!=PWD_OK){
		sta=overtueExist==PWD_OK_OVERTUE?PWD_OK_OVERTUE:sta;//11个密码重没有正确密码，且中间存在超时
	}
		#if DEBUGE 
		p1("\r\n\r\n\r\n\r\n");
		#endif
	return sta;
}

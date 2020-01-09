#include "randomPwd.h"
#include "lock.h"
#include "decryptPwd.h"
#include "decryptPwdV2.h"
/*�������ܣ�������������
*���룺u8 *pwd:������׵�ַ ���磺1,2,3,4,5,6,7,8;pwd�����ݱ���Ϊ�����ݣ�������0x12�����ӵ�
				u8 len;��Ҫת�������鳤�ȣ�4--8
*���أ�int���� int a = 12345678
*ע��ֻ�ú���judge_renovation_legalʹ��
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
 �������ܣ��ж�ʱ�������Ƿ�Ϸ�
 ���룺int time����ʾʱ�����ʱ��(11250)
 �������
 ���أ�1�����ݺϷ���0�����ݲ��Ϸ�
 */
u8 judge_time_legal(int time)
{
    int time_buf=time;
    u8 time_arr[3]={0};
    time_arr[0]=time_buf/10000;//��
    time_arr[1]=(time_buf/100)%100;//ʱ
    time_arr[2]=time_buf%100;//��
    if((time_arr[0]>31)&&(time_arr[0]==0))//��
    {
        return 0;
    }
    if(time_arr[1]>24)//ʱ
    {
        return 0;
    }
    if(time_arr[2]>59)//��
    {
        return 0;
    }
    return 1;
}
//judge_TemporaryPwd_legal �ڲ�����
//���룺int time:��ʱ�֣�121212
//�����u8 *whole_time:������ʱ���룬ʮ���ƣ�17 1 12 12 12 00
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
*���ܣ��ж���ʱ�����Ƿ�Ϸ�
*
*���룺	u8 *pwd����ʱ���������׵�ַ
*���أ�
	PWD_WRONG,�������
	PWD_OK_OVERTUE,������ڣ�ǰ������ȷ��
	PWD_OK,������ȷ
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
//	int x_time=0;//ʱ���
//	u8 pwd_u8[RANDOM_PWD_LEN] = {0};
//	
//	for(i=0;i<RANDOM_PWD_LEN;i++)
//		pwd_u8[i] = pwd[i];
//	user_Input = single_arr_to_int(pwd_u8,RANDOM_PWD_LEN);//������������룬תΪint��
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
//	real_time_use[0]=real_time_arr[2];//��   ʮ����
//	real_time_use[1]=real_time_arr[3];//ʱ
//	real_time_use[2]=real_time_arr[4];//��
//	real_time=real_time_use[0]*10000+real_time_use[1]*100+real_time_use[2];//��ǰʱ�䣬�� ʱ ��
//	
//		get_unlock_code(lock_pwd);
//		knowledge_Pwd=single_arr_to_int(lock_pwd,PWD_LEN_6);//���������ʱЧ���룬int��
//	
//		#if DEBUGE
//		p1("lock pwd is   %d  \r\n",knowledge_Pwd);
//		#endif
//	
//		pwd_time=user_Input/100;//ȥ�������е�CRCУ���벿��
//		end_time=pwd_time^knowledge_Pwd;//������֪�������ȡ����ʱ��
//	
//		#if DEBUGE
//		p1("endtime is   %d  \r\n",end_time);
//		#endif	
//		
//		if(judge_time_legal(end_time)==1)//�жϽ���ʱ���Ƿ�Ϸ�
//		{
//				double_int_to_arr(end_time,knowledge_Pwd,arr_time_password);
//				for(i=0;i<12;i++){
//					arr_time_password_int[i] = arr_time_password[i];
//				}
//				crc=GetCRC8(arr_time_password_int,12)%100;
//				if(crc==(user_Input%100))
//				{
//					//���ݽ���ʱ��dead_time ��ȫʱ��
//					Complete_time(end_time,end_time_Hole);
//					//�ѵ�ǰʱ��ͽ���ʱ��ת��ΪSֵ
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
//					if(x_time >= 0)//У���Ƿ����
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
	u8 lockPwd[PWD_LEN_6]={0},realTime[TIME_DATA_LEN]={0};//���ڵ����룬�����˳�������(ֻȡ��λ)
	int lockPwd_int[PWD_LEN_6]={0};  
	int deadTime[6]={0};
	int realTime_Full[6]={0};//{2018,1,12,12,12,12};  V2�㷨�� 
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
		ack=checkDataV2(keyInputPwd,lockPwd_int,realTime_Full,deadTime);//V1�㷨��readTime_int V2��readTime_Full
		
		if(ack == 1){
				#if DEBUGE 
				p1("ack == 1 OK\r\n");
				plnu8(lockPwd,6);p1("TemporaryPwd OK\r\n");
				#endif	
			//��ʱ�����жϳɹ���ֱ������
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
		sta=overtueExist==PWD_OK_OVERTUE?PWD_OK_OVERTUE:sta;//11��������û����ȷ���룬���м���ڳ�ʱ
	}
		#if DEBUGE 
		p1("\r\n\r\n\r\n\r\n");
		#endif
	return sta;
}

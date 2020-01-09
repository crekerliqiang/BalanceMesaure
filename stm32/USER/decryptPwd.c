//
//  decryptPwd.c
//  arithmetic
//
//  Created by liqiang on 2018/4/25.
//  Copyright © 2018 李强. All rights reserved.
//

#include "decryptPwd.h"
#include "passwordTool.h"
#include "usart1.h"
/*
 * 解密临时密码数据
 * 返回值：int value[9]
 * index0-index5：开锁时间 （日时分）
 * index6-index8:hash值（用于计算该CRC值与锁内的密码+index1-6的时间是否一致）
 *
 */
void getTimeAndHash(const  int * keyInputPwd,const int * lockPwd,int *value) {
    int num[6] = {0};
    int time[6]={0};
    int clear_pwd[9] = {0};
    int i = 0;
    //解混淆
    mixArrDe(keyInputPwd,clear_pwd);
    //time
    for(i = 0;i<6;i++) {
        num[i] = clear_pwd[i];
    }
    //解time
    arrSpicialSubtraction(num,lockPwd,time);
    for(i=0;i<6;i++) {
        value[i] = time[i];
    }
    //赋值hash
    for(i = 0;i<3;i++) {
        value[6+i] = clear_pwd[6+i];
    }
    return;
}
/*
 * 获取时间   获取hash
 * 是上面一个方法的两个具体化
 */
void getTime(const int * keyInputPwd,const int* lockPwd,int *returnTime){
    int data[9]={0};
		int i=0;
    getTimeAndHash(keyInputPwd,lockPwd,data);
    for(i=0;i<6;i++) {
        returnTime[i] = data[i];
    }
    return ;
}
void getHash(const int * keyInputPwd,const int * lockPwd,int *returnHash) {
    int data[9]={0};
		int i=0;
    getTimeAndHash(keyInputPwd,lockPwd,data);
    for(i=0;i<3;i++) {
        returnHash[i] = data[i+6];
    }
    return ;
}
/*
 * 判断时间（日时分 0 4 1 2 4 5）是否合法
 *返回1合法
 */
int checkTimeIsLegal(int * time) {
		if(((time[0]*10+time[1])>=32) || ((time[0]*10+time[1]) == 0))//日
        return 0;
    if((time[2]*10+time[3]) >= 24)//时
        return 0;
    if((time[4]*10+time[5])>=60)//分
        return 0;
    return 1;
}

/*
 * 将时间数据日时分转化为分钟
 */
int timeToMinutes(const int * time) {
    int num = 0;
    num += (time[0]*10+time[1] -1)*24*60;//日
    num += (time[2]*10+time[3])*60;//时
    num += (time[4]*10+time[5]);//分
    return num;
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
int checkData(const int* keyInputPwd,const int * lockPwd,const int * realTime,int * deadTimeOut) {
    int keyInputPwd_time[6]= {0};
    int keyInputPwdHash[3]={0};
    int InputTimeAndLockPwdHash[3] = {0};
    int i = 0,t1=0,t2=0;
    getTime(keyInputPwd,lockPwd,keyInputPwd_time);//日时分
    getHash(keyInputPwd,lockPwd,keyInputPwdHash);
		for(i=0;i<6;i++){
			deadTimeOut[i] = keyInputPwd_time[i];
		}
		#if DEBUGE
		p1("input  ");pln(keyInputPwd,9);
		p1("pwd    ");pln(lockPwd,6);
		p1("time   ");pln(keyInputPwd_time,6);
		p1("rl time");pln(realTime,6);
		#endif
		
    //1判断time是否合法
    if(checkTimeIsLegal(keyInputPwd_time) == 0){
			#if DEBUGE
			p1("timelegael   \r\n\r\n");
			#endif	
			
			return -1;
		}
    //check hash                    lockPwd+keyInputPwd_time
    getArrHashNum(keyInputPwd_time,lockPwd,InputTimeAndLockPwdHash);

    for(i=0;i<3;i++) {
        if(keyInputPwdHash[i] != InputTimeAndLockPwdHash[i]){
					#if DEBUGE
					p1("keyInputPwdHash wrong   \r\n\r\n");
					pln(keyInputPwdHash,3);
					pln(InputTimeAndLockPwdHash,3);
					#endif	
					return -2;
				}
    }

    //时间有没有过期，判断是否超过2小时（120分钟）
    t1 = timeToMinutes(keyInputPwd_time);
    t2 = timeToMinutes(realTime);
		#if DEBUGE
		p1("t1  %d  t2  %d  x:%d \r\n\r\n",t1,t2,t1-t2);
		#endif
		if( t1 - t2 < 0){
			return -3;
		}
    if(t1 - t2 > 120) {
        return -4;
    }
    return 1;
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
int timeArr_to_int(const int * time){
		int num=0;
		num+=time[0]*100000;
		num+=time[1]*10000;
		num+=time[2]*1000;
		num+=time[3]*100;
		num+=time[4]*10;
		num+=time[5]*1;
		return num;
	}
int checkDataV2_Father(const int* keyInputPwd,const int * lockPwd,const int * realTime,int * deadTimeOut) {
    int keyInputPwd_time[6]= {0};
    int keyInputPwdHash[3]={0};
    int InputTimeAndLockPwdHash[3] = {0};
    int i = 0,t1=0,t2=0;
    getTime(keyInputPwd,lockPwd,keyInputPwd_time);//日时分
    getHash(keyInputPwd,lockPwd,keyInputPwdHash);
		for(i=0;i<6;i++){
			deadTimeOut[i] = keyInputPwd_time[i];
		}
		#if DEBUGE
		p1("input pwd ");pln(keyInputPwd,9);
		p1("lock  pwd ");pln(lockPwd,6);
		p1("input time ");pln(keyInputPwd_time,6);//从临时密码中获取到的时间
		p1("real  time ");pln(realTime,6);
		p1("dead  time ");pln(deadTimeOut,6);//从临时密码中获取到的时间
		
		#endif
    //check hash                    lockPwd+keyInputPwd_time
    getArrHashNum(keyInputPwd_time,lockPwd,InputTimeAndLockPwdHash);

    for(i=0;i<3;i++) {
        if(keyInputPwdHash[i] != InputTimeAndLockPwdHash[i]){
					#if DEBUGE
					p1("keyInputPwdHash wrong   \r\n\r\n");
					pln(keyInputPwdHash,3);
					pln(InputTimeAndLockPwdHash,3);
					#endif	
					return -2;
				}
    }

    //时间有没有过期，判断是否超过2小时（120分钟）
    t1 = timeArr_to_int(keyInputPwd_time);//时效时间
    t2 = timeArr_to_int(realTime);//当前时间
		#if DEBUGE
		p1("t1  %d  t2  %d  x:%d \r\n\r\n",t1,t2,t1-t2);
		#endif
		if( t1 - t2 < 0){
			return -3;
		}
    if(t1 - t2 > 120) {
        return -4;
    }
    return 1;
}

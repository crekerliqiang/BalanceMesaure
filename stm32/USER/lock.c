//
//  lock.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//
#include "lock.h"
//判断密码是否合法 rule:必须是0-9的数
static BOOL is_pwd_legal(const u8 *src,const int len){
	int i;
	for(i=0;i<len;i++){
		if(src[i]>9)return FALSE;
	}
	return TRUE;
}
//读取开锁密码 如果flash内为空，则赋值为1 2 3 4 5 6
void get_unlock_code(u8 *out_pwd){
	int i=0;
	read_flash(out_pwd,PWD_ADDR,PWD_LEN_6);
	if(is_pwd_legal(out_pwd,PWD_LEN_6) == FALSE){
		for(i=0;i<PWD_LEN_6;i++)
			out_pwd[i] = i+1;
	}
}

//6位数的开锁密码是否正确 
BOOL is_usual_pwd_ok(const int *pwd ,const int len){
	u8 lock_pwd[PWD_LEN_6]={0};
	int i=0;
	if(len != PWD_LEN_6)return FALSE;
	get_unlock_code(lock_pwd);
	for(i=0;i<PWD_LEN_6;i++){
		if(lock_pwd[i] != pwd[i])
			return FALSE;
	}
	return TRUE;
}

void lock(void){
	__disable_irq();
	LOCK_PIN_CYRLE();
	system_delay_ms(300);
	LOCK_PIN_RESET();
	__enable_irq();
}
int lock_judge(const int *src,int src_len){
	if(src_len != 0){
		return 2;
	}
	return 1;
}

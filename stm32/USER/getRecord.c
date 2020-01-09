//
//  getRecord.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "getRecord.h"
#define RECORD_MAX 85
//清除所有开锁记录
void clear_record(void){
	FLASH_ErasePage(RECORD_ADDR);
}
//判断记录是否合法
static BOOL is_record_legal(u8 *re){
	int i=0;
	if(re[1] != 1 && re[1] != 2)return FALSE;
	for(i=0;i<12;i++){
		if(re[i] == 255)return FALSE;
	}
	return TRUE;
}
//查询锁内还有多少条记录
static int rest_record_quality(void){
	
	u8 read[12] = {0};
	int i;
	for(i=0;i<RECORD_MAX;i++){
		read_flash(read,i*12+RECORD_ADDR,12);
		if(is_record_legal(read) == FALSE){
			return i;
		}
		memset(read,0,12);
	}
	return RECORD_MAX;
}

void write_recod(const int *pwd,int unlock_way){
	int i=0;
	int ack = 0;
	u8 write_buf[12]={0};
	u8 time[6]={0};
	u8 pwd_bcd[4]={0};
	u32 addr = RECORD_ADDR;
	ack=rest_record_quality();
	if(ack == RECORD_MAX){
		clear_record();
		ack = 0;
	}
	read_time(time);
	
	write_buf[0] = ack;//1
	write_buf[1] = unlock_way;//2
	for(i=0;i<6;i++)//3
		write_buf[2+i] = time[i];
	//4
	if(unlock_way == 1){
		for(i=0;i<3;i++)
		{
			pwd_bcd[i] = pwd[2*i]*10+pwd[2*i+1];
		}
		pwd_bcd[3] = 0;
	}else
	if(unlock_way == 2){
		for(i=0;i<4;i++)
		{
			pwd_bcd[i] = pwd[2*i]*10+pwd[2*i+1];
		}		
	}
	for(i=0;i<4;i++)
		write_buf[8+i] = pwd_bcd[i];
	addr = ack*12+RECORD_ADDR;
	write_flash(write_buf,addr,12);
}
void read_record(int *record){
	u8 read[12]={0};
	int i;
	u32 addr = RECORD_ADDR;
	int ack=rest_record_quality();
	if(ack>0){
		ack=ack-1;//剩下的条数，不包括本身
		addr = ack*12+RECORD_ADDR;
		read_flash(read,addr,12);
		for(i=0;i<12;i++){
			record[i] = read[i];
		}
		//erase
		for(i=0;i<12;i++){
			read[i] = 255;
		}
		write_flash(read,addr,12);
	}else{
		for(i=0;i<12;i++){
			record[i]=0;
		}
	}
}

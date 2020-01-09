//
//  disKey.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "disKey.h"
//1 enable 0 disable
static void enable_key(void){
	u8 d[2] = {1,1};
	write_flash(d,IS_LOCKNO_ENABLE_ADDR,2);
}
static void disable_key(void){
	u8 d[2] = {0,0};
	write_flash(d,IS_LOCKNO_ENABLE_ADDR,2);
}
BOOL is_key_enable(void){
	u8 d[2] = {0,0};
	read_flash(d,IS_LOCKNO_ENABLE_ADDR,2);
	if(d[0] == 1)return TRUE;
	if(d[0] == 255)return TRUE;//default value
	return FALSE;
}
int disable_key_handle(const int *src,const int len){
	if(len != 0)return -2;
	disable_key();
	return 1;
}
int enable_key_handle(const int *src,const int len){
	if(len!=0)return -2;
	enable_key();
	return 1;
}

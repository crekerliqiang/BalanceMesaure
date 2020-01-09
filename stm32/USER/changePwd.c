//
//  changePwd.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "changePwd.h"
#include "module.h"
//写入新密码
static BOOL write_new_pwd(const int *src,const int len){
	u8 src_u8[6]={0};
	u8 i=0;
	if(len == 6){
		for(i=0;i<6;i++)
			src_u8[i] = src[i];
		write_flash(src_u8,PWD_ADDR,len);
		return TRUE;
	}else{
		return FALSE;
	}
}

//判断密码是否合法 rule:必须是0-9的数
static BOOL is_pwd_legal(const int *src,const int len){
	int i;
	for(i=0;i<len;i++){
		if(src[i]>9)return FALSE;
		if(src[i]<0)return FALSE;
	}
	return TRUE;
}
int change_password_handle(const int *src,const int len){
	int oldpwd[6]={0};
	int newpwd[6]={0};
	u8 newpwd8[6] = {0};
	int i=0;
	if(len != 12){
		return -2;
	}
	for(i=0;i<6;i++){
		oldpwd[i] = src[i];
		newpwd[i] = src[i+6];
	}
	if(FALSE == is_usual_pwd_ok(oldpwd,6)){
		
		#if DEBUGE
		p1("change error pwd error\r\n ");
		#endif
		
		return 0;
	}else{
		if(FALSE == is_pwd_legal(newpwd,6)){
			return 2;
		}else{
			write_new_pwd(newpwd,6);
			
			#if DEBUGE
			p1("change ok ");
			#endif
			
			get_unlock_code(newpwd8);
			
			#if DEBUGE
			for(i=0;i<6;i++)p1("%d ",newpwd8[i]);p1("\r\n");
			#endif
			
			return 1;
		}
	}
}

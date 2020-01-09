//
//  checkKey.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "checkKey.h"
#include "disKey.h"
int check_key_handle(const int *src,const int len){
	if(len!=0)return -2;
	if(is_key_enable() == TRUE){
		return 0;
	}else{
		return 1;
	}
}

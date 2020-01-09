//
//  dump.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "dump.h"
int  dump_handle(const int *src,int len){
	if(len != 0)return -2;
	return get_battery();
}

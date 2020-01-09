//
//  shakeHands.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "shakeHands.h"
int shake_hands_handle(const int *src,const int len){
	u8 time[TIME_DATA_LEN]={0};
	int i;
	if(len != TIME_DATA_LEN){
		return -2;
	}else{
		for(i=0;i<TIME_DATA_LEN;i++){
			time[i] = src[i];
		}
		write_time(time);
		return 1;
	}
}

//
//  unlock.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "unlock.h"
#include "module.h"
void unlock(void){
	__disable_irq();
	LOCK_PIN_RE_CYCLE();
	system_delay_ms(300);
	LOCK_PIN_RESET();
	__enable_irq();
}
int unlock_judge(const int *pwd,const int pwd_len){
	if(pwd_len != 6){
		return -2;
	}else
	if(is_usual_pwd_ok(pwd ,pwd_len) == TRUE){
		return 1;
	}else{
		return 0;
	}
}

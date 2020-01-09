//
//  randomPwd.h
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright ? 2018年 李强. All rights reserved.
//

#ifndef randomPwd_h
#define randomPwd_h

#include <stdio.h>
#include "driver.h"


typedef enum{
	PWD_WRONG,
	PWD_OK_OVERTUE,
	PWD_OK,
	PWD_DEFAULT,
	PWD_OK_DISABLE,//密码正确但是被禁用
	PWD_NOTIMEDATA,//系统时钟失效
}PWD_STA;
extern PWD_STA judge_TemporaryPwd_legal(const int *pwd);
#endif /* randomPwd_h */

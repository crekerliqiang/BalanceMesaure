//
//  randomPwd.h
//  test
//
//  Created by ��ǿ on 2018/3/29.
//  Copyright ? 2018�� ��ǿ. All rights reserved.
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
	PWD_OK_DISABLE,//������ȷ���Ǳ�����
	PWD_NOTIMEDATA,//ϵͳʱ��ʧЧ
}PWD_STA;
extern PWD_STA judge_TemporaryPwd_legal(const int *pwd);
#endif /* randomPwd_h */

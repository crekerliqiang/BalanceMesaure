//
//  lock.h
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#ifndef lock_h
#define lock_h

#include <stdio.h>
#include "driver.h"
void lock(void);
extern BOOL is_usual_pwd_ok(const int *pwd ,const int len);
extern void get_unlock_code(u8 *pwd);
int lock_judge(const int *src,int src_len);
#endif /* lock_h */

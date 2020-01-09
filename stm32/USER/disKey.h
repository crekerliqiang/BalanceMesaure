//
//  disKey.h
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#ifndef disKey_h
#define disKey_h

#include <stdio.h>
#include "driver.h"
BOOL is_key_enable(void);
int disable_key_handle(const int *src,const int len);
int enable_key_handle(const int *src,const int len);
#endif /* disKey_h */

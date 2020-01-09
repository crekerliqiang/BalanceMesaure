//
//  passwordTool.h
//  arithmetic
//
//  Created by liqiang on 2018/4/25.
//  Copyright © 2018 李强. All rights reserved.
//

#ifndef passwordTool_h
#define passwordTool_h

#include <stdio.h>
#include "stm32f10x_it.h" 

void arrSpicialSubtraction(const int * num,const int *pwd,int *returnTime);
void getArrHashNum(const int *time,const int *pwd,int crcNumArr[3]);
void mixArrDe(const int * arr,int *out);
void mixArrDe10(const u8 * arr,int *out);
int hashCode(const int * Source, const int len);
#endif /* passwordTool_h */

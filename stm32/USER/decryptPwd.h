//
//  decryptPwd.h
//  arithmetic
//
//  Created by liqiang on 2018/4/25.
//  Copyright © 2018 李强. All rights reserved.
//

#ifndef decryptPwd_h
#define decryptPwd_h

#include <stdio.h>
#include "stm32f10x_it.h" 
void getTimeAndHash(const  int * keyInputPwd,const int * lockPwd,int *value) ;
void getTime(const int * keyInputPwd,const int* lockPwd,int *returnTime);
void getHash(const int * keyInputPwd,const int * lockPwd,int *returnHash) ;
int checkData(const int* keyInputPwd,const int * lockPwd,const int * realTime,int * deadTimeOut) ;
int checkDataV2_Father(const int* keyInputPwd,const int * lockPwd,const int * realTime,int * deadTimeOut);
extern int checkDataV2(const int* keyInputPwd,const int * lockPwd,const int * realTime,int * deadTimeOut);

extern void pArr(char *h ,const int * src,int len);


int checkTimeIsLegal(int * time);
#endif /* decryptPwd_h */

#ifndef _USART1_H_
#define _USART1_H_
#include "stm32f10x_it.h" 
#include "string.h"
#include "math.h"
#include "stdio.h"  
#include "stdarg.h"
#define DEBUGE 1
#if DEBUGE
	#define BT 460800
	#define p1 printf
	extern void pNln(const int *a,u8 len);
	extern void pln(const int *a,u8 len);
	extern void plnu8(const u8 *a,u8 len);
#else
	#define BT 9600
#endif


extern void debuge_usart1_init(u32 bound);
#endif

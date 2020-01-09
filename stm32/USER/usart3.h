#ifndef usart3_h
#define usart3_h

#include "stm32f10x_it.h" 
#include "string.h"
#include "math.h"
#include "stdio.h"  
#include "stdarg.h"
#define DEBUGE_LEN 100 
void usart3_debuge(u32 bound);
void enable_usart3(void);
void disable_usart3(void);
void p3cln(const char *p,u8 len);
#endif 


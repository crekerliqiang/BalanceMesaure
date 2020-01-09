#ifndef delay_h
#define delay_h
#include "stm32f10x_it.h" 
 
 
 
 
 
void system_delay_init(void);
void system_delay_us(u32 nus);
void system_delay_ms(u16 nms);
void system_delay_ms_divide10(u16 times);

#endif

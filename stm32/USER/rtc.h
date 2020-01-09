#ifndef __RTC_H
#define __RTC_H	    
#include "stm32f10x_it.h"

typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date; 
}_calendar_obj;					 
u8 rtc_init(void);
u32 time_to_SEC(const u8 *time);
void read_time(u8 *buffer_time);
void write_time(u8 *buffer_time);
#endif



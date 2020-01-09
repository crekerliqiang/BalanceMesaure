#ifndef wdg_h
#define wdg_h
#include "stm32f10x_it.h" 
typedef enum{
	_RESET,_3S,_6S,_11S,
}FEED_TIME;
extern void i_dog_feed(void);
extern void rst_sys(void);
#endif

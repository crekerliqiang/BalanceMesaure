#ifndef adc_h
#define adc_h
#include "stm32f10x.h"
void system_adc_init(void);

extern u16 get_voltage(void);
extern u8 get_battery(void);
#endif

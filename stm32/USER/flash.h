#ifndef flash_h
#define flash_h
#include "stm32f10x_it.h"


//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不使能;1，使能)
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址


/*******************************************************************************
*
*地址分配
*code size 48Kbytes 0x08000000 -- 0x0800BFFF
*memo size 16Kbytes 0x0800C000 -- 0x0800FFFF
*******************************************************************************/


#define ROM_BASE 0x0800C000
#define SCORE_SIZE 0x400//1KB

void read_flash(u8 *buffer,u32 area,u16 length);
void write_flash(const u8 *buffer,u32 area,u16 length);


#endif

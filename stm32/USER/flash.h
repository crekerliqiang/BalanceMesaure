#ifndef flash_h
#define flash_h
#include "stm32f10x_it.h"


//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0����ʹ��;1��ʹ��)
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ


/*******************************************************************************
*
*��ַ����
*code size 48Kbytes 0x08000000 -- 0x0800BFFF
*memo size 16Kbytes 0x0800C000 -- 0x0800FFFF
*******************************************************************************/


#define ROM_BASE 0x0800C000
#define SCORE_SIZE 0x400//1KB

void read_flash(u8 *buffer,u32 area,u16 length);
void write_flash(const u8 *buffer,u32 area,u16 length);


#endif

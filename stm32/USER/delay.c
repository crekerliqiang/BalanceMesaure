#include "delay.h"
static u8  fac_us=0;							   
static u16 fac_ms=0;						
void system_delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	fac_us=SystemCoreClock/8000000;			
	fac_ms=(u16)fac_us*1000;				
}								    
					   
void system_delay_us(u32 nus)
{		
	u32 temp;
	if(nus == 0)return;
	SysTick->LOAD=nus*fac_us; 					 
	SysTick->VAL=0x00;        				
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	
	SysTick->VAL =0X00;      					
}
void system_delay_ms(u16 nms)
{
	u32 temp;
	if(nms == 0)return;
	SysTick->LOAD=(u32)nms*fac_ms;			
	SysTick->VAL =0x00;						
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;       		
} 
/*
*用于超过1000ms的延时
*/
void system_delay_ms_divide10(u16 times)
{
	u16 i;
	times = times/10;
	for(i = 0;i<10;i++)
	{
		system_delay_ms(times);
	}
}

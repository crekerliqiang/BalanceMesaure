#include "gpio.h"
#include "delay.h"
#include "usart1.h"

//lockpin 
void lockpin_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);						
 GPIO_ResetBits(GPIOB,GPIO_Pin_9);						
}
//voice
void voice_init(void){
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					
 GPIO_ResetBits(GPIOA,GPIO_Pin_8);						
}
static void bibi(int nap,int times){
	int i=0;
	for(i=0;i<times;i++){
		VOICE_ON();
		system_delay_ms(nap);
		VOICE_SLIENCE();
		system_delay_ms(nap);
	}
	VOICE_SLIENCE();
}
void say(SAY len){
	switch(len){
		case DIDI:
			bibi(5,1);
		break;
		case KEY_DISABLE:
			bibi(1,100);
		break;
		case UNLOCK:
			bibi(1,100);
		break;
		case PWD_ERROR:
		case PWD_OVERTUE:	
			bibi(1,100);
			system_delay_ms(100);
			bibi(1,100);
			system_delay_ms(100);
			bibi(1,100);
		break;		
		default:break;
	}
}
//backled
void backled_init(void){
	//BACK
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOA,GPIO_Pin_11);						
//BAT
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_ResetBits(GPIOB,GPIO_Pin_13);						
}
//ble enable
void ble_enable_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOB,GPIO_Pin_12);						
}

#include "measure.h"
#include "usart1.h"
#include "delay.h"
	u32 rawData = 0;
	//clk A11
	//dat A12
	#define CLK GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)
	#define DAT GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)
	
	
	void clear(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	system_delay_ms(1000);
	
}
	void measure_init(void){
		
	GPIO_InitTypeDef GPIO_mode;
	 GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	GPIO_mode.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_mode.GPIO_Pin = GPIO_Pin_11;
	GPIO_mode.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_mode);

	GPIO_mode.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_mode.GPIO_Pin = GPIO_Pin_12;
	GPIO_mode.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_mode);
		
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOB,GPIO_Pin_8);	


	clear();
	
}
	


void getData(void) {
	int tmpClk;
	int tmpData;
	int i = 0;
	int data[24] = {0};
	
  rawData = 0;
  for (i = 0; i < 24; i++) {
      rawData = rawData << 1;
    tmpClk = CLK;
    while (tmpClk == 0) {
      tmpClk = CLK;
    }
    tmpData = DAT;
		data[i] = tmpData;

    if (tmpData == 1) {
      //rawData = rawData | 0x00800000;
			rawData++;
    }
    else {
      //rawData = rawData & 0xFF7FFFFF;
    }
		
    while (tmpClk == 1) {
      tmpClk = CLK;
    }
  }
	p1("data :%x--%ld\r\n",rawData,rawData);
	pln(data,24);
}

	
	

#include "wdg.h"
/*
	*初始化独立看门狗
	*prer:分频数:0~7(只有低3位有效!)
	*分频因子=4*2^prer.但最大值只能是256!
	*rlr:重装载寄存器值:低11位有效.
	*时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).作废
			*4,10000-->3S
			*7,5000-->6S
			*7,10000-->11.5S
*/
static void system_iwdg_init(u8 prer,u16 rlr){
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
	IWDG_SetPrescaler(prer); 
	IWDG_SetReload(rlr);
	IWDG_ReloadCounter();
	IWDG_Enable();
}
void iwdg_init(FEED_TIME t){
	switch(t){
		case _3S:system_iwdg_init(4,10000);break;
		case _6S:system_iwdg_init(7,5000);break;
		case _11S:system_iwdg_init(7,10000);break;
		case _RESET:system_iwdg_init(1,1);break;
		default:system_iwdg_init(7,5000);break;
	}	
}
//喂独立看门狗
void i_dog_feed(void){   
 	IWDG_ReloadCounter();//reload										   
}
void rst_sys(void){
	system_iwdg_init(1,1);
	while(1);
}


#include "wdg.h"
/*
	*��ʼ���������Ź�
	*prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
	*��Ƶ����=4*2^prer.�����ֵֻ����256!
	*rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
	*ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).����
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
//ι�������Ź�
void i_dog_feed(void){   
 	IWDG_ReloadCounter();//reload										   
}
void rst_sys(void){
	system_iwdg_init(1,1);
	while(1);
}


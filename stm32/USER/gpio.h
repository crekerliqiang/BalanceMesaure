#ifndef gpio_h
#define gpio_h
#include "stm32f10x_it.h"
#define LOCK_PIN_CYRLE()				{GPIO_ResetBits(GPIOB,GPIO_Pin_8);\
																GPIO_SetBits(GPIOB,GPIO_Pin_9);}
#define LOCK_PIN_RE_CYCLE()			 {GPIO_SetBits(GPIOB,GPIO_Pin_8);\
																GPIO_ResetBits(GPIOB,GPIO_Pin_9);}
#define LOCK_PIN_RESET()			 	{GPIO_ResetBits(GPIOB,GPIO_Pin_8);\
																GPIO_ResetBits(GPIOB,GPIO_Pin_9);}

#define VOICE_ON()			GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define VOICE_SLIENCE()		GPIO_ResetBits(GPIOA,GPIO_Pin_8);		

#define BACK_LED_EN			GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define BACK_LED_RESET	GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define BAT_LED_EN			GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define BAT_LED_RESET 	GPIO_ResetBits(GPIOB,GPIO_Pin_13)

void lockpin_init(void);

void voice_init(void);
typedef enum{
	DIDI,
	KEY_DISABLE,
	UNLOCK,
	PWD_ERROR,
	PWD_OVERTUE,
	
}SAY;
void say(SAY len);
	
void backled_init(void);


#define BLE_ENABLE()		GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define BLE_DISABLE() 	GPIO_ResetBits(GPIOB,GPIO_Pin_12)
void ble_enable_init(void);
#endif

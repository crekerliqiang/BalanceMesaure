#ifndef iic_h
#define iic_h

#include	"stm32f10x_it.h"
/************************触摸芯片数据读取驱动*****************************/
/*键盘数据读取的相关定义，PA0*/
#define	KeyBoard_RCC_GPIO					RCC_APB2Periph_GPIOA		//键盘对应的时钟
#define	KeyBoard_RCC_AFIO					RCC_APB2Periph_AFIO			//键盘对应的复用时钟
#define	KeyBoard_Port_Pin					GPIO_Pin_0							//键盘对应的Pin
#define	KeyBoard_Port							GPIOA										//键盘对应的端口
#define	KeyBoard_PortSource				GPIO_PortSourceGPIOA		//键盘对应的中断端口
#define touchKeyInt GPIO_ReadInputDataBit(KeyBoard_Port,KeyBoard_Port_Pin)//读取按键
enum{	
	TOUCHKEY_RELEASE = 0,//松开手
	TOUCHKEY_PRESS_DOWN = 1,//按下
};
#define SDA_IN()  {GPIOB->CRL&=0XFFFFFFF0;\
										GPIOB->CRL|=(u32)8<<0;}
#define SDA_OUT() {GPIOB->CRL&=0XFFFFFFF0;\
										GPIOB->CRL|=(u32)3<<0;}
#define IIC_SCL    PBout(1) //SCL
#define IIC_SDA    PBout(0) //SDA	 
#define READ_SDA   PBin(0)  //输入SDA //IIC所有操作函数

#define TOUCH_IIC_DELAY 4




										
										
										
u8 get_outkey_iic_data(void);
void get_outkey_iic_init(void);
void touch_key_read_init(void);
#endif

#ifndef iic_h
#define iic_h

#include	"stm32f10x_it.h"
/************************����оƬ���ݶ�ȡ����*****************************/
/*�������ݶ�ȡ����ض��壬PA0*/
#define	KeyBoard_RCC_GPIO					RCC_APB2Periph_GPIOA		//���̶�Ӧ��ʱ��
#define	KeyBoard_RCC_AFIO					RCC_APB2Periph_AFIO			//���̶�Ӧ�ĸ���ʱ��
#define	KeyBoard_Port_Pin					GPIO_Pin_0							//���̶�Ӧ��Pin
#define	KeyBoard_Port							GPIOA										//���̶�Ӧ�Ķ˿�
#define	KeyBoard_PortSource				GPIO_PortSourceGPIOA		//���̶�Ӧ���ж϶˿�
#define touchKeyInt GPIO_ReadInputDataBit(KeyBoard_Port,KeyBoard_Port_Pin)//��ȡ����
enum{	
	TOUCHKEY_RELEASE = 0,//�ɿ���
	TOUCHKEY_PRESS_DOWN = 1,//����
};
#define SDA_IN()  {GPIOB->CRL&=0XFFFFFFF0;\
										GPIOB->CRL|=(u32)8<<0;}
#define SDA_OUT() {GPIOB->CRL&=0XFFFFFFF0;\
										GPIOB->CRL|=(u32)3<<0;}
#define IIC_SCL    PBout(1) //SCL
#define IIC_SDA    PBout(0) //SDA	 
#define READ_SDA   PBin(0)  //����SDA //IIC���в�������

#define TOUCH_IIC_DELAY 4




										
										
										
u8 get_outkey_iic_data(void);
void get_outkey_iic_init(void);
void touch_key_read_init(void);
#endif

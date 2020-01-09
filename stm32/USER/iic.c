#include "iic.h"
#include "delay.h"
/************************����оƬ���ݶ�ȡ����*****************************/
void get_outkey_iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1); 	//
}
//����оƬ��ƽ�źų�ʼ��
void touch_key_read_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(KeyBoard_RCC_GPIO,ENABLE);//ʹ��PORTʱ��
	GPIO_InitStructure.GPIO_Pin  = KeyBoard_Port_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//-------
 	GPIO_Init(KeyBoard_Port, &GPIO_InitStructure);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	system_delay_us(TOUCH_IIC_DELAY);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	system_delay_us(TOUCH_IIC_DELAY);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	system_delay_us(TOUCH_IIC_DELAY);	
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	system_delay_us(TOUCH_IIC_DELAY);
	IIC_SCL=1; 
	system_delay_us(TOUCH_IIC_DELAY);		
	IIC_SDA=1;//����I2C���߽����ź�
	system_delay_us(TOUCH_IIC_DELAY);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ɹ�
//        0������Ӧ��ʧ��
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA=1;
	system_delay_us(1);
	SDA_IN();      //SDA����Ϊ����  	
//	IIC_SCL=1;
	system_delay_us(TOUCH_IIC_DELAY);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SDA_OUT();//sda�����
			system_delay_us(TOUCH_IIC_DELAY);
			//IIC_Stop();
			return 0;//ʧ��
		}
	}
	SDA_OUT();
	IIC_SCL = 1;
	system_delay_us(TOUCH_IIC_DELAY);system_delay_us(TOUCH_IIC_DELAY);
	IIC_SCL=0;//ʱ�����0
	system_delay_us(TOUCH_IIC_DELAY);system_delay_us(TOUCH_IIC_DELAY);	
	return 1; //�ɹ� 
}	  
u8 IIC_Send_Byte(u8 ch)  
{
    u8 t;
		t=8;
	SDA_OUT(); 	
		while(t--)
		{
			if(ch&0x80)
			{
				IIC_SDA=1;
			}else
			{
				IIC_SDA=0;				
			}
			ch<<=1;
			system_delay_us(TOUCH_IIC_DELAY);
			IIC_SCL=1;
			system_delay_us(TOUCH_IIC_DELAY);
			system_delay_us(TOUCH_IIC_DELAY);
			IIC_SCL = 0;
			system_delay_us(TOUCH_IIC_DELAY);		
		}
		return (IIC_Wait_Ack());
 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(u8 SendAck)
{
	u8 i,ddata;
	IIC_SDA=1;
	SDA_IN();//SDA����Ϊ����
	i=0x08;
	ddata=0x00;
  while(i-- )
	{
		IIC_SCL=1; 
		ddata<<=1; 
		system_delay_us(TOUCH_IIC_DELAY);
		if(READ_SDA)
		{
			ddata|=0x01;
		}
		IIC_SCL=0;
		system_delay_us(TOUCH_IIC_DELAY);
		}	
		SDA_OUT();
		if(SendAck)
		{
			IIC_SDA = 1;
		}else
		{
			IIC_SDA = 0;
		}
		system_delay_us(TOUCH_IIC_DELAY);
		IIC_SCL=1;
		system_delay_us(TOUCH_IIC_DELAY);
		IIC_SCL=0;
		system_delay_us(TOUCH_IIC_DELAY);system_delay_us(TOUCH_IIC_DELAY);
		return ddata;

}
u8 get_outkey_iic_data(void)
{
    u8 tc_Key_Data_08;
		u8 tc_Key_Data_09;
		u16 result;
		u16 got;
    IIC_Start();
    IIC_Send_Byte(0xA0);
    IIC_Send_Byte(0x08);
    IIC_Stop(); 
    
    IIC_Start();
    IIC_Send_Byte(0xA1);
    tc_Key_Data_08 = IIC_Read_Byte(0x01);
    IIC_Stop();	
///////////////////////////////////////////		
    IIC_Start();
    IIC_Send_Byte(0xA0);
    IIC_Send_Byte(0x09);
    IIC_Stop(); 
    
    IIC_Start();
    IIC_Send_Byte(0xA1);
    tc_Key_Data_09 = IIC_Read_Byte(0x01);
    IIC_Stop();		
///////////////////////////////////////////		
		result = (tc_Key_Data_09<<8) + tc_Key_Data_08;
		switch (result){
			case 0x0001:got = 11;break;
			case 0x0002:got = 7;break;
			case 0x0004:got = 4;break;
			case 0x0008:got = 1;break;
			
			case 0x0010:got = 0;break;
			case 0x0020:got = 8;break;
			case 0x0040:got = 5;break;
			
			case 0x0080:got = 2;break;
			case 0x0100:got = 3;break;
			case 0x0200:got = 6;break;
			case 0x0400:got = 12;break;
			case 0x0800:got = 9;break;
			default:got = 0xff;break;
    }
    return got;
}



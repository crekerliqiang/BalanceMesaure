#include "adc.h"
////////////是否已经获取电池电量，//////////////////////////////////////
//电池电压是否已经获取
//1：已经获取电池电量值；0：没有获取电池电量值
static u8 has_getbattery=0;
/*
	功能：给bat_set_value赋值
*/
void bat_set_value(u8 val){
	has_getbattery=val;
}
/*
	功能：获取has_getbattery的值
*/
u8 bat_get_value(void){
	return has_getbattery;
}

///////////////////////////////////////////////////////////
void  system_adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//6分频 12M                     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;        //这是使用连续扫描模式时   
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_Init(ADC1, &ADC_InitStructure); 
	ADC_Cmd(ADC1, ENABLE);	
	ADC_ResetCalibration(ADC1);	
	while(ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);	
	while(ADC_GetCalibrationStatus(ADC1));	
	bat_set_value(0);//没有采集电压
}
/***************************************************************/
u16 Get_Adc(u8 ch)   
{
  ADC_RegularChannelConfig(ADC1,ch,1, ADC_SampleTime_239Cycles5 );  //  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	return ADC_GetConversionValue(ADC1);	
}
//采集总时间=21*times us
/*
*函数功能：采集模拟信号
*输入：采集通道，采集总时间（单位：us）
*输出：模拟电压值
*/
u16 Get_Adc_Average(u8 ch,u32 time_us)
{
	u32 temp_val=0;
	u32 t;
	time_us = time_us/21;//将采集时间转化为采集次数
	for(t=0;t<time_us;t++)
	{
		temp_val+=Get_Adc(ch);
	}
	t = temp_val/time_us;
	return t;
} 	 
//获取电池电压 单位：mv
u16 get_battery_votage(void)
{
	u16 buffer = 0x0000;
	u8 i;
	u32 num = 0x00;
	u8 tms = 2;
	for(i = 0;i<tms;i++)
	{
		buffer = Get_Adc_Average(ADC_Channel_1,50000);//50ms,获取的是AD采集值
		num = num + buffer;
	}
	buffer = num/tms;
	buffer = (buffer*3300) / 4096;
	return buffer;
}

//获取电池电量
//返回值为20-99 
/*
*获取电池电量
*输出：电池电量 1--100
*返回值：电压值
*/
u16 getbattery(u8 *voltage)
{
	u16 buffer=0,returnValue=0;
	u8 i;
	u8 dump;
	u16 data[100] = {
		6391 ,6214 ,6096 ,6008 ,5998 ,5900 ,5880 ,5821 ,5801 ,5801 ,
		5762 ,5752 ,5733 ,5703 ,5693 ,5693 ,5674 ,5664 ,5664 ,5634 ,
		5624 ,5585 ,5575 ,5565 ,5536 ,5536 ,5526 ,5516 ,5506 ,5487 ,
		5477 ,5477 ,5467 ,5457 ,5428 ,5418 ,5418 ,5388 ,5369 ,5359 ,
		5359 ,5359 ,5349 ,5329 ,5310 ,5310 ,5290 ,5270 ,5270 ,5270 ,
		5270 ,5261 ,5241 ,5231 ,5221 ,5211 ,5211 ,5211 ,5202 ,5202 ,
		5192 ,5192 ,5192 ,5172 ,5172 ,5152 ,5143 ,5133 ,5123 ,5113 ,
		5103 ,5103 ,5093 ,5093 ,5084 ,5074 ,5054 ,5054 ,5025 ,5000 ,
		4950 ,4900 ,4850 ,4800 ,4750 ,4700 ,4650 ,4600 ,4550 ,4500 ,
		4450 ,4400 ,4350 ,4300 ,4250 ,4200 ,4150 ,4100 ,4050 ,4000 ,
};
	buffer = get_battery_votage();//buffer表示的电压值
	buffer = buffer*147/47 + 300;//300mV表示二极管压降
	for(i=0;i<100;i++)
	{
		if(buffer>data[i])
		{
			break;
		}	
	}
	returnValue=buffer;
	dump = 100-i;
	if(dump>100)dump=100;
	if(dump==0)dump=1;
	*voltage=dump;
	bat_set_value(1);//表示已经读取了电压
	return returnValue;
}

/////////////////////获取电池电量，外部调用方法/////////////////////////////
static u16 private_voltage=0;
static u8 private_battery=0;
//获取电量
u8 get_battery(void)
{
	if(bat_get_value()==0)
	{
		private_voltage=getbattery(&private_battery);
	}
	return private_battery;
}
//获取电压
u16 get_voltage(void)
{
	if(bat_get_value()==0)
	{
		private_voltage=getbattery(&private_battery);
	}
	return private_voltage;
}



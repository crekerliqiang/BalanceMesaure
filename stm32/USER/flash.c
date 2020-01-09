#include "flash.h"

static u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
//读取一个字节

//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
static void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
static void STMFLASH_Read(u16 *pBuffer,u32 ReadAddr,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
u16 STMFLASH_BUF[SCORE_SIZE/2];//最多是2K字节
static void STMFLASH_Write(u16 *pBuffer,u32 WriteAddr,u16 NumToWrite)
{
	u32 secpos = 0;	   //扇区地址
	u16 secoff = 0;	   //扇区内偏移地址(16位字计算)
	u16 secremain = 0; //扇区内剩余地址(16位字计算)	   
 	u16 i = 0;    
	u32 offaddr = 0;   //去掉0X08000000后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
		return;//非法地址
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/SCORE_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%SCORE_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=SCORE_SIZE/2-secoff;		//扇区剩余空间大小   
	if(secremain<NumToWrite)
	{
	//写此扇区	
		//read
		STMFLASH_Read(STMFLASH_BUF,secpos*SCORE_SIZE+STM32_FLASH_BASE,SCORE_SIZE/2);//读出整个扇区的内容
		//combine
		for(i=0;i<secremain;i++)//复制
		{
			STMFLASH_BUF[i+secoff]=pBuffer[i];	  
		}
		//erase_page
		FLASH_ErasePage(secpos*SCORE_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		//write
		STMFLASH_Write_NoCheck(secpos*SCORE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,SCORE_SIZE/2);//写入整个扇区  
	
		
		//写下一个扇区
		//combine
		for(i=0;i<NumToWrite - secremain;i++)//获取剩余的数据
		{
			STMFLASH_BUF[i]=pBuffer[i+secremain];	  //从0开始
		}
		//erase_page
		FLASH_ErasePage((secpos+1)*SCORE_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		//write
		STMFLASH_Write_NoCheck((secpos+1)*SCORE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,SCORE_SIZE/2);
	}else
	{
		//read
		STMFLASH_Read(STMFLASH_BUF,secpos*SCORE_SIZE+STM32_FLASH_BASE,SCORE_SIZE/2);//读出整个扇区的内容
		//combine
		for(i=0;i<NumToWrite;i++)//复制
		{
			STMFLASH_BUF[i+secoff]=pBuffer[i];	  
		}
		//erase_page
		FLASH_ErasePage(secpos*SCORE_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		//write
		STMFLASH_Write_NoCheck(secpos*SCORE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,SCORE_SIZE/2);//写入整个扇区  
	}
	FLASH_Lock();//上锁
}

/*********************************/
//工具类

//不可超过128位
void read_flash(u8 *buffer,u32 area,u16 length)
{
	u16 *buf;
	u8 lenbuf;
	u16 i = 0;
	u16 j = 0;
	u16 bufstr[128] = {0x00};
	buf = bufstr;
	lenbuf = length + length%2;//变成最接近的偶数
	area = area + area%2;////变成最接近的偶数
	__disable_irq();
	STMFLASH_Read(buf,area,lenbuf/2);//16位读取，长度只要1/2
	__enable_irq();
	while(i<length)
	{
		buffer[i] = bufstr[j] % 256;
		if((length%2==1)&&(length == (i+1)))//为奇数，最后一位不赋值
		{
			break;
		}else
		{
			buffer[i+1] = bufstr[j] / 256;
		}
		i+=2;
		j+=1;
	}
}
void write_flash(const u8 *buffer,u32 area,u16 length)
{ 
	u16 *buf;
	u16 i = 0;
	u16 j = 0;
	u16 bufstr[128] = {0x00};
	if(area<ROM_BASE)return;
	buf = bufstr;
	while(i<length)
	{
		bufstr[j] = buffer[i+1]*256+buffer[i];
		j+=1;
		i+=2;
	}
	length = length + length%2;//变成最接近的偶数
	area = area + area%2;//变成最接近的偶数
	//16位写入，所以定义u8数组时必须为偶数
__disable_irq();
	STMFLASH_Write(buf,area,length/2);//16位写入，长度只要1/2
__enable_irq();
}







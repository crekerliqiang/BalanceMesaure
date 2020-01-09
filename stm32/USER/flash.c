#include "flash.h"

static u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
//��ȡһ���ֽ�

//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
static void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
static void STMFLASH_Read(u16 *pBuffer,u32 ReadAddr,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
u16 STMFLASH_BUF[SCORE_SIZE/2];//�����2K�ֽ�
static void STMFLASH_Write(u16 *pBuffer,u32 WriteAddr,u16 NumToWrite)
{
	u32 secpos = 0;	   //������ַ
	u16 secoff = 0;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain = 0; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i = 0;    
	u32 offaddr = 0;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
		return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/SCORE_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%SCORE_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=SCORE_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(secremain<NumToWrite)
	{
	//д������	
		//read
		STMFLASH_Read(STMFLASH_BUF,secpos*SCORE_SIZE+STM32_FLASH_BASE,SCORE_SIZE/2);//������������������
		//combine
		for(i=0;i<secremain;i++)//����
		{
			STMFLASH_BUF[i+secoff]=pBuffer[i];	  
		}
		//erase_page
		FLASH_ErasePage(secpos*SCORE_SIZE+STM32_FLASH_BASE);//�����������
		//write
		STMFLASH_Write_NoCheck(secpos*SCORE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,SCORE_SIZE/2);//д����������  
	
		
		//д��һ������
		//combine
		for(i=0;i<NumToWrite - secremain;i++)//��ȡʣ�������
		{
			STMFLASH_BUF[i]=pBuffer[i+secremain];	  //��0��ʼ
		}
		//erase_page
		FLASH_ErasePage((secpos+1)*SCORE_SIZE+STM32_FLASH_BASE);//�����������
		//write
		STMFLASH_Write_NoCheck((secpos+1)*SCORE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,SCORE_SIZE/2);
	}else
	{
		//read
		STMFLASH_Read(STMFLASH_BUF,secpos*SCORE_SIZE+STM32_FLASH_BASE,SCORE_SIZE/2);//������������������
		//combine
		for(i=0;i<NumToWrite;i++)//����
		{
			STMFLASH_BUF[i+secoff]=pBuffer[i];	  
		}
		//erase_page
		FLASH_ErasePage(secpos*SCORE_SIZE+STM32_FLASH_BASE);//�����������
		//write
		STMFLASH_Write_NoCheck(secpos*SCORE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,SCORE_SIZE/2);//д����������  
	}
	FLASH_Lock();//����
}

/*********************************/
//������

//���ɳ���128λ
void read_flash(u8 *buffer,u32 area,u16 length)
{
	u16 *buf;
	u8 lenbuf;
	u16 i = 0;
	u16 j = 0;
	u16 bufstr[128] = {0x00};
	buf = bufstr;
	lenbuf = length + length%2;//�����ӽ���ż��
	area = area + area%2;////�����ӽ���ż��
	__disable_irq();
	STMFLASH_Read(buf,area,lenbuf/2);//16λ��ȡ������ֻҪ1/2
	__enable_irq();
	while(i<length)
	{
		buffer[i] = bufstr[j] % 256;
		if((length%2==1)&&(length == (i+1)))//Ϊ���������һλ����ֵ
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
	length = length + length%2;//�����ӽ���ż��
	area = area + area%2;//�����ӽ���ż��
	//16λд�룬���Զ���u8����ʱ����Ϊż��
__disable_irq();
	STMFLASH_Write(buf,area,length/2);//16λд�룬����ֻҪ1/2
__enable_irq();
}







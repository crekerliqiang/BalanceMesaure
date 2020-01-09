//

//
//  Created by 李强 on 2017/7/14.
//  Copyright  2017年 李强. All rights reserved.
//

#include "systemString.h"
#include "string.h"
u8 two_str_cmp(const char *str1,const char *str2,\
	const int afirstChar,const int bfirstChar,const int length)
{
    int i = 0;
    u8 cmpflag = 0;
    for(i = 0;i<length;i++)
    {
        if(str1[i + afirstChar] != str2[i + bfirstChar])
        {
            cmpflag = 0;
            break;
        }
        else
        {
            cmpflag = 1;
        }
    }
    return cmpflag;
}

//1:两个数组相同
u8 two_hex_cmp(const u8 *str1,const u8 *str2,\
	const u8 afirstChar,const u8 bfirstChar,const u8 length)
{
    u8 i = 0;
    u8 cmpflag = 0;
    for(i = 0;i<length;i++)
    {
        if(str1[i + afirstChar] != str2[i + bfirstChar])
        {
            cmpflag = 0;
            break;
        }
        else
        {
            cmpflag = 1;
        }
    }
    return cmpflag;
}

void my_strncpy(char *aStr1,const int first,const char *bStr2,\
	const int bfirst,const int length)
{
    int i;
    for(i = 0;i<length;i++)
    {
        aStr1[i+first] = bStr2[i+bfirst];
    }
}
void my_hexncpy(u8 *aStr1,const u8 first,const u8 *bStr2,\
	const u8 bfirst,const u8 length)
{
    u8 i;
    for(i = 0;i<length;i++)
    {
        aStr1[i+first] = bStr2[i+bfirst];
    }
}
//网关协议
/*
*函数功能：把0x30改为0x00......
*输入：	u8 *in 
*				u8 *out
*				u8 len:长度
*输出：	无
 */
void u8_to_u8(const u8 *in,u8 *out,u8 len)
{
	u8 i;
	for(i=0;i<len;i++){
		out[i]=in[i]-0x30;
	}
}

/*
*函数功能：比较两个时间数据（time1-time2）
*输入：	u8 *time1:6字节的时间数组 
*				u8 *time2:6字节的时间数组
*				u8 len:比较的长度
*输出：	无
*返回：1：time1时间超前；0：两个时间一样；-1：time2超前
  */
int two_time_cmp(const u8 *time1,const u8 *time2,u8 len)
{
	u8 i;
	int returnFlag = 0;
	if(1 == two_hex_cmp(time1,time2,0,0,len))
		returnFlag = 0;
	for(i=0;i<len;i++)
	{
		if(time1[i] > time2[i])
		{
			returnFlag = 1;
			break;
		}else
		if(time1[i] < time2[i])
		{
			returnFlag = (-1);
			break;
		}
	}
	return returnFlag;
}

//函数功能:把数组从0开始长度为len的数据设置为set_data
void set_hex_str(u8 *str,u8 len,u8 set_data)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		str[i] = set_data;
	}
}

/*
*函数功能：将9位的压缩密码还原为12的密码(前3位还原为6位，后六位不变)
*输入：u8 *pwd_hex
*输出：u8 *pwd:十六进制数组
*示例：输入：u8 pwd_hex[]={0x12,0x34,0x56,0x17,0x7,0x9,0x12,0x12,0x12};输出：u8 pwd[]={0x1,0x2,0x3,0x4,0x5,0x6,0x17,0x7,0x9,0x12,0x12,0x12};
*说明：pwd_to_bcd(u8 *pwd,u8 *pwd_hex)的逆运算
*返回：1:正常
*/
u8 pwd_bcd_to_pwd(const u8 *pwd_hex, u8 *pwd)
{
	u8 i, j = 0;
	for (i = 0; i < 3; i++)
	{
		if(pwd_hex[i]==0xff)
		{
			pwd[j] = 0xff;
			j += 1;
			pwd[j] = 0xff;
			j += 1;			
		}else
		{
			pwd[j] = pwd_hex[i] / 10;
			j += 1;
			pwd[j] = pwd_hex[i] % 10;
			j += 1;
		}
	}		
	j=6;
	for (i = 3; i < 9; i++)
	{
		pwd[j] = pwd_hex[i];
		j += 1;
	}
	return 1;
}
/*******************************************************************************
 * Function Name  : GetCRC8
 * Description    : 单字节查表计算CRC8(UP_左移正序 ).
 * Input          : *pSource: 数据头指针  len:数据长度
 * Output         : None
 * Return         : CRC8码
 *******************************************************************************/
int GetCRC8(int *pSource, int len)
{
	int sum=0;
	int i=0;
	for(i=0;i<len;i++){
		sum = sum ^ pSource[i];
	}
   return  sum%256;
}
/*
 *函数功能：将两个int的数据拆分为数组,int 数据为六位数，数组为12位
 *输入：   int deadtime:时间时间的日时分；
          int PWD:锁对应的时效密码
 *输出：u8 arr_time_pwd[12]
 *返回值：无
 */
void double_int_to_arr(const int deadtime,const int pwd,u8 *arr_time_pwd)
{
	arr_time_pwd[0]=deadtime/100000;
	arr_time_pwd[1]=(deadtime%100000)/10000;
	arr_time_pwd[2]=(deadtime%10000)/1000;
	arr_time_pwd[3]=(deadtime%1000)/100;
	arr_time_pwd[4]=(deadtime%100)/10;
	arr_time_pwd[5]=deadtime%10;
	arr_time_pwd[6]=pwd/100000;
	arr_time_pwd[7]=(pwd%100000)/10000;
	arr_time_pwd[8]=(pwd%10000)/1000;
	arr_time_pwd[9]=(pwd%1000)/100;
	arr_time_pwd[10]=(pwd%100)/10;
	arr_time_pwd[11]=pwd%10;
}
/*
 *函数功能：将int数据转化为char类型的字符串
 *输入：	const int sor：
 *输出：	char *out ：字符串
 *返回：无
 *输入：int sor = 123；输出：char *out="123";
 */

void int_to_char(const int sor,char *out)
{
    u8 i=0;
    u8 len=0;
		if(sor<0)return;
    for(i=0;i<32;i++)//判断sor是i位数
    {
        if(sor/ten_pow(i)==0)
            break;
    }
    len=i;
    for(i=0;i<len;i++)
    {
        out[i]=(sor%ten_pow(len-i))/ten_pow(len-i-1);
        out[i]+=0x30;
    }
    out[len]=0;
}
/*
 *函数功能：返回10的i次方
*/
u32 ten_pow(u8 i)
{
    u32 num=1;
    u8 j=0;
    for(j=0;j<i;j++)
    {
        num=num*10;
    }
    return num;
}

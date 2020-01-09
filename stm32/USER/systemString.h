#ifndef _SYSTEMSTRING_H_
#define _SYSTEMSTRING_H_
#include "stm32f10x_it.h"
#include "rc4.h"

extern u8 two_str_cmp(const char *str1,const char *str2,const int afirstChar,const int bfirstChar,const int length);
extern u8 two_hex_cmp(const u8 *str1,const u8 *str2,const u8 afirstChar,const u8 bfirstChar,const u8 length);
extern void my_strncpy(char *aStr1,const int first,const char *bStr2,const int bfirst,const int length);
extern void my_hexncpy(u8 *aStr1,const u8 first,const u8 *bStr2,const u8 bfirst,const u8 length);
extern void u8_to_u8(const u8 *in,u8 *out,u8 len);
extern int two_time_cmp(const u8 *time1,const u8 *time2,u8 len);
extern void set_hex_str(u8 *str,u8 len,u8 set_data);
extern u8 pwd_bcd_to_pwd(const u8 *pwd_hex, u8 *pwd);
extern int GetCRC8(int *pSource, int len);
extern void double_int_to_arr(const int deadtime,const int pwd,u8 *arr_time_pwd);
extern void int_to_char(const int sor,char *out);
extern u32 ten_pow(u8 i);

#endif

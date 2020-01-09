//
//  ble.c
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#include "ble.h"
#define HEADER 1
#define keyLen24 24
static BLE_STA ble_sta = BLE_STA_DEFAULT;
static u32 ble_sta_tick = 0;
static BLE_CMD ble_cmd = API_DEFAULT;
BLE_STA get_bleSta(void){
	return ble_sta;
}
void set_bleSta(BLE_STA s){
	ble_sta=s;
}
void set_bleStaTickSelfAdd1(void){
	ble_sta_tick+=1;
}
void set_bleStaTick(u32 s){
	ble_sta_tick=s;
}
u32 get_bleStaTick(void){
	return ble_sta_tick;
}
BLE_CMD get_bleCmd(void){
	return ble_cmd;
}


//send ble data by usart3
/*
protocol frame:
	header:1,
	index:1,
	length:5,
	cmd:1
	data:1 2 10 3 4 //"12.34"
	crc:123
*/
static int get_index(void){
	return 1;
}
void get_key_str(int index,int *key){
	int i;
	int a[] = {
			1,2,3,4,5,6,7,8,9,0,
			1,2,3,4,5,6,7,8,9,0,
			1,2,3,4,5,6,7,8,9,0,
			1,2,3,4,5,6,7,8,9,0,
			1,2,3,4,5,6,7,8,9,0,
			1,2,3,4,5,6,7,8,9,0,
			1,2,3,4,5,6,7,8,9,0,
			1,2,3,4,5,6,7,8,9,0,	
	};
	for(i=0;i<keyLen24;i++){
		key[i] = a[(i+index)%keyLen24];
	}
}
void ble_send_data(const BLE_CMD cmd,const int *data,const int data_len)
{
	char header=0,key_index=0,length=0;
	int sendBuf[40]={0};
	char send_buf[40]={0};
	int data_clear_text[40]={0},data_cipher_text[40]={0};
	int key[24]={0};
	int crc=0;
	int pack_len_noneCrc=0;
	int arr_index=0;
	int i=0;
	
	header = HEADER;
	sendBuf[arr_index]=header;arr_index+=1;
	
	key_index=get_index();
	sendBuf[arr_index]=key_index;arr_index+=1;
	
	length = data_len;//
	pack_len_noneCrc = length + 4;
	
	sendBuf[arr_index]=length;arr_index+=1;
	//加密数据
	data_clear_text[0]=cmd;
	for(i=0;i<length;i++){
		data_clear_text[i+1] = data[i];
	}
	get_key_str(get_index(),key);
	HloveyRC4(data_clear_text,key,1+length,data_cipher_text);
	
	for(i=0;i<length+1;i++){
		sendBuf[arr_index+i]=data_cipher_text[i];
	}arr_index+=(length+1);
	
	crc=GetCRC8(sendBuf,pack_len_noneCrc);
	sendBuf[arr_index] = crc;arr_index+=1;
	
	for(i=0;i<pack_len_noneCrc+1;i++){
		send_buf[i]= sendBuf[i];
	}
	p3cln(send_buf,pack_len_noneCrc+1); 
	set_bleSta(SENT_DATA);
	ble_cmd = cmd;
	
	
	
	#if DEBUGE
	p1("ble ack ");
	for(i=0;i<pack_len_noneCrc+1;i++){
		p1("%d  ",send_buf[i]);
	}p1("\r\n");	
	#endif
	
	

}
/*
 *函数功能：获取蓝牙的数据，接收完成后，解密出应用层的cmd和data
 *返回：0，没有 ch 字符；1，拆分成功
 */
BOOL ble_rec_data(const char *rec_byte,BLE_CMD *cmd,int *data_out,int *data_out_len)
{
	int key[24]={0};
	int none_crc_len=0;
	int crc_re=0;
	int cipher_text_len = 0;
	int i=0;
	int data_clear_text[40]={0},data_cipher_text[40]={0},rec_byte_buf[40]={0};
	set_bleSta(RECVEVED_DATA);
	if(rec_byte[0]!=HEADER){
		return FALSE;
	}
	none_crc_len = 4+rec_byte[2];//4表示header index cmd length
	for(i=0;i<none_crc_len;i++){
		rec_byte_buf[i]=rec_byte[i];
	}
	crc_re=GetCRC8(rec_byte_buf,none_crc_len);
	if(crc_re != rec_byte[none_crc_len]){
		return FALSE;
	}
	cipher_text_len = rec_byte[2]+1;
	for(i=0;i<cipher_text_len;i++){
		data_cipher_text[i]=rec_byte_buf[i+3];
	}
	get_key_str(rec_byte[1],key);
	HloveyRC4(data_cipher_text,key,cipher_text_len,data_clear_text);
	*cmd = (BLE_CMD)data_clear_text[0];
	for(i=0;i<cipher_text_len-1;i++){
		data_out[i]=data_clear_text[1+i];
	}
	*data_out_len = cipher_text_len-1;
	
	
	#if DEBUGE
	p1("cmd %d \r\n",*cmd);
	p1("cmd    ");
	pln(data_out,*data_out_len);
	#endif
	
	
	return TRUE;
}

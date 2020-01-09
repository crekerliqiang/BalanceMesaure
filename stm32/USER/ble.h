//
//  ble.h
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright © 2018年 李强. All rights reserved.
//

#ifndef ble_h
#define ble_h

#include <stdio.h>
#include "driver.h"

typedef enum{
	API_DEFAULT=0,
	API_P_UNLOCK = 1,
	API_P_LOCK = 2,
	API_P_PASSWD = 3,
	API_P_RECORD = 4,
	API_P_DUMP = 5,
	API_P_DIS_KEY = 6,
	API_P_CHECK_KEY = 7,
	API_P_SHAKE_HAND = 8,
	API_P_EN_KEY = 9,
}BLE_CMD;
typedef enum{
	BLE_STA_DEFAULT,
	SENT_DATA,
	RECVEVED_DATA,
}BLE_STA;
#define BLE_ACK_TIME_OUT 1500
#define BLE_REC_BUF_LEN 48
BLE_STA get_bleSta(void);
void set_bleSta(BLE_STA s);
void set_bleStaTickSelfAdd1(void);
u32 get_bleStaTick(void);
void set_bleStaTick(u32 s);
BLE_CMD get_bleCmd(void);


void ble_send_data(const BLE_CMD cmd,const int *data,const int data_len);
BOOL ble_rec_data(const char *rec_byte,BLE_CMD *cmd,int *data_out,int *data_out_len);
#endif /* ble_h */

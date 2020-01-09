#include "bleapi.h"
#include "interrupt.h"
extern userTask user;
void handle_ble_data(BLE_CMD cmd,const int *src,const int src_len){
	int data=0;
	int record_data[12] = {0};
	/*
	 *3 steps
	 *	1.judge 2.handle 3.ack
	*/
	switch(cmd){
		case API_P_UNLOCK:
			data = unlock_judge(src,src_len);//1
			if(data == 0)user.is_ble_check_error = TRUE;//2
			if(data == 1)user.is_ble_check_ok = TRUE;
			ble_send_data(API_P_UNLOCK,&data,1);//3
		break;
		case API_P_LOCK:
			data = lock_judge(src,src_len);
			if(data == 1)
				user.is_ble_lock_ = TRUE;
			ble_send_data(API_P_LOCK,&data,1);
		break;
		case API_P_PASSWD:
			data = change_password_handle(src,src_len);
			ble_send_data(API_P_PASSWD,&data,1);
		break;
		case API_P_RECORD:
			read_record(record_data);
			ble_send_data(API_P_RECORD,record_data,12);
		break;
		case API_P_DUMP:
			data = dump_handle(src,src_len);
			ble_send_data(API_P_DUMP,&data,1);
		break;
		case API_P_DIS_KEY:
			data = disable_key_handle(src,src_len);
			ble_send_data(API_P_DIS_KEY,&data,1);
		break;
		case API_P_CHECK_KEY:
			data = check_key_handle(src,src_len);
			ble_send_data(API_P_CHECK_KEY,&data,1);
		break;
		case API_P_SHAKE_HAND:
			data = shake_hands_handle(src,src_len);
			ble_send_data(API_P_SHAKE_HAND,&data,1);
		break;  
		case API_P_EN_KEY:
			data = enable_key_handle(src,src_len);
			ble_send_data(API_P_EN_KEY,&data,1);			
		break;
		default:break;
	}
}





#ifndef interrupt_h
#define interrupt_h
#include "module.h"

extern u32 get_systemTick(void);
extern void reset_systemTick(void);
extern void set_systemTick(u32 t);

#define BUTTON_PWD_LEN 100
#define B_DEFAULT 20
typedef struct{
	BOOL is_handle_key;
	BOOL read_key_en;
	u8 key_value;
	u8 button_index;
	BOOL is_check_error;
	BOOL is_key_check_ok;//密码开锁成功
	int buttonData[BUTTON_PWD_LEN];
	BOOL is_check_overtue;
	BOOL is_handle_ble_data;
	BOOL is_ble_check_ok;
	BOOL is_ble_check_error;
	BOOL is_ble_lock_;
	BOOL is_auto_lock;//是否自动关锁
	int  is_auto_lock_counter;
	
	BOOL is_shine_back_led;
	int is_shine_back_led_counter;
}userTask;
#endif

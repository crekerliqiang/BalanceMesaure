#include "driver.h"
#include "interrupt.h"
#include "bleapi.h"
static BOOL isBleStart = FALSE;
static u32 bleTick = 0;
BLE_CMD ble_cmd_rec=API_DEFAULT;
char ble_data_in[BLE_REC_BUF_LEN]="";
int ble_data_out[BLE_REC_BUF_LEN]={0},ble_data_out_len=0;
static u8 ble_data_index=0;
static u32 systemTick = 0,feedDogTick=0;
u32 get_systemTick(void){
	return systemTick;
}
void reset_systemTick(void){
	systemTick = 0;
}
void set_systemTick(u32 t){
	systemTick=t;
}
userTask user={
	FALSE,
	FALSE,
	0,
	0,
	
};
static u8 pre = 2;
static u8 now = 1;
u8 rt[6]={0},rti=0;
void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 
		user.read_key_en = TRUE;
		feedDogTick+=1;
		//关机定时任务
		systemTick+=1;
		if(systemTick*_NAP_30 > DEALY_OFF_TIME){//DEALY_OFF_TIME S自动关机
			set_systemTick(0);
			power_off_now();
		}
		//喂狗任务
		if((feedDogTick*_NAP_30) > 1000){//1000ms
			feedDogTick=0;
			i_dog_feed();
		}
		//自动关锁任务
		if(user.is_auto_lock == TRUE){
			user.is_auto_lock_counter+=1;
			if(user.is_auto_lock_counter * _NAP_30 > 5000){//5000ms自动关锁
				user.is_ble_lock_ = TRUE;
				user.is_auto_lock = FALSE;
			}
		}
		//背光灯闪烁任务
		if(user.is_shine_back_led == TRUE){
			user.is_shine_back_led_counter+=1;
			if(user.is_shine_back_led_counter * _NAP_30 > 100){
				user.is_shine_back_led = FALSE;
				user.is_shine_back_led_counter = 0;
				BACK_LED_EN;
			}
		}
		//按键扫描任务
		if(user.read_key_en==TRUE)
			{
				now = touchKeyInt;
				if(now == TOUCHKEY_RELEASE)
				{
					pre = now;//覆盖上一个，表示状态一直没变
				}
				if(now == TOUCHKEY_PRESS_DOWN)//表示按下
				{
					if(pre == TOUCHKEY_RELEASE)//表示上一个没有按下
					{
						power_off_delay();
						pre = now;//重叠
						user.is_handle_key=TRUE;
						user.key_value = get_outkey_iic_data();
						if((user.key_value != 12)&&(user.key_value != 11))
						{
							user.is_handle_key=FALSE;//统计数据不需要处理
							if(user.key_value != 0xff)
							{
								say(DIDI);
								user.button_index += 1;
								user.buttonData[user.button_index - 1] = user.key_value;	
								BACK_LED_RESET;//熄灯
								user.is_shine_back_led = TRUE;
								#if DEBUGE	
								p1("button  ");
								pln(user.buttonData,user.button_index);
								#endif
								
							}
						}
					}
				}
			}
		//蓝牙数据接收任务
		if(isBleStart == TRUE){
			bleTick+=1;
			if(bleTick*_NAP_30 > 50){
				bleTick = 0;
				isBleStart = FALSE;
				if(TRUE == ble_rec_data(ble_data_in,&ble_cmd_rec,ble_data_out,&ble_data_out_len)){//参见协议
					power_off_delay();
					user.is_handle_ble_data = TRUE;
				}
				memset(ble_data_in,0,ble_data_index);
				ble_data_index=0;						
			}
		}
	}
}
//蓝牙接收程序

u8 res;
void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		isBleStart = TRUE;
		bleTick=0;
		res=USART_ReceiveData(USART3);
		
		#if DEBUGE
		p1("ble %d\r\n",res);
		#endif
		
		ble_data_in[ble_data_index]=res;
		ble_data_index+=1;
		if(ble_data_index>BLE_REC_BUF_LEN)ble_data_index=0;
	}
}


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
		//�ػ���ʱ����
		systemTick+=1;
		if(systemTick*_NAP_30 > DEALY_OFF_TIME){//DEALY_OFF_TIME S�Զ��ػ�
			set_systemTick(0);
			power_off_now();
		}
		//ι������
		if((feedDogTick*_NAP_30) > 1000){//1000ms
			feedDogTick=0;
			i_dog_feed();
		}
		//�Զ���������
		if(user.is_auto_lock == TRUE){
			user.is_auto_lock_counter+=1;
			if(user.is_auto_lock_counter * _NAP_30 > 5000){//5000ms�Զ�����
				user.is_ble_lock_ = TRUE;
				user.is_auto_lock = FALSE;
			}
		}
		//�������˸����
		if(user.is_shine_back_led == TRUE){
			user.is_shine_back_led_counter+=1;
			if(user.is_shine_back_led_counter * _NAP_30 > 100){
				user.is_shine_back_led = FALSE;
				user.is_shine_back_led_counter = 0;
				BACK_LED_EN;
			}
		}
		//����ɨ������
		if(user.read_key_en==TRUE)
			{
				now = touchKeyInt;
				if(now == TOUCHKEY_RELEASE)
				{
					pre = now;//������һ������ʾ״̬һֱû��
				}
				if(now == TOUCHKEY_PRESS_DOWN)//��ʾ����
				{
					if(pre == TOUCHKEY_RELEASE)//��ʾ��һ��û�а���
					{
						power_off_delay();
						pre = now;//�ص�
						user.is_handle_key=TRUE;
						user.key_value = get_outkey_iic_data();
						if((user.key_value != 12)&&(user.key_value != 11))
						{
							user.is_handle_key=FALSE;//ͳ�����ݲ���Ҫ����
							if(user.key_value != 0xff)
							{
								say(DIDI);
								user.button_index += 1;
								user.buttonData[user.button_index - 1] = user.key_value;	
								BACK_LED_RESET;//Ϩ��
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
		//�������ݽ�������
		if(isBleStart == TRUE){
			bleTick+=1;
			if(bleTick*_NAP_30 > 50){
				bleTick = 0;
				isBleStart = FALSE;
				if(TRUE == ble_rec_data(ble_data_in,&ble_cmd_rec,ble_data_out,&ble_data_out_len)){//�μ�Э��
					power_off_delay();
					user.is_handle_ble_data = TRUE;
				}
				memset(ble_data_in,0,ble_data_index);
				ble_data_index=0;						
			}
		}
	}
}
//�������ճ���

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


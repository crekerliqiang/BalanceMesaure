#include "driver.h"
void driver_init(void){
	__disable_irq();
	system_delay_init();
	rtc_init();
	system_bkp_init();
	lockpin_init();
	voice_init();
	backled_init();	
	system_adc_init();
	get_outkey_iic_init();
	touch_key_read_init();	
	debuge_usart1_init(BT);
	usart3_debuge(9600);
	ble_enable_init();
	tim3_init(_NAP_30);

	__enable_irq();
}


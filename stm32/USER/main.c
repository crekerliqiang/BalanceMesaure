
#include "driver.h"
#include "measure.h"

int main(void) 
{
	__disable_irq();
	system_delay_init();
	debuge_usart1_init(115200);
	
	measure_init();
	__enable_irq();	
	while(1){
		
		getData();
		system_delay_ms(10);
	}
}

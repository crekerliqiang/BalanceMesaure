#include "tool.h"
#include "interrupt.h"
#include "module.h"
extern userTask user;

/*
*函数功能：读取按键的值，做相应的处理
*输入：无
*输出：无
*返回：无
*/
void handle_button_data(void)
{
		PWD_STA pwd_sta;
		say(DIDI);
		if(is_key_enable() == FALSE){
			memset(user.buttonData,B_DEFAULT,BUTTON_PWD_LEN);
			user.button_index = 0;
			say(KEY_DISABLE);
			
			#if DEBUGE
			p1("key has been disabled\r\n");
			#endif
			
			return;
		}
		if(user.key_value == 11)//删除输入
		{
			memset(user.buttonData,B_DEFAULT,BUTTON_PWD_LEN);
			user.button_index = 0;
		}else
		if(user.key_value == 12)
		{
			if(user.button_index == 6)//密码-6位
			{
				if(is_usual_pwd_ok(user.buttonData,user.button_index) == TRUE){
					user.is_key_check_ok=TRUE;
					write_recod(user.buttonData,1);
					user.is_check_error=FALSE;
				}else{
					user.is_key_check_ok=FALSE;
					user.is_check_error=TRUE;
				}
			}else
			if(user.button_index == 9)//随机密码的位数 -9位
			{
				pwd_sta = judge_TemporaryPwd_legal(user.buttonData);
				if(pwd_sta == PWD_OK)
				{
					user.is_key_check_ok=TRUE;
					write_recod(user.buttonData,2);
				}else
				if(pwd_sta == PWD_WRONG)
				{
					user.is_check_error=TRUE;
				}else
				{
					user.is_check_overtue = TRUE;
				}
			}else
			if(user.button_index == 0){
				user.is_ble_lock_ = TRUE;
			}else//密码位数不对，直接提示错误
			{
				user.is_check_error=TRUE;
			}
			user.button_index = 0;
			memset(user.buttonData,B_DEFAULT,BUTTON_PWD_LEN);
		}else//正常输入密码
		{
			//中断中处理
		}
}







#include "display.h"
#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "dwin.h"
extern float CurrentValue,OilPressValue,AirPressValue;
extern u16 Temperature;
extern u8 DisconnectFlag;//断线标志位
extern u8 Tim4_cnt1,Tim4_cnt2;
void Lcd_Display(u8 sta)
{
	char s[248];

	switch(sta)
	{
		case 0:
//			MotorCtl=1;//打开电机
			sprintf(s,"※预润滑%2d秒，请5秒后启动 ",10-Tim4_cnt1);
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 1:
			MotorCtl=0;//关闭电机
			sprintf(s,"※祝您工作愉快                ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
//			USART_PutStr(USART1,"预润滑完成，电机关闭");
			break;
		case 2:
			MotorCtl=1;//打开电机
			//printf("DCV32(0,13,'※%d秒延时保护中                                ',2);\r\n",30-Tim4_cnt2);
			sprintf(s,"※%2d秒延时保护中              ",30-Tim4_cnt2);
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 3:
//			printf("DCV32(0,13,'※废气涡壳降温中                                ',2);\r\n");
			sprintf(s,"※废气涡壳降温中                ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 4:
			MotorCtl=0;
//			printf("DCV32(0,13,'※涡轮增压运行正常，祝您工作愉快                ',2);\r\n");
			sprintf(s,"※涡轮增压运行正常，祝您工作愉");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"快              ");
//			USART_PutStr(USART1,"运行正常，电机关闭");
			break;
		case 5:
			MotorCtl=1;//打开电机
//			printf("DCV32(0,13,'※废弃涡壳壳高温及低压保护中                    ',2);\r\n");
			sprintf(s,"※废弃涡壳壳高温及低压保护中  ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 6:
			MotorCtl=1;//打开电机
//			printf("DCV32(0,13,'※机油低压保护中                                ',2);\r\n");//绿色
			sprintf(s,"机油低压保护中                ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 7:
			MotorCtl=0;//关闭电机
//			printf("DCV32(0,13,'※异常！电机电流过大，请检查电机情况            ',1);\r\n");//红色
			sprintf(s,"※异常！电机电流过大，请检查电");
			LCD_printf(0x45,RED,BLACK,0,14,s);
			LCD_printf(0x45,RED,BLACK,0,56,"机情况            ");
		
//			USART_PutStr(USART1,"电流过大，电机保护");
			break;
	}
	
	delay_ms(10);
//	CheckBusy();
////	printf("DCV32(0,103,'※电机电流为%.1fA  ',4);\r\n",CurrentValue);
////	CheckBusy();
	if(DisconnectFlag==0)
	{
//		printf("DCV32(0,103,'※废气涡壳温度%d℃  ',4);\r\n",Temperature);//黄色
//		CheckBusy();
		sprintf(s,"※废气涡壳温度%d℃   ",Temperature);
		LCD_printf(0x45,YELLOW,BLACK,0,98,s);
		delay_ms(10);
	}
	else
	{
//		printf("DCV32(0,103,'※温度传感器异常       ',1);\r\n");
//		CheckBusy();
		sprintf(s,"※温度传感器异常     ");
		LCD_printf(0x45,RED,BLACK,0,98,s);
		delay_ms(10);
	}
//	printf("DCV32(0,148,'※机油压力%.1fkg/cm^2  ',4);\r\n",OilPressValue);
//	CheckBusy();
//	printf("DCV32(0,193,'※进气压力%.1fkg/cm^2  ',4);\r\n",AirPressValue);
//	CheckBusy();
	sprintf(s,"※机油压力%.1fkg/cm^2      ",OilPressValue);
	LCD_printf(0x45,YELLOW,BLACK,0,140,s);
	delay_ms(10);
	sprintf(s,"※进气压力%.1fkg/cm^2      ",AirPressValue);
	LCD_printf(0x45,YELLOW,BLACK,0,182,s);
	delay_ms(10);
	
}





















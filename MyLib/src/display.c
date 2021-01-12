#include "display.h"
#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "dwin.h"
extern float CurrentValue,OilPressValue,AirPressValue;
extern u16 Temperature;
extern u8 DisconnectFlag;//���߱�־λ
extern u8 Tim4_cnt1,Tim4_cnt2;
void Lcd_Display(u8 sta)
{
	char s[248];

	switch(sta)
	{
		case 0:
//			MotorCtl=1;//�򿪵��
			sprintf(s,"��Ԥ��%2d�룬��5������� ",10-Tim4_cnt1);
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 1:
			MotorCtl=0;//�رյ��
			sprintf(s,"��ף���������                ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
//			USART_PutStr(USART1,"Ԥ����ɣ�����ر�");
			break;
		case 2:
			MotorCtl=1;//�򿪵��
			//printf("DCV32(0,13,'��%d����ʱ������                                ',2);\r\n",30-Tim4_cnt2);
			sprintf(s,"��%2d����ʱ������              ",30-Tim4_cnt2);
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 3:
//			printf("DCV32(0,13,'�������пǽ�����                                ',2);\r\n");
			sprintf(s,"�������пǽ�����                ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 4:
			MotorCtl=0;
//			printf("DCV32(0,13,'��������ѹ����������ף���������                ',2);\r\n");
			sprintf(s,"��������ѹ����������ף��������");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"��              ");
//			USART_PutStr(USART1,"��������������ر�");
			break;
		case 5:
			MotorCtl=1;//�򿪵��
//			printf("DCV32(0,13,'�������пǿǸ��¼���ѹ������                    ',2);\r\n");
			sprintf(s,"�������пǿǸ��¼���ѹ������  ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 6:
			MotorCtl=1;//�򿪵��
//			printf("DCV32(0,13,'�����͵�ѹ������                                ',2);\r\n");//��ɫ
			sprintf(s,"���͵�ѹ������                ");
			LCD_printf(0x45,GREEN,BLACK,0,14,s);
			LCD_printf(0x45,GREEN,BLACK,0,56,"               ");
			break;
		case 7:
			MotorCtl=0;//�رյ��
//			printf("DCV32(0,13,'���쳣����������������������            ',1);\r\n");//��ɫ
			sprintf(s,"���쳣������������������");
			LCD_printf(0x45,RED,BLACK,0,14,s);
			LCD_printf(0x45,RED,BLACK,0,56,"�����            ");
		
//			USART_PutStr(USART1,"�������󣬵������");
			break;
	}
	
	delay_ms(10);
//	CheckBusy();
////	printf("DCV32(0,103,'���������Ϊ%.1fA  ',4);\r\n",CurrentValue);
////	CheckBusy();
	if(DisconnectFlag==0)
	{
//		printf("DCV32(0,103,'�������п��¶�%d��  ',4);\r\n",Temperature);//��ɫ
//		CheckBusy();
		sprintf(s,"�������п��¶�%d��   ",Temperature);
		LCD_printf(0x45,YELLOW,BLACK,0,98,s);
		delay_ms(10);
	}
	else
	{
//		printf("DCV32(0,103,'���¶ȴ������쳣       ',1);\r\n");
//		CheckBusy();
		sprintf(s,"���¶ȴ������쳣     ");
		LCD_printf(0x45,RED,BLACK,0,98,s);
		delay_ms(10);
	}
//	printf("DCV32(0,148,'������ѹ��%.1fkg/cm^2  ',4);\r\n",OilPressValue);
//	CheckBusy();
//	printf("DCV32(0,193,'������ѹ��%.1fkg/cm^2  ',4);\r\n",AirPressValue);
//	CheckBusy();
	sprintf(s,"������ѹ��%.1fkg/cm^2      ",OilPressValue);
	LCD_printf(0x45,YELLOW,BLACK,0,140,s);
	delay_ms(10);
	sprintf(s,"������ѹ��%.1fkg/cm^2      ",AirPressValue);
	LCD_printf(0x45,YELLOW,BLACK,0,182,s);
	delay_ms(10);
	
}





















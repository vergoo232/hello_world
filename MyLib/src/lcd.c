#include "lcd.h"
#include "usart.h"
#include "gpio.h"
extern float CurrentValue,OilPressValue,AirPressValue;
extern u16 Temperature;
extern u8 DisconnectFlag;//���߱�־λ
extern u8 Tim4_cnt1,Tim4_cnt2;
void Lcd_Display(u8 sta)
{
	switch(sta)
	{
		case 0:
//			MotorCtl=1;//�򿪵��
			printf("DCV32(0,13,'��Ԥ��%2d�룬��5�������                       ',2);\r\n",10-Tim4_cnt1);
			break;
		case 1:
			MotorCtl=0;//�رյ��
			printf("DCV32(0,13,'��ף���������                                  ',2);\r\n");
			USART_PutStr(USART1,"Ԥ����ɣ�����ر�");
			break;
		case 2:
			MotorCtl=1;//�򿪵��
			printf("DCV32(0,13,'��%d����ʱ������                                ',2);\r\n",30-Tim4_cnt2);
			break;
		case 3:
			printf("DCV32(0,13,'�������пǽ�����                                ',2);\r\n");
			break;
		case 4:
			MotorCtl=0;
			printf("DCV32(0,13,'��������ѹ����������ף���������                ',2);\r\n");
			USART_PutStr(USART1,"��������������ر�");
			break;
		case 5:
			MotorCtl=1;//�򿪵��
			printf("DCV32(0,13,'�������пǿǸ��¼���ѹ������                    ',2);\r\n");
			break;
		case 6:
			MotorCtl=1;//�򿪵��
			printf("DCV32(0,13,'�����͵�ѹ������                                ',2);\r\n");
			break;
		case 7:
			MotorCtl=0;//�رյ��
			printf("DCV32(0,13,'���쳣����������������������            ',1);\r\n");
			USART_PutStr(USART1,"�������󣬵������");
			break;
	}
	CheckBusy();
//	printf("DCV32(0,103,'���������Ϊ%.1fA  ',4);\r\n",CurrentValue);
//	CheckBusy();
	if(DisconnectFlag==0)
	{
		printf("DCV32(0,103,'�������п��¶�%d��  ',4);\r\n",Temperature);
		CheckBusy();
	}
	else
	{
		printf("DCV32(0,103,'���¶ȴ������쳣       ',1);\r\n");
		CheckBusy();	
	}
	printf("DCV32(0,148,'������ѹ��%.1fkg/cm^2  ',4);\r\n",OilPressValue);
	CheckBusy();
	printf("DCV32(0,193,'������ѹ��%.1fkg/cm^2  ',4);\r\n",AirPressValue);
	CheckBusy();
}

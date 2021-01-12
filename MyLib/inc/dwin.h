#ifndef __DWIN_H
#define __DWIN_H
#include "system.h"

#define  LCD_USART  USART2
/*
* ���弰������ɫ 
*/
typedef enum 
{
	BLACK = 0x0000,
	RED = 0xf800,
	YELLOW = 0xffe0,
	GREEN = 0x07e0
	
}ColorTypes;

void LCD_printf(u8 mode,ColorTypes Color,ColorTypes Bcolor,u16 x,u16 y,const char *format, ...);//��ʾ�ַ������߱���
void erweima(u16 x,u16 y,u8 QR_Pixel,const char *format, ...);//���ɶ�ά��
void clr(ColorTypes Color);//����
void Linear_interpolation(u8 num,ColorTypes Color,...);//����ֱ��
void Place_point(u8 num,ColorTypes Color,u16 nx,u16 ny,...);//���Ƶ�
void DIM_Set(u8 Set);//���ڱ���
void Bode_Set(u16 Set);//������չ���ڲ�����
void disd_ico(u16 x,u16 y,u8 mode,u8 Icon_lib,u8 Icon_IDs,u8 Icon_0IDe,u8 Delay_time);//���ö�̬ͼ��
void dis_ico(u16 x,u16 y,u8 mode,u8 ids);//��ʾicoͼ��
void pic(u8 id);//д���ݴ洢��
#endif 


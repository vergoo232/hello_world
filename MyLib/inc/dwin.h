#ifndef __DWIN_H
#define __DWIN_H
#include "system.h"

#define  LCD_USART  USART2
/*
* 字体及背景颜色 
*/
typedef enum 
{
	BLACK = 0x0000,
	RED = 0xf800,
	YELLOW = 0xffe0,
	GREEN = 0x07e0
	
}ColorTypes;

void LCD_printf(u8 mode,ColorTypes Color,ColorTypes Bcolor,u16 x,u16 y,const char *format, ...);//显示字符串或者变量
void erweima(u16 x,u16 y,u8 QR_Pixel,const char *format, ...);//生成二维码
void clr(ColorTypes Color);//清屏
void Linear_interpolation(u8 num,ColorTypes Color,...);//绘制直线
void Place_point(u8 num,ColorTypes Color,u16 nx,u16 ny,...);//绘制点
void DIM_Set(u8 Set);//调节背光
void Bode_Set(u16 Set);//调节扩展串口波特率
void disd_ico(u16 x,u16 y,u8 mode,u8 Icon_lib,u8 Icon_IDs,u8 Icon_0IDe,u8 Delay_time);//设置动态图标
void dis_ico(u16 x,u16 y,u8 mode,u8 ids);//显示ico图标
void pic(u8 id);//写数据存储器
#endif 


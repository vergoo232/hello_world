#include "dwin.h"
#include "usart.h"
/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
/****************显示字符串函数*****************/
/*参数：mode：D7********************************/
void LCD_printf(u8 mode,ColorTypes Color,ColorTypes Bcolor,u16 x,u16 y,const char *format, ...)
{
	char tmp[200];
	va_list arg; 
	va_start(arg, format); 
	vsprintf(tmp,format,arg);
	va_end(arg); 
	Usart_SendByte(LCD_USART,0xaa);

	Usart_SendByte(LCD_USART,0x11);
	Usart_SendByte(LCD_USART,mode);//模式
	Usart_SendByte(LCD_USART,Color/256);
	Usart_SendByte(LCD_USART,Color%256);//前景颜色
	Usart_SendByte(LCD_USART,Bcolor/256);
	Usart_SendByte(LCD_USART,Bcolor%256);//背景颜色
	Usart_SendByte(LCD_USART,x/256);
	Usart_SendByte(LCD_USART,x%256);//横坐标
	Usart_SendByte(LCD_USART,y/256);
	Usart_SendByte(LCD_USART,y%256);//Y坐标

	Usart_SendString(LCD_USART,tmp);	
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************显示二维码函数*****************/
/*参数：mode：D7********************************/
void erweima(u16 x,u16 y,u8 QR_Pixel,const char *format, ...)
{
	char tmp[200];
	va_list arg; 
	va_start(arg, format); 
	vsprintf(tmp,format,arg);
	va_end(arg); 

	Usart_SendByte(LCD_USART,0xaa);

	Usart_SendByte(LCD_USART,0x21);

	Usart_SendByte(LCD_USART,x/256);
	Usart_SendByte(LCD_USART,x%256);//横坐标
	Usart_SendByte(LCD_USART,y/256);
	Usart_SendByte(LCD_USART,y%256);//Y坐标
	Usart_SendByte(LCD_USART,QR_Pixel);//二维码每个点的大小

	Usart_SendString(LCD_USART,tmp);	
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************读存储器***********************/
/*参数：mode：D7********************************/
void read_sram_flsh(u16 Length,u8 Type,u16 Address)//写数据存储器
{
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x32);
	Usart_SendByte(LCD_USART,Type);
	Usart_SendByte(LCD_USART,Address/256);
	Usart_SendByte(LCD_USART,Address%256);
	Usart_SendByte(LCD_USART,Length);
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}

/****************绘图清屏函数*****************/
/*参数：mode：D7********************************/
void clr(ColorTypes Color)
{
	Usart_SendByte(LCD_USART,0xaa);

	Usart_SendByte(LCD_USART,0x01);

	Usart_SendByte(LCD_USART,Color/256);
	Usart_SendByte(LCD_USART,Color%256);//颜色


	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************绘图直线插入函数*****************/
/*参数：mode：D7********************************/
void Linear_interpolation(u8 num,ColorTypes Color,...)
{
	va_list arg_ptr;
	int tempValue;
	num*=2;
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x03);
	Usart_SendByte(LCD_USART,Color/256);
	Usart_SendByte(LCD_USART,Color%256);
	va_start(arg_ptr,Color);
	while(num--)
	{
		tempValue=va_arg(arg_ptr,int);
		Usart_SendByte(LCD_USART,tempValue/256);
		Usart_SendByte(LCD_USART,tempValue%256);
	}

	va_end(arg_ptr);
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************绘图打点插入函数*****************/
/*参数：mode：D7********************************/
void Place_point(u8 num,ColorTypes Color,u16 nx,u16 ny,...)
{
	va_list arg_ptr;
	int tempValue;
	num*=2;
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x02);
	Usart_SendByte(LCD_USART,Color/256);
	Usart_SendByte(LCD_USART,Color%256);
	Usart_SendByte(LCD_USART,nx%256);
	Usart_SendByte(LCD_USART,ny%256);
	va_start(arg_ptr,ny);
	while(num--)
	{
		tempValue=va_arg(arg_ptr,int);
		Usart_SendByte(LCD_USART,tempValue/256);
		Usart_SendByte(LCD_USART,tempValue%256);
	}

	va_end(arg_ptr);
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************屏幕背光亮度设置*****************/
/*参数：mode：D7********************************/
void DIM_Set(u8 Set)
{

	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x30);
	Usart_SendByte(LCD_USART,Set);
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************设置扩展串口波特率**************/
/*参数：mode：D7********************************/
void Bode_Set(u16 Set)//调节扩展串口波特率
{
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x38);
	Usart_SendByte(LCD_USART,15667200/Set/256);
	Usart_SendByte(LCD_USART,15667200/Set%256);
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************从扩展串口发送字符串**************/
/*参数：mode：D7********************************/
void UART_TX(const char *format, ...)//串口发送字符串
{
	char tmp[200];
	va_list arg; 
	va_start(arg, format); 
	vsprintf(tmp,format,arg);
	va_end(arg); 
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x39);
	Usart_SendString(LCD_USART,tmp);
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************显示ICO图标**************/
/*参数：mode：D7********************************/
void dis_ico(u16 x,u16 y,u8 mode,u8 ids)//显示ico图标
{
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x23);
	Usart_SendByte(LCD_USART,x/256);
	Usart_SendByte(LCD_USART,x%256);
	Usart_SendByte(LCD_USART,y/256);
	Usart_SendByte(LCD_USART,y%256);	
	Usart_SendByte(LCD_USART,mode);	
	Usart_SendByte(LCD_USART,ids);
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************设置动态图标*******************/
/*参数：mode：D7********************************/
void disd_ico(u16 x,u16 y,u8 mode,u8 Icon_lib,u8 Icon_IDs,u8 Icon_0IDe,u8 Delay_time)//显示ico图标
{
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x28);
	Usart_SendByte(LCD_USART,x/256);
	Usart_SendByte(LCD_USART,x%256); 
	Usart_SendByte(LCD_USART,y/256);
	Usart_SendByte(LCD_USART,y%256);	
	Usart_SendByte(LCD_USART,mode);
	Usart_SendByte(LCD_USART,Icon_lib);
	Usart_SendByte(LCD_USART,Icon_IDs);
	Usart_SendByte(LCD_USART,Icon_0IDe);
	Usart_SendByte(LCD_USART,Delay_time);

	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}
/****************控制动态图标*******************/
/*参数：mode：D7********************************/
void disc_ico(u16 set)//控制ico动态图标
{
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x29);

	Usart_SendByte(LCD_USART,set/256);
	Usart_SendByte(LCD_USART,set%256); 

	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}

/****************写存储器***********************/
/*参数：mode：D7********************************/
void writ_sram_flsh(u16 Length,u8 Type,u16 Address)//写数据存储器
{
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x31);
	Usart_SendByte(LCD_USART,Type);
	Usart_SendByte(LCD_USART,Address/256);
	Usart_SendByte(LCD_USART,Address%256);
	while(Length--)
	{
		Usart_SendByte(LCD_USART,Address++);
	}	

	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}

/****************读存储器***********************/
/*参数：mode：D7********************************/
void pic(u8 id)
{
	Usart_SendByte(LCD_USART,0xaa);
	Usart_SendByte(LCD_USART,0x22);
	Usart_SendByte(LCD_USART,0x00);
	Usart_SendByte(LCD_USART,id);
	\
	Usart_SendByte(LCD_USART,0xcc);
	Usart_SendByte(LCD_USART,0x33);
	Usart_SendByte(LCD_USART,0xc3);
	Usart_SendByte(LCD_USART,0x3c);
}

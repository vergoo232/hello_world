#ifndef __HT1621B_H__
#define __HT1621B_H__	 
#include "system.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#define uchar unsigned char
#define uint unsigned int
//////////////////////////////////////////////////////////////////////////////////	
// - - 定义子程序函数
void Ht1621_Init(void); // - - 初始化 HT1621
void Ht1621Wr_Data(uchar Data,uchar cnt); // - - HT1621 写入数据函数
void Ht1621WrCmd(uchar Cmd); // - - HT1621 写入命令函数
void Ht1621WrOneData(uchar Addr,uchar Data); // - - HT1621 在指定地址写入数据函数
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt); // - - HT1621 连续写入数据函数
void Display(void);	//显示数字
void Display_lcd_dot(void);//显示字符
void data_convertor(unsigned long adc_value);//输入数字

#endif

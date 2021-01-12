#ifndef __HT1621B_H__
#define __HT1621B_H__	 
#include "system.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#define uchar unsigned char
#define uint unsigned int
//////////////////////////////////////////////////////////////////////////////////	
// - - �����ӳ�����
void Ht1621_Init(void); // - - ��ʼ�� HT1621
void Ht1621Wr_Data(uchar Data,uchar cnt); // - - HT1621 д�����ݺ���
void Ht1621WrCmd(uchar Cmd); // - - HT1621 д�������
void Ht1621WrOneData(uchar Addr,uchar Data); // - - HT1621 ��ָ����ַд�����ݺ���
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt); // - - HT1621 ����д�����ݺ���
void Display(void);	//��ʾ����
void Display_lcd_dot(void);//��ʾ�ַ�
void data_convertor(unsigned long adc_value);//��������

#endif

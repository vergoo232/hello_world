// - - * * * * * * * HT1621 ���������� * * * * * * *
// - - * Description: HT1621 ���������� *
// - - * C ���Գ��� *
// - - �ӿڶ���
/*************************************************
 HT1621_CS  PBout(1) // - - HT1621ʹ������ 
 HT1621_WR  PBout(2) // - - HT1621ʱ������
 HT1621_DA PBout(10) // - - HT1621��������
**************************************************/ 
// - - ���庯��
#include "ht1621b.h"

#define BIAS 0x52 //0b1000 0101 0010 1/3duty 4com
#define SYSDIS 0X00 //0b1000 0000 0000 ����ϵͳ������LCDƫѹ������
#define SYSEN 0X02 //0b1000 0000 0010 ��ϵͳ����
#define LCDOFF 0X04 //0b1000 0000 0100 ��LCDƫѹ
#define LCDON 0X06 //0b1000 0000 0110 ��LCDƫѹ
#define XTAL 0x28 //0b1000 0010 1000 �ⲿ��ʱ��
#define RC256 0X30 //0b1000 0011 0000 �ڲ�ʱ��
#define TONEON 0X12 //0b1000 0001 0010 ���������
#define TONEOFF 0X10 //0b1000 0001 0000 �ر��������
#define WDTDIS 0X0A //0b1000 0000 1010 ��ֹ���Ź�
// - - ����ӿ�
// - - HT1621����λ��Һ��ģ��ӿڶ��壬�������ѵ���Ҫ���ģ�
#define HT1621_CS  PBout(1) // - - HT1621ʹ������ 
#define HT1621_WR  PBout(2) // - - HT1621ʱ������
#define HT1621_DA PBout(10) // - - HT1621��������        
// - - �������,����
unsigned int tmp;
unsigned int n1, n2, n3, n4;
unsigned char Ht1621Tab[]={0x00,0x00,0x00,0x00};
unsigned char DispTab[]={0x7B,0x12,0x67,0x57,0x1E,0x5D,0x7D,0x13,0x7F,0x5F,0x69,0x45,0x3f, 0x7a, 0x3e};
              //          0    1    2    3    4    5    6    7    8    9    C    ��   A     V     H
/******************************************************
�������ƣ�void Ht1621Wr_Data(uchar Data,uchar cnt)
��������: HT1621����д�뺯��
ȫ�ֱ�������
����˵����DataΪ���ݣ�cntΪ����λ��
����˵������
˵ ����д���ݺ���,cntΪ��������λ��,���ݴ���Ϊ��λ��ǰ
*******************************************************/
void Ht1621Wr_Data(unsigned char Data,unsigned char cnt) 
{ 
  unsigned char i; 
  for (i=0;i<cnt;i++) 
   {   
     HT1621_WR = 0;
	 delay_us(10); //�������
     if((Data & 0x80)==0x80) {HT1621_DA = 1;}
     else {HT1621_DA = 0;}
	 delay_us(10); 
     HT1621_WR = 1;
	 delay_us(10);  
     Data<<=1; 
   } 
} 
/********************************************************
�������ƣ�void Ht1621WrCmd(uchar Cmd)
��������: HT1621����д�뺯��
ȫ�ֱ�������
����˵����CmdΪд����������
����˵������
˵ ����д�������ʶλ100
********************************************************/
void Ht1621WrCmd(unsigned char Cmd) 
{ 
   HT1621_CS = 0; 
   Ht1621Wr_Data(0x80,4);          //д�������־100 
   Ht1621Wr_Data(Cmd,8);           //д���������� 
   HT1621_CS = 1; 
} 
/********************************************************
�������ƣ�void Ht1621WrOneData(uchar Addr,uchar Data)
��������: HT1621��ָ����ַд�����ݺ���
ȫ�ֱ�������
����˵����AddrΪд���ʼ��ַ��DataΪд������
����˵������
˵ ������ΪHT1621������λ4λ������ʵ��д������Ϊ�����ĺ�4λ
********************************************************/
void Ht1621WrOneData(unsigned char Addr,unsigned char Data)
{
  HT1621_CS = 0;
  Ht1621Wr_Data(0xa0,3);  //д�����ݱ�־101
  Ht1621Wr_Data(Addr<<2,6); //д���ַ����
  Ht1621Wr_Data(Data,4); //д�����ݵ�ǰ��λ 7  6  5  4
  HT1621_CS = 1;
}
/********************************************************
�������ƣ�void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
��������: HT1621����д�뷽ʽ����
ȫ�ֱ�������
����˵����AddrΪд���ʼ��ַ��*pΪ����д������ָ�룬
cntΪд����������
����˵������
˵ ����HT1621������λ4λ���˴�ÿ������Ϊ8λ��д������
������8λ����
********************************************************/
void Ht1621WrAllData(unsigned char Addr,unsigned char *p,unsigned char cnt)
{
  unsigned char i;
  HT1621_CS = 0;
  Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
  Ht1621Wr_Data(Addr<<2,6); //д���ַ����
  for (i=0;i<cnt;i++)
   {
    Ht1621Wr_Data(*p,8); //д������
    p++;
   }
  HT1621_CS = 1;
}

/********************************************************
�������ƣ�void Ht1621_Init(void)
��������: HT1621��ʼ��
ȫ�ֱ�������
����˵������
����˵������
�� ����1.0
********************************************************/
void Ht1621_Init(void) 
{ 
	HT1621_CS=1;
	HT1621_WR=1;
	HT1621_DA=1;
	Ht1621WrCmd(BIAS); 
	Ht1621WrCmd(RC256);             //ʹ���ڲ����� 
	//Ht1621WrCmd(XTAL);             //ʹ���ⲿ���� 
	Ht1621WrCmd(SYSDIS); 
	Ht1621WrCmd(WDTDIS); 
	Ht1621WrCmd(SYSEN); 
	Ht1621WrCmd(LCDON); 
} 
///////////////////////////////////////////////////////////
/********************************************************
�������ƣ�void data_convertor(unsigned long adc_value)
��������: ����������ַָ� 
ȫ�ֱ�������
����˵����adc_value ���������
����˵������
�� ����1.0
********************************************************/
void data_convertor(unsigned long adc_value) 
{  
    tmp=adc_value;         //adc 
    n4=tmp/100;
    tmp=tmp%100;	
    n3=tmp/10;
    tmp=tmp%10;		
    n2=tmp; 
                 // 4 
    n1=10;       //display "C"
    //n1=11;       //display "��"
    //n1=12;       //display "A"
    //n1=13;       //display "V"
    //n1=14;       //display "H"
 }
/********************************************************
�������ƣ�void Display(void)
��������: ��ʾ���� n1 n2 n3 n4 
ȫ�ֱ�������
����˵������
����˵������
�� ����1.0
********************************************************/
void Display(void) 
{ 
   unsigned char com;
   //��ַ0  SEG0
   com = 0;
   if((DispTab[n1]&0x01) == 0x01){ com = com + 0x10;}  // �ж�1A�Ƿ���Ҫ��ʾ
   if((DispTab[n2]&0x01) == 0x01){ com = com + 0x20;}  // �ж�2A�Ƿ���Ҫ��ʾ
   if((DispTab[n3]&0x01) == 0x01){ com = com + 0x40;}  // �ж�3A�Ƿ���Ҫ��ʾ
   if((DispTab[n4]&0x01) == 0x01){ com = com + 0x80;}  // �ж�4A�Ƿ���Ҫ��ʾ
   Ht1621WrOneData(0 , com);
    //��ַ1  SEG2
   com = 0;
   if((DispTab[n1]&0x02) == 0x02){ com = com + 0x10;}  // �ж�1B�Ƿ���Ҫ��ʾ
   if((DispTab[n2]&0x02) == 0x02){ com = com + 0x20;} // �ж�2B�Ƿ���Ҫ��ʾ
   if((DispTab[n3]&0x02) == 0x02){ com = com + 0x40;}  // �ж�3B�Ƿ���Ҫ��ʾ
   if((DispTab[n4]&0x02) == 0x02){ com = com + 0x80;}  // �ж�4B�Ƿ���Ҫ��ʾ
   Ht1621WrOneData(1 , com);
   com = 0;
   if((DispTab[n1]&0x10) == 0x10){ com = com + 0x10;}  // �ж�1C�Ƿ���Ҫ��ʾ
   if((DispTab[n2]&0x10) == 0x10){ com = com + 0x20;}	// �ж�2C�Ƿ���Ҫ��ʾ
   if((DispTab[n3]&0x10) == 0x10){ com = com + 0x40;}  // �ж�3C�Ƿ���Ҫ��ʾ
   if((DispTab[n4]&0x10) == 0x10){ com = com + 0x80;} // �ж�4C�Ƿ���Ҫ��ʾ
   Ht1621WrOneData(2 , com);
   com = 0;
   if((DispTab[n1]&0x40) == 0x40){ com = com + 0x10;}  // �ж�1D�Ƿ���Ҫ��ʾ
   if((DispTab[n2]&0x40) == 0x40){ com = com + 0x20;}  // �ж�2D�Ƿ���Ҫ��ʾ
   if((DispTab[n3]&0x40) == 0x40){ com = com + 0x40;}  // �ж�3D�Ƿ���Ҫ��ʾ
   if((DispTab[n4]&0x40) == 0x40){ com = com + 0x80;}  // �ж�4D�Ƿ���Ҫ��ʾ
   Ht1621WrOneData(3 , com);
   com = 0;
   if((DispTab[n1]&0x20) == 0x20){ com = com + 0x10;}		//E
   if((DispTab[n2]&0x20) == 0x20){ com = com + 0x20;}
   if((DispTab[n3]&0x20) == 0x20){ com = com + 0x40;}
   if((DispTab[n4]&0x20) == 0x20){ com = com + 0x80;}
   Ht1621WrOneData(4 , com);
   com = 0;
   if((DispTab[n1]&0x08) == 0x08){ com = com + 0x10;}	  //F
   if((DispTab[n2]&0x08) == 0x08){ com = com + 0x20;}
   if((DispTab[n3]&0x08) == 0x08){ com = com + 0x40;}
   if((DispTab[n4]&0x08) == 0x08){ com = com + 0x80;}
   Ht1621WrOneData(5 , com);
   //��ַ6 seg6
   com = 0;
   if((DispTab[n1]&0x04) == 0x04){ com = com + 0x10;}	   //G
   if((DispTab[n2]&0x04) == 0x04){ com = com + 0x20;}
   if((DispTab[n3]&0x04) == 0x04){ com = com + 0x40;}
   if((DispTab[n4]&0x04) == 0x04){ com = com + 0x80;}
   Ht1621WrOneData(6 , com); 
}
/********************************************************
�������ƣ�void Display_lcd_dot(void)
��������: ��ʾ���� ��.  
ȫ�ֱ�������
����˵������
����˵������
�� ����1.0
********************************************************/
void Display_lcd_dot(void) 
{ 
   Ht1621WrOneData(7 , 0x50);	   //.'	   2down 3 up
 //  Ht1621WrOneData(7 , 0x80);    //.   --> 1 down
  //Ht1621WrOneData(7 , 0x40);    //. --> 2 down
  // Ht1621WrOneData(7 , 0x20);    //. --> 2 up
   //Ht1621WrOneData(7 , 0x10);    //.:' --> 3 up
}


// - - * * * * * * * HT1621 段码屏驱动 * * * * * * *
// - - * Description: HT1621 段码屏驱动 *
// - - * C 语言程序 *
// - - 接口定义
/*************************************************
 HT1621_CS  PBout(1) // - - HT1621使能引脚 
 HT1621_WR  PBout(2) // - - HT1621时钟引脚
 HT1621_DA PBout(10) // - - HT1621数据引脚
**************************************************/ 
// - - 定义函数
#include "ht1621b.h"

#define BIAS 0x52 //0b1000 0101 0010 1/3duty 4com
#define SYSDIS 0X00 //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN 0X02 //0b1000 0000 0010 打开系统振荡器
#define LCDOFF 0X04 //0b1000 0000 0100 关LCD偏压
#define LCDON 0X06 //0b1000 0000 0110 打开LCD偏压
#define XTAL 0x28 //0b1000 0010 1000 外部接时钟
#define RC256 0X30 //0b1000 0011 0000 内部时钟
#define TONEON 0X12 //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10 //0b1000 0001 0000 关闭声音输出
#define WDTDIS 0X0A //0b1000 0000 1010 禁止看门狗
// - - 定义接口
// - - HT1621控制位（液晶模块接口定义，根据自已的需要更改）
#define HT1621_CS  PBout(1) // - - HT1621使能引脚 
#define HT1621_WR  PBout(2) // - - HT1621时钟引脚
#define HT1621_DA PBout(10) // - - HT1621数据引脚        
// - - 定义变量,数组
unsigned int tmp;
unsigned int n1, n2, n3, n4;
unsigned char Ht1621Tab[]={0x00,0x00,0x00,0x00};
unsigned char DispTab[]={0x7B,0x12,0x67,0x57,0x1E,0x5D,0x7D,0x13,0x7F,0x5F,0x69,0x45,0x3f, 0x7a, 0x3e};
              //          0    1    2    3    4    5    6    7    8    9    C    三   A     V     H
/******************************************************
函数名称：void Ht1621Wr_Data(uchar Data,uchar cnt)
功能描述: HT1621数据写入函数
全局变量：无
参数说明：Data为数据，cnt为数据位数
返回说明：无
说 明：写数据函数,cnt为传送数据位数,数据传送为低位在前
*******************************************************/
void Ht1621Wr_Data(unsigned char Data,unsigned char cnt) 
{ 
  unsigned char i; 
  for (i=0;i<cnt;i++) 
   {   
     HT1621_WR = 0;
	 delay_us(10); //必须加上
     if((Data & 0x80)==0x80) {HT1621_DA = 1;}
     else {HT1621_DA = 0;}
	 delay_us(10); 
     HT1621_WR = 1;
	 delay_us(10);  
     Data<<=1; 
   } 
} 
/********************************************************
函数名称：void Ht1621WrCmd(uchar Cmd)
功能描述: HT1621命令写入函数
全局变量：无
参数说明：Cmd为写入命令数据
返回说明：无
说 明：写入命令标识位100
********************************************************/
void Ht1621WrCmd(unsigned char Cmd) 
{ 
   HT1621_CS = 0; 
   Ht1621Wr_Data(0x80,4);          //写入命令标志100 
   Ht1621Wr_Data(Cmd,8);           //写入命令数据 
   HT1621_CS = 1; 
} 
/********************************************************
函数名称：void Ht1621WrOneData(uchar Addr,uchar Data)
功能描述: HT1621在指定地址写入数据函数
全局变量：无
参数说明：Addr为写入初始地址，Data为写入数据
返回说明：无
说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
********************************************************/
void Ht1621WrOneData(unsigned char Addr,unsigned char Data)
{
  HT1621_CS = 0;
  Ht1621Wr_Data(0xa0,3);  //写入数据标志101
  Ht1621Wr_Data(Addr<<2,6); //写入地址数据
  Ht1621Wr_Data(Data,4); //写入数据的前四位 7  6  5  4
  HT1621_CS = 1;
}
/********************************************************
函数名称：void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
功能描述: HT1621连续写入方式函数
全局变量：无
参数说明：Addr为写入初始地址，*p为连续写入数据指针，
cnt为写入数据总数
返回说明：无
说 明：HT1621的数据位4位，此处每次数据为8位，写入数据
总数按8位计算
********************************************************/
void Ht1621WrAllData(unsigned char Addr,unsigned char *p,unsigned char cnt)
{
  unsigned char i;
  HT1621_CS = 0;
  Ht1621Wr_Data(0xa0,3); //写入数据标志101
  Ht1621Wr_Data(Addr<<2,6); //写入地址数据
  for (i=0;i<cnt;i++)
   {
    Ht1621Wr_Data(*p,8); //写入数据
    p++;
   }
  HT1621_CS = 1;
}

/********************************************************
函数名称：void Ht1621_Init(void)
功能描述: HT1621初始化
全局变量：无
参数说明：无
返回说明：无
版 本：1.0
********************************************************/
void Ht1621_Init(void) 
{ 
	HT1621_CS=1;
	HT1621_WR=1;
	HT1621_DA=1;
	Ht1621WrCmd(BIAS); 
	Ht1621WrCmd(RC256);             //使用内部振荡器 
	//Ht1621WrCmd(XTAL);             //使用外部振荡器 
	Ht1621WrCmd(SYSDIS); 
	Ht1621WrCmd(WDTDIS); 
	Ht1621WrCmd(SYSEN); 
	Ht1621WrCmd(LCDON); 
} 
///////////////////////////////////////////////////////////
/********************************************************
函数名称：void data_convertor(unsigned long adc_value)
功能描述: 将输入的数字分割 
全局变量：无
参数说明：adc_value 输入的数字
返回说明：无
版 本：1.0
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
    //n1=11;       //display "三"
    //n1=12;       //display "A"
    //n1=13;       //display "V"
    //n1=14;       //display "H"
 }
/********************************************************
函数名称：void Display(void)
功能描述: 显示数字 n1 n2 n3 n4 
全局变量：无
参数说明：无
返回说明：无
版 本：1.0
********************************************************/
void Display(void) 
{ 
   unsigned char com;
   //地址0  SEG0
   com = 0;
   if((DispTab[n1]&0x01) == 0x01){ com = com + 0x10;}  // 判断1A是否需要显示
   if((DispTab[n2]&0x01) == 0x01){ com = com + 0x20;}  // 判断2A是否需要显示
   if((DispTab[n3]&0x01) == 0x01){ com = com + 0x40;}  // 判断3A是否需要显示
   if((DispTab[n4]&0x01) == 0x01){ com = com + 0x80;}  // 判断4A是否需要显示
   Ht1621WrOneData(0 , com);
    //地址1  SEG2
   com = 0;
   if((DispTab[n1]&0x02) == 0x02){ com = com + 0x10;}  // 判断1B是否需要显示
   if((DispTab[n2]&0x02) == 0x02){ com = com + 0x20;} // 判断2B是否需要显示
   if((DispTab[n3]&0x02) == 0x02){ com = com + 0x40;}  // 判断3B是否需要显示
   if((DispTab[n4]&0x02) == 0x02){ com = com + 0x80;}  // 判断4B是否需要显示
   Ht1621WrOneData(1 , com);
   com = 0;
   if((DispTab[n1]&0x10) == 0x10){ com = com + 0x10;}  // 判断1C是否需要显示
   if((DispTab[n2]&0x10) == 0x10){ com = com + 0x20;}	// 判断2C是否需要显示
   if((DispTab[n3]&0x10) == 0x10){ com = com + 0x40;}  // 判断3C是否需要显示
   if((DispTab[n4]&0x10) == 0x10){ com = com + 0x80;} // 判断4C是否需要显示
   Ht1621WrOneData(2 , com);
   com = 0;
   if((DispTab[n1]&0x40) == 0x40){ com = com + 0x10;}  // 判断1D是否需要显示
   if((DispTab[n2]&0x40) == 0x40){ com = com + 0x20;}  // 判断2D是否需要显示
   if((DispTab[n3]&0x40) == 0x40){ com = com + 0x40;}  // 判断3D是否需要显示
   if((DispTab[n4]&0x40) == 0x40){ com = com + 0x80;}  // 判断4D是否需要显示
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
   //地址6 seg6
   com = 0;
   if((DispTab[n1]&0x04) == 0x04){ com = com + 0x10;}	   //G
   if((DispTab[n2]&0x04) == 0x04){ com = com + 0x20;}
   if((DispTab[n3]&0x04) == 0x04){ com = com + 0x40;}
   if((DispTab[n4]&0x04) == 0x04){ com = com + 0x80;}
   Ht1621WrOneData(6 , com); 
}
/********************************************************
函数名称：void Display_lcd_dot(void)
功能描述: 显示符号 。.  
全局变量：无
参数说明：无
返回说明：无
版 本：1.0
********************************************************/
void Display_lcd_dot(void) 
{ 
   Ht1621WrOneData(7 , 0x50);	   //.'	   2down 3 up
 //  Ht1621WrOneData(7 , 0x80);    //.   --> 1 down
  //Ht1621WrOneData(7 , 0x40);    //. --> 2 down
  // Ht1621WrOneData(7 , 0x20);    //. --> 2 up
   //Ht1621WrOneData(7 , 0x10);    //.:' --> 3 up
}


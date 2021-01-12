#include "sim900a.h"
#include "stdio.h"
#include "gpio.h"
#include "flash.h"
u8 Data_Buf[45];  // 必须定义的大一点否则snprintf函数越界
extern u8 ipaddr[];	//服务器IP
extern u8 port[];	//服务器端口
u16 rport=2305;	 //0~65535	//服务器
u8 i;
u8 RESTART;
extern u8 USART_RX_BUF[]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
extern u8 USART_RX_STA; 
extern u16 USART_RX_LEN; 
//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART_RX_STA)		//接收到一次数据了
	{ 
	//	USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
		USART_RX_STA=0;
		for(i=0;i<USART_RX_LEN;i++)
		{
			USART_RX_BUF[i]=0;
		}
		USART_RX_LEN=0;	 //	数组归零重新计数
	} 
	return (u8*)strx;
}
//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当0<cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//当cmd为0时只接收不发送
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	if(cmd!=0x00) 
	{	
		USART_RX_STA=0;//先清除标志位
		USART_RX_LEN=0;
	}
	if(((u32)cmd<=0XFF)&&((u32)cmd>0X00))
	{
		 USART1->DR=(u32)cmd;  //发送16进制数
	}
	else if((u32)cmd>0XFF)
		printf("%s\r\n",cmd);//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART_RX_STA)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				USART_RX_STA=0;	  //清空标志位
				USART_RX_LEN=0;	 //	数组归零重新计数
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//将1个字符转换为16进制数字
//chr:字符,0~9/A~F/a~F
//返回值:chr对应的16进制数值
u8 sim900a_chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}
//将1个16进制数字转换为字符
//hex:16进制数字,0~15;
//返回值:字符
u8 sim900a_hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
/*将字符串s转换成相应的10进制*/  
int sim900a_chr2dec(char s[])  
{  
    int i;  
    int n = 0;  
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)  
    {  
        n = 10 * n + (s[i] - '0');  
    }  
    return n;  
}
/*将10进制d转换成相应的字符*/
//如果高位有数则地位需要考虑零的情况 
void sim900a_dec2chr(int d)  
{  
    char flag=0;
	if(d/10000) 
	{ 
		flag=1;	
		printf("%d",d/10000);
	}
	if(d%10000/1000||(flag==1&&(d%10000/1000==0))) 
	{
		flag=1;
		printf("%d",d%10000/1000);
	} 
	if(d%10000%1000/100||(flag==1&&(d%10000%1000/100==0))) 
	{
		flag=1;
		printf("%d",d%10000%1000/100);
	}
	if(d%10000%1000%100/10||(flag==1&&(d%10000%1000%100/10==0))) 
	{
		flag=1;
		printf("%d",d%10000%1000%100/10);
	}
	if(d%10000%1000%100%10||(flag==1&&(d%10000%1000%100%10==0))) 
		printf("%d",d%10000%1000%100%10);
}
//初始化SIM900A 如果成功返回0  失败返回其他
void sim900a_init(void)
{
	for(i=0;i<3;i++)
	{
		LED1=1;
		delay_ms(500);
		LED1=0;
		RESTART++;
		sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//关闭连接
		sim900a_send_cmd("AT+CIPSHUT","SHUT OK",100);		//关闭移动场景 
		if(sim900a_send_cmd("AT+CGCLASS=\"B\"","OK",1000))	goto reinit;//设置GPRS移动台类别为B,支持包交换和数据交换 
		if(sim900a_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",500)) goto reinit;//设置PDP上下文,互联网接协议,接入点等信息
		if(sim900a_send_cmd("AT+CGATT=1","OK",500))	goto reinit;			//附着GPRS业务
		if(sim900a_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500))	goto reinit;//设置为GPRS连接模式
		
			
	//	if(sim900a_send_cmd("AT+CIPSTART=\"TCP\",\"60.214.198.63\",\"2305\"","OK",500)==0)		//发起连接
		snprintf((char*)Data_Buf,45,"AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",\"%d\"",ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3],rport);  //发起连接
		if(sim900a_send_cmd(Data_Buf,"OK",500)==0)
		{  
			if(sim900a_send_cmd((u8*)0X00,"CONNECT OK",1000)==0)	 //判断是否接收到  CONNECT OK
			{	
				LED0=0;//点亮LED0
				LED1=1;//灭LED1
		
				break;//初始化成功
			}
		}
		else goto reinit;
		reinit:
		delay_ms(1000);
		if(RESTART>=3)
		{
			RESTART=0;//重启
		}
	}
}

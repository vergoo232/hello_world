#include "system.h"
#include "usart.h"
#include "timerx.h"
//////////////////////////////////////////////////////////////////

u8 USART_RX_BUF[8]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART_RX_STA=0;
u8 USART_RX_LEN=0;
u8 OvertimeFlag=0;//等待超时标志位

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

void uart_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;//| USART_Mode_Rx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 
	USART_ClearFlag(USART1, USART_FLAG_TC);//防止第一个字节发不出去
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能 GPIOA 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能 USART2 时钟
	USART_DeInit(USART2);  //复位串口2
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //USART2_TX PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化 GPIOA.2
	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //USART2_RX PA.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化 GPIOA.3
	  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断   			
	USART_Cmd(USART2, ENABLE);                    //使能串口 
}
void USART_PutChar(USART_TypeDef* USARTx, uint8_t Data)  
{  
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); 
	USART_SendData(USARTx, Data);       
}  
void USART_PutStr(USART_TypeDef* USARTx, uint8_t *str)    
{    
    while (*str!=0)    
    {    
        USART_PutChar(USARTx, *str);    
        str++;    
    }  
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); 	
}
///*接收空闲中断 当接收完成时就会进入中断，特别适用于接收的长度不确定的情况下*/ 
void USART2_IRQHandler(void)                	//串口中断服务程序		////////////////////
{
	u8 Res;
	// 有两种情况接收完成 。一溢出 二 十毫秒没接收到数据
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != 0)  //接收中断 ///////////////////////
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据 //////////////////
		if(USART_RX_STA!=0xc0)//判断是否接受到回车换行符
		{
			if(USART_RX_STA&0x40)//已经接收到0x0d
			{
				if(Res==0x0a)
				{
					USART_RX_STA|=0x80;	//接收到0x0a  接收完成
					USART_RX_BUF[USART_RX_LEN]='\0'; //标记完成
				}
				else 
				{
					USART_RX_STA=0;//接收错误，重新接收					
					USART_RX_LEN=0;
				}
			}
			else //还没有接收到0x0d
			{	
				if(Res!=0x0d)
				{
					USART_RX_BUF[USART_RX_LEN++]=Res;	
					if(USART_RX_LEN>7)
					{
						USART_RX_STA=0;
						USART_RX_LEN=0;
					}
				}
				else
				{
					USART_RX_STA|=0x40;
				}
			}
		}
	}   		 
}	 
void CheckBusy(void)  
{
	OvertimeFlag=0;//清零
	TIM3_Set(1);//打开TIM3 
	while(!OvertimeFlag)
	{
		if(USART_RX_STA==0xc0&&USART_RX_BUF[0]=='O'&&USART_RX_BUF[1]=='K')
		{
			TIM3_Set(0);//关闭TIM3 
			USART_RX_BUF[0]=0;
			USART_RX_BUF[1]=0;
			USART_RX_STA=0;	
			USART_RX_LEN=0;
			break;				
		}				
	}
}

#include "can.h"
#include "delay.h"								  
//////////////////////////////////////////////////////////////////////////////////
/******************** ********************
 * 硬件连接：-----------------
 *          | 				  |
            |  PB8-CAN-RX     |
 *          |  PB9-CAN-TX     |
 *          |                 |
 *           -----------------
 //CAN初始化
//tsjw:重新同步跳跃时间单元.范围:1~3; CAN_SJW_1tq ~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.范围:1~8;	  CAN_BS2_1tq ~ CAN_BS2_8tq
//tbs1:时间段1的时间单元.范围:1~16;	  CAN_BS1_1tq ~ CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//注意以上参数任何一个都不能设为0,否则会乱.
//波特率=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,普通模式;1,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);
//则波特率为://36M/((1+10+7)*8)=250Kbps
//返回值:0,初始化OK;
//    其他,初始化失败;
*********************************************************/
void CAN_Mode_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure; 
	CAN_InitTypeDef	        CAN_InitStructure;
 	CAN_FilterInitTypeDef   CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

/*	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 //CANTX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	 //CANRX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化IO */
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* 复用功能和GPIOB端口时钟使能*/	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	/* Configure CAN pin: RX */	 // PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure CAN pin: TX */   // PB9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*重映射到 PB8 PB9*/
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);//重映射到I/O
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟 
 	//CAN单元设置
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //禁止时间触发通信模式 //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //禁止软件自动离线管理	//
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=DISABLE;						 //禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;	         //模式设置： mode:普通模式;回环模式; //
  	//设置波特率
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq ~ CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_10tq; //tbs1+1个时间单位 CAN_BS1_1tq ~ CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq; //tbs2+1个时间单位 CAN_BS2_1tq ~ CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=8;//分频系数(Fdiv)为brp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);// 初始化CAN1 
	//设置过滤器
 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//指定过滤器为标识符屏蔽位模式 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //过滤器位宽32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID// 过滤器标识符的高16位值
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//	 过滤器标识符的低16位值
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK//过滤器屏蔽标识符的高16位值
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;	//	过滤器屏蔽标识符的低16位值
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;//过滤器0关联到FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化

#if CAN_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
}   
//can发送一组数据(固定格式:ID为0X1200,扩展帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//其他,失败;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x00;					 // 标准标识符为0 在选择使用标准符时可用
	TxMessage.ExtId=0x0000;				 // 设置扩展标示符（29位）
	TxMessage.IDE=CAN_ID_EXT;			 // 使用扩展标识符
	TxMessage.RTR=CAN_RTR_DATA;		 // 消息类型为数据帧，一帧8位
	TxMessage.DLC=len;							 // 发送信息
	for(i=0;i<8;i++)
		TxMessage.Data[i]=msg[i];				 // 一帧信息          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)!=CAN_TxStatus_Failed)&&(i<0XFFF))	i++;	//等待发送结束
	if(i>=0XFFF)	return 1;
	return 0;		

}  
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}
#if CAN_RX0_INT_ENABLE	//使能RX0中断
//中断接收服务函数			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	int i=0;
    CAN_Receive(CAN1, 0, &RxMessage);
	for(i=0;i<8;i++)
	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);	 //中断接收到的数据放到了RxMessage.Data[i]中
}
#endif














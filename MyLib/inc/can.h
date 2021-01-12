#ifndef __CAN_H
#define __CAN_H	 
#include "system.h"	    

//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
										 							 				    
void CAN_Mode_Init(void);//CAN初始化
 
u8 Can_Send_Msg(u8* msg,u8 len);						//发送数据

u8 Can_Receive_Msg(u8 *buf);							//接收数据
void can_tx(u8 Data1,u8 Data2);
 void CAN_INIT(void);
 void can_tx(u8 Data1,u8 Data2);

void CAN_GPIO_Config(void);
#endif


















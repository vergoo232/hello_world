#ifndef __CAN_H
#define __CAN_H	 
#include "system.h"	    

//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
void CAN_Mode_Init(void);//CAN��ʼ��
 
u8 Can_Send_Msg(u8* msg,u8 len);						//��������

u8 Can_Receive_Msg(u8 *buf);							//��������
void can_tx(u8 Data1,u8 Data2);
 void CAN_INIT(void);
 void can_tx(u8 Data1,u8 Data2);

void CAN_GPIO_Config(void);
#endif


















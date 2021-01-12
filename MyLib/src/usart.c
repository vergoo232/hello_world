#include "system.h"
#include "usart.h"
#include "timerx.h"
//////////////////////////////////////////////////////////////////

u8 USART_RX_BUF[8]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u8 USART_RX_STA=0;
u8 USART_RX_LEN=0;
u8 OvertimeFlag=0;//�ȴ���ʱ��־λ

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;//| USART_Mode_Rx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
	USART_ClearFlag(USART1, USART_FLAG_TC);//��ֹ��һ���ֽڷ�����ȥ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ�� GPIOA ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ�� USART2 ʱ��
	USART_DeInit(USART2);  //��λ����2
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //USART2_TX PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ�� GPIOA.2
	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //USART2_RX PA.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ�� GPIOA.3
	  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�   			
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
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
///*���տ����ж� ���������ʱ�ͻ�����жϣ��ر������ڽ��յĳ��Ȳ�ȷ���������*/ 
void USART2_IRQHandler(void)                	//�����жϷ������		////////////////////
{
	u8 Res;
	// ���������������� ��һ��� �� ʮ����û���յ�����
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != 0)  //�����ж� ///////////////////////
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ������� //////////////////
		if(USART_RX_STA!=0xc0)//�ж��Ƿ���ܵ��س����з�
		{
			if(USART_RX_STA&0x40)//�Ѿ����յ�0x0d
			{
				if(Res==0x0a)
				{
					USART_RX_STA|=0x80;	//���յ�0x0a  �������
					USART_RX_BUF[USART_RX_LEN]='\0'; //������
				}
				else 
				{
					USART_RX_STA=0;//���մ������½���					
					USART_RX_LEN=0;
				}
			}
			else //��û�н��յ�0x0d
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
	OvertimeFlag=0;//����
	TIM3_Set(1);//��TIM3 
	while(!OvertimeFlag)
	{
		if(USART_RX_STA==0xc0&&USART_RX_BUF[0]=='O'&&USART_RX_BUF[1]=='K')
		{
			TIM3_Set(0);//�ر�TIM3 
			USART_RX_BUF[0]=0;
			USART_RX_BUF[1]=0;
			USART_RX_STA=0;	
			USART_RX_LEN=0;
			break;				
		}				
	}
}
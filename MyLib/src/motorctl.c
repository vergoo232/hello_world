///�������������Ϊpb4��pb5��pb6��pb7 ��1234��ʹ�õĵ������оƬΪL293DD  ʹ������Ϊpa15 �ߵ�ƽ��Ч
#include "motorctl.h"
#include "timerx.h"
extern u8 Direction;
void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //��PA PB ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //��PA15|PB3|PB4�����ͨIO��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //ENABLE-->PA15 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO����ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);						  //ʧ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	  //IN1-4-->PB4-7 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO����ٶ�50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);	
	
}
//Motor_CTL������ƺ���
//������� diection ���� 0��ʾͣ ��1��ʾ������2��ʾ��ת
//������� speed �ٶ� ��Χ1-100 Խ��Խ��
void Motor_CTL(u8 direction ,u8 speed)
{
	static u8 CurrentSpd=1;//��ʱ���浱ǰ�ٶ�
	static u8 CurrentDir=0;//��ʱ���浱ǰ����
	static char state=0;
	if(direction)
	{
		if(state==0)//��ʱ���ɹرյ���
		{
			state=1;
			Motor_EN=1;//��ʹ�ܶ�
			TIM3_Set(1);//�򿪶�ʱ��
		}
		if(CurrentSpd!=speed&&speed>0&&speed<=100)//����ٶȸı���
		{
			CurrentSpd=speed;//����
			TIM3_Change(speed-1);
		}
		if(CurrentDir!=direction)//�������ı���
		{
			CurrentDir=direction;
			Direction=direction;//���������ת��ֱ�Ӹ�ֵ
		}			
	}
	else//ͣ
	{
		state=0;
		Motor_EN=0;//�ر�ʹ�ܶ�
		TIM3_Set(0);//�رն�ʱ��
	}
}

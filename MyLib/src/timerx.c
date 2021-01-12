#include "timerx.h"
//�ö�ʱ��������������ȷ������������һ������
#include "usart.h"
#include "max6675.h"
#include "lcd.h"
#include "gpio.h"
//ͨ�ö�ʱ���жϳ�ʼ��
//Ԥ��Ƶϵ������1 ��Ƶ�ʳ�2��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
extern u8 OvertimeFlag;									//�ȴ���ʱ��־λ
extern u8 KeyOnFlag;									//Կ���ϵ��־λ
extern u8 IgnitionFlag;									//����־λ �ж��Ƿ���
extern u8 FlameoutFlag;									//Ϩ���־λ
extern u16 Temperature;									//�¶�
extern const u16 CRITICALTEMPERATURE;					//�����ٽ��¶�
extern u8 StatusFlag;									//��ǰ״̬
extern u8 TempDelayRun;									//�¶���ʱ���� 
u8 Tim4_cnt1=0,Tim4_cnt2=0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 	  
			 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); //����жϱ�־λ
	TIM_ITConfig( TIM3, TIM_IT_Update ,ENABLE );//ʹ�ܻ���ʧ��ָ����TIM�ж�
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
	
	TIM3_Set(0);					 
}
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 	  
			 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //����жϱ�־λ
	TIM_ITConfig( TIM4, TIM_IT_Update ,ENABLE );//ʹ�ܻ���ʧ��ָ����TIM�ж�
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����	 
}
//����TIM3�Ŀ���
//sta:0���ر�;1,����;
void TIM3_Set(u8 sta)
{
	if(sta)
	{
		TIM_SetCounter(TIM3,0);//���������
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	
	}
	else 
		TIM_Cmd(TIM3, DISABLE);//�رն�ʱ��
}
//10msһ�� ˵��û��10ms��û�н��յ����� ��������ɱ�־λ ��һ//
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
		TIM3_Set(0);			//�ر�TIM3 
		OvertimeFlag=1;
	}
}
void TIM4_IRQHandler(void)   //TIM4�ж�  1s��ʱ�� ��ʱˢ��
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
		LED=!LED;
		if(KeyOnFlag==0)
		{
			if(Tim4_cnt1<9)//10s
			{
				Tim4_cnt1++;
			}
			else
			{
				Tim4_cnt1=0;
				KeyOnFlag=1;
				if(StatusFlag==0)//�ж��Ƿ���Ԥ���ȹ�����
				{
					StatusFlag=1;//Ԥ�������
				}
			}
		}
		if(FlameoutFlag==1)//Ϩ���־λ
		{
			if(Tim4_cnt2<30)//30s
			{
				Tim4_cnt2++;
			}
			else if(Temperature<CRITICALTEMPERATURE)//���¶ȵ�300�ҵ���30s�Ժ�
			{
				Tim4_cnt2=0;
				FlameoutFlag=0;//Ϩ���־λ
				if(StatusFlag==2||StatusFlag==3)//�����ж�Ϩ��֮������ֵ������
				{
					AXPowerCtl=0;//�жϵ�ع��� ����������ʱ��ϵ���
					if(IsKeyOn==1)//��û�жϵ�
					{
						StatusFlag=1;//������е����ָ���Ԥ������Ժ�
					}
				}
			}
			else
			{
				StatusFlag=3;
			}
		}
		
	}
}

#include "timerx.h"
//该定时器的作用是用于确定串口上收完一组数据
#include "usart.h"
#include "max6675.h"
#include "lcd.h"
#include "gpio.h"
//通用定时器中断初始化
//预分频系数不是1 则频率乘2，
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
extern u8 OvertimeFlag;									//等待超时标志位
extern u8 KeyOnFlag;									//钥匙上电标志位
extern u8 IgnitionFlag;									//点火标志位 判断是否点火
extern u8 FlameoutFlag;									//熄火标志位
extern u16 Temperature;									//温度
extern const u16 CRITICALTEMPERATURE;					//定义临界温度
extern u8 StatusFlag;									//当前状态
extern u8 TempDelayRun;									//温度延时启动 
u8 Tim4_cnt1=0,Tim4_cnt2=0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 	  
			 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); //清除中断标志位
	TIM_ITConfig( TIM3, TIM_IT_Update ,ENABLE );//使能或者失能指定的TIM中断
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	
	TIM3_Set(0);					 
}
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 	  
			 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除中断标志位
	TIM_ITConfig( TIM4, TIM_IT_Update ,ENABLE );//使能或者失能指定的TIM中断
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设	 
}
//设置TIM3的开关
//sta:0，关闭;1,开启;
void TIM3_Set(u8 sta)
{
	if(sta)
	{
		TIM_SetCounter(TIM3,0);//计数器清空
		TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
	}
	else 
		TIM_Cmd(TIM3, DISABLE);//关闭定时器
}
//10ms一到 说明没有10ms内没有接收到数据 将接收完成标志位 置一//
void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
		TIM3_Set(0);			//关闭TIM3 
		OvertimeFlag=1;
	}
}
void TIM4_IRQHandler(void)   //TIM4中断  1s定时器 定时刷新
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
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
				if(StatusFlag==0)//判断是否还在预加热过程中
				{
					StatusFlag=1;//预加热完成
				}
			}
		}
		if(FlameoutFlag==1)//熄火标志位
		{
			if(Tim4_cnt2<30)//30s
			{
				Tim4_cnt2++;
			}
			else if(Temperature<CRITICALTEMPERATURE)//在温度低300且到达30s以后
			{
				Tim4_cnt2=0;
				FlameoutFlag=0;//熄火标志位
				if(StatusFlag==2||StatusFlag==3)//用于判断熄火之后接着又点火的情况
				{
					AXPowerCtl=0;//切断电池供电 正常情况这个时候断电了
					if(IsKeyOn==1)//还没有断电
					{
						StatusFlag=1;//如果还有电程序恢复到预润滑完成以后
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

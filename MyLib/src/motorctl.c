///电机的输入引脚为pb4、pb5、pb6、pb7 （1234）使用的电机控制芯片为L293DD  使能引脚为pa15 高电平有效
#include "motorctl.h"
#include "timerx.h"
extern u8 Direction;
void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //打开PA PB 的时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //将PA15|PB3|PB4设成普通IO口
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //ENABLE-->PA15 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO输出速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);						  //失能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	  //IN1-4-->PB4-7 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO输出速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);	
	
}
//Motor_CTL电机控制函数
//输入参数 diection 方向 0表示停 ，1表示正传，2表示反转
//输入参数 speed 速度 范围1-100 越大越慢
void Motor_CTL(u8 direction ,u8 speed)
{
	static u8 CurrentSpd=1;//临时储存当前速度
	static u8 CurrentDir=0;//临时储存当前方向
	static char state=0;
	if(direction)
	{
		if(state==0)//定时器由关闭到打开
		{
			state=1;
			Motor_EN=1;//打开使能端
			TIM3_Set(1);//打开定时器
		}
		if(CurrentSpd!=speed&&speed>0&&speed<=100)//如果速度改变了
		{
			CurrentSpd=speed;//保存
			TIM3_Change(speed-1);
		}
		if(CurrentDir!=direction)//如果方向改变了
		{
			CurrentDir=direction;
			Direction=direction;//如果是正反转则直接赋值
		}			
	}
	else//停
	{
		state=0;
		Motor_EN=0;//关闭使能端
		TIM3_Set(0);//关闭定时器
	}
}

#include "gpio.h"
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 							//打开PB 的时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_10|GPIO_Pin_11;	 				// LED-->PB6   MotorCtl pb10 Battery-->Pb11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									//IO输出速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_10|GPIO_Pin_11);							//输出低
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;											//KeyOn
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 										//设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);												//初始化
}

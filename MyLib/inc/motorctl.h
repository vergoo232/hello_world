#ifndef __MOTORCTL_H
#define __MOTORCTL_H	 
#include "system.h"

#define Motor_EN PAout(15)	// PA15	
#define Motor_Step1 {PBout(4)=0;PBout(5)=0;PBout(6)=1;PBout(7)=0;}
#define Motor_Step2 {PBout(4)=1;PBout(5)=0;PBout(6)=1;PBout(7)=0;}
#define Motor_Step3 {PBout(4)=1;PBout(5)=0;PBout(6)=0;PBout(7)=0;}
#define Motor_Step4 {PBout(4)=1;PBout(5)=0;PBout(6)=0;PBout(7)=1;}
#define Motor_Step5 {PBout(4)=0;PBout(5)=0;PBout(6)=0;PBout(7)=1;}
#define Motor_Step6 {PBout(4)=0;PBout(5)=1;PBout(6)=0;PBout(7)=1;}
#define Motor_Step7 {PBout(4)=0;PBout(5)=1;PBout(6)=0;PBout(7)=0;}
#define Motor_Step8 {PBout(4)=0;PBout(5)=1;PBout(6)=1;PBout(7)=0;}
void Motor_Init(void);//电机初始化配置
void Motor_CTL(u8 direction ,u8 speed);//电机方向控制函数
		 				    
#endif

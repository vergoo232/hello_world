#include "system.h"
//中断优先级分组，  指定抢占优先级，指定响应优先级
//该函数用于你对整个程序进行的中断进行管理，优先级的分配问题
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2  :  2位抢占优先级，2位响应优先级

}

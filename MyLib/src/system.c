#include "system.h"
//�ж����ȼ����飬  ָ����ռ���ȼ���ָ����Ӧ���ȼ�
//�ú��������������������е��жϽ��й������ȼ��ķ�������
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2  :  2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}

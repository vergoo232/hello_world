#include "sim900a.h"
#include "stdio.h"
#include "gpio.h"
#include "flash.h"
u8 Data_Buf[45];  // ���붨��Ĵ�һ�����snprintf����Խ��
extern u8 ipaddr[];	//������IP
extern u8 port[];	//�������˿�
u16 rport=2305;	 //0~65535	//������
u8 i;
u8 RESTART;
extern u8 USART_RX_BUF[]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
extern u8 USART_RX_STA; 
extern u16 USART_RX_LEN; 
//sim900a���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART_RX_STA)		//���յ�һ��������
	{ 
	//	USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
		USART_RX_STA=0;
		for(i=0;i<USART_RX_LEN;i++)
		{
			USART_RX_BUF[i]=0;
		}
		USART_RX_LEN=0;	 //	����������¼���
	} 
	return (u8*)strx;
}
//��sim900a��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��0<cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//��cmdΪ0ʱֻ���ղ�����
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	if(cmd!=0x00) 
	{	
		USART_RX_STA=0;//�������־λ
		USART_RX_LEN=0;
	}
	if(((u32)cmd<=0XFF)&&((u32)cmd>0X00))
	{
		 USART1->DR=(u32)cmd;  //����16������
	}
	else if((u32)cmd>0XFF)
		printf("%s\r\n",cmd);//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART_RX_STA)//���յ��ڴ���Ӧ����
			{
				if(sim900a_check_cmd(ack))break;//�õ���Ч���� 
				USART_RX_STA=0;	  //��ձ�־λ
				USART_RX_LEN=0;	 //	����������¼���
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��1���ַ�ת��Ϊ16��������
//chr:�ַ�,0~9/A~F/a~F
//����ֵ:chr��Ӧ��16������ֵ
u8 sim900a_chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}
//��1��16��������ת��Ϊ�ַ�
//hex:16��������,0~15;
//����ֵ:�ַ�
u8 sim900a_hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
/*���ַ���sת������Ӧ��10����*/  
int sim900a_chr2dec(char s[])  
{  
    int i;  
    int n = 0;  
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)  
    {  
        n = 10 * n + (s[i] - '0');  
    }  
    return n;  
}
/*��10����dת������Ӧ���ַ�*/
//�����λ�������λ��Ҫ���������� 
void sim900a_dec2chr(int d)  
{  
    char flag=0;
	if(d/10000) 
	{ 
		flag=1;	
		printf("%d",d/10000);
	}
	if(d%10000/1000||(flag==1&&(d%10000/1000==0))) 
	{
		flag=1;
		printf("%d",d%10000/1000);
	} 
	if(d%10000%1000/100||(flag==1&&(d%10000%1000/100==0))) 
	{
		flag=1;
		printf("%d",d%10000%1000/100);
	}
	if(d%10000%1000%100/10||(flag==1&&(d%10000%1000%100/10==0))) 
	{
		flag=1;
		printf("%d",d%10000%1000%100/10);
	}
	if(d%10000%1000%100%10||(flag==1&&(d%10000%1000%100%10==0))) 
		printf("%d",d%10000%1000%100%10);
}
//��ʼ��SIM900A ����ɹ�����0  ʧ�ܷ�������
void sim900a_init(void)
{
	for(i=0;i<3;i++)
	{
		LED1=1;
		delay_ms(500);
		LED1=0;
		RESTART++;
		sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//�ر�����
		sim900a_send_cmd("AT+CIPSHUT","SHUT OK",100);		//�ر��ƶ����� 
		if(sim900a_send_cmd("AT+CGCLASS=\"B\"","OK",1000))	goto reinit;//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
		if(sim900a_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",500)) goto reinit;//����PDP������,��������Э��,��������Ϣ
		if(sim900a_send_cmd("AT+CGATT=1","OK",500))	goto reinit;			//����GPRSҵ��
		if(sim900a_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500))	goto reinit;//����ΪGPRS����ģʽ
		
			
	//	if(sim900a_send_cmd("AT+CIPSTART=\"TCP\",\"60.214.198.63\",\"2305\"","OK",500)==0)		//��������
		snprintf((char*)Data_Buf,45,"AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",\"%d\"",ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3],rport);  //��������
		if(sim900a_send_cmd(Data_Buf,"OK",500)==0)
		{  
			if(sim900a_send_cmd((u8*)0X00,"CONNECT OK",1000)==0)	 //�ж��Ƿ���յ�  CONNECT OK
			{	
				LED0=0;//����LED0
				LED1=1;//��LED1
		
				break;//��ʼ���ɹ�
			}
		}
		else goto reinit;
		reinit:
		delay_ms(1000);
		if(RESTART>=3)
		{
			RESTART=0;//����
		}
	}
}

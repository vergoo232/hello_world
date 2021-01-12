#include "max6675.h"
#include "system.h"
#include "spi.h"
#include "delay.h"
u8 DisconnectFlag=0;//���߱�־λ
u16 Temperature=25;

void Read_Temperature(void)
{
	u8 i,j;
	u16 Tempbuf[11]={0};
	u16 HighByte=0,LowByte=0,RawData=0,DealData=0,temp;//���ݸ߰�λ�͵ڰ�λ ԭʼ����
	for(i=0;i<11;i++)
	{
		SPI_FLASH_CS_LOW();//Ƭѡ����
		HighByte=SPI_FLASH_ReadByte();//��ȡ��8λ
		HighByte=HighByte<<8;
		LowByte=SPI_FLASH_ReadByte();//��ȡ��8λ
		SPI_FLASH_CS_HIGH();//Ƭѡ����
		RawData=HighByte|LowByte;//���ԭʼ����
		DisconnectFlag=RawData&0x04;
		if(DisconnectFlag==0&&RawData!=0)//��·�������Ҷ�������
		{
			DealData=RawData<<1;//ȥ��D15
			DealData=DealData>>4;//ȥ��D0 D1 D2
			Tempbuf[i]=(u16)(DealData*0.25);//��ȡ��ʵ�¶�
			delay_ms(5);
		}
		else 
		{
			return;
		}
	}
	for (j=0;j<=11;j++)
	{
		for (i=0;i<=11-j;i++)
		{
			if (Tempbuf[i] > Tempbuf[i+1])
			{
				temp = Tempbuf[i];
				Tempbuf[i] = Tempbuf[i+1];
				Tempbuf[i+1] = temp;
			}
		}
	}
	Temperature=Tempbuf[5];
}


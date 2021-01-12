#include "max6675.h"
#include "system.h"
#include "spi.h"
#include "delay.h"
u8 DisconnectFlag=0;//断线标志位
u16 Temperature=25;

void Read_Temperature(void)
{
	u8 i,j;
	u16 Tempbuf[11]={0};
	u16 HighByte=0,LowByte=0,RawData=0,DealData=0,temp;//数据高八位和第八位 原始数据
	for(i=0;i<11;i++)
	{
		SPI_FLASH_CS_LOW();//片选拉低
		HighByte=SPI_FLASH_ReadByte();//读取高8位
		HighByte=HighByte<<8;
		LowByte=SPI_FLASH_ReadByte();//读取高8位
		SPI_FLASH_CS_HIGH();//片选拉高
		RawData=HighByte|LowByte;//获得原始数据
		DisconnectFlag=RawData&0x04;
		if(DisconnectFlag==0&&RawData!=0)//线路正常并且读到数据
		{
			DealData=RawData<<1;//去掉D15
			DealData=DealData>>4;//去掉D0 D1 D2
			Tempbuf[i]=(u16)(DealData*0.25);//获取真实温度
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


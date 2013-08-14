#include "DS18B20.h"

uchar tplsb,tpmsb;		// 温度值低位、高位字节 
unsigned int tpl;

/* 延时t毫秒 */
void delay(uint t)
{
	uint i;
	while(t--)
	{
		/* 对于11.0592M时钟，约延时1ms */
		for (i=0;i<125;i++)
		{}
	}
} 

/* 产生复位脉冲初始化DS18B20 */
void TxReset(void)
{
	uint i;
	DQ = 0;

	/* 拉低约900us */
	i = 100;
	while (i>0)	i--;	
	
	DQ = 1;				// 产生上升沿
	i = 4;
	while (i>0)	i--;
}

/* 等待应答脉冲 */
void RxWait(void)
{
	uint i;
	while(DQ);
	while(~DQ);			// 检测到应答脉冲 
	i = 4;
	while (i>0)	i--;
}

/* 读取数据的一位，满足读时隙要求 */ 
bit RdBit(void)
{
	uint i;
	bit b;
	DQ = 0;
	i++;
	DQ = 1;
	i++;i++;			// 延时15us以上，读时隙下降沿后15us，DS18B20输出数据才有效
	b = DQ;
	i = 8;
	while(i>0) i--;
	return (b);
}

/* 读取数据的一个字节 */
uchar RdByte(void)
{
	uchar i,j,b;
	b = 0;
	for (i=1;i<=8;i++)
	{
		j = RdBit();
		b = (j<<7)|(b>>1);
	}
	return(b);
}

/* 写数据的一个字节，满足写1和写0的时隙要求 */
void WrByte(uchar b)
{
	uint i;
	uchar j;
	bit btmp;
	for(j=1;j<=8;j++)
	{
		btmp = b&0x01;
		b = b>>1;		// 取下一位（由低位向高位）
		if (btmp)
		{
			/* 写1 */
			DQ = 0;
			i++;i++;	// 延时，使得15us以内拉高
			DQ = 1;
			i = 8;
			while(i>0) i--;	// 整个写1时隙不低于60us 
	   	}
		else
		{
			/* 写0 */
			DQ = 0;			
			i = 8;
			while(i>0) i--;	// 保持低在60us到120us之间
			DQ = 1;
			i++;
			i++;
		}
	}
}

/* 启动温度转换 */
void convert(void)
{
	TxReset();			// 产生复位脉冲，初始化DS18B20
	RxWait();			// 等待DS18B20给出应答脉冲
	delay(1);			// 延时
	WrByte(0xcc);		// skip rom 命令
	WrByte(0x44);		// convert T 命令
}

/* 读取温度值 */
unsigned int RdTemp(void)
{
  float tt = 0;
	TxReset();			// 产生复位脉冲，初始化DS18B20 
	RxWait();			// 等待DS18B20给出应答脉冲
	delay(1);			// 延时
	WrByte(0xcc);		// skip rom 命令
	WrByte(0xbe);		// read scratchpad 命令
	tplsb = RdByte();	// 温度值低位字节（其中低4位为二进制的“小数”部分）
	tpmsb = RdByte();	// 高位值高位字节（其中高5位为符号位）
  tpl = tpmsb;
  tpl <<= 8;
  tpl = tpl | tplsb;
  tt = tpl * 0.0625;
  tpl = tt * 10 + 0.5;
  return tpl;
}

/* 主程序，读取的温度值最终存放在tplsb和tpmsb变量中。
   tplsb其中低4位为二进制的“小数”部分；tpmsb其中高
   5位为符号位。真正通过数码管输出时，需要进行到十进
   制有符号实数（包括小数部分）的转换。              */
unsigned int ReadTemperature(void)
{
	delay(1);		// 延时1ms
	convert();		// 启动温度转换，需要750ms
	delay(1000);	// 延时1s
	return RdTemp();		// 读取温度			
}


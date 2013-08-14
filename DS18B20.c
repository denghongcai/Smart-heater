#include "DS18B20.h"

uchar tplsb,tpmsb;		// �¶�ֵ��λ����λ�ֽ� 
unsigned int tpl;

/* ��ʱt���� */
void delay(uint t)
{
	uint i;
	while(t--)
	{
		/* ����11.0592Mʱ�ӣ�Լ��ʱ1ms */
		for (i=0;i<125;i++)
		{}
	}
} 

/* ������λ�����ʼ��DS18B20 */
void TxReset(void)
{
	uint i;
	DQ = 0;

	/* ����Լ900us */
	i = 100;
	while (i>0)	i--;	
	
	DQ = 1;				// ����������
	i = 4;
	while (i>0)	i--;
}

/* �ȴ�Ӧ������ */
void RxWait(void)
{
	uint i;
	while(DQ);
	while(~DQ);			// ��⵽Ӧ������ 
	i = 4;
	while (i>0)	i--;
}

/* ��ȡ���ݵ�һλ�������ʱ϶Ҫ�� */ 
bit RdBit(void)
{
	uint i;
	bit b;
	DQ = 0;
	i++;
	DQ = 1;
	i++;i++;			// ��ʱ15us���ϣ���ʱ϶�½��غ�15us��DS18B20������ݲ���Ч
	b = DQ;
	i = 8;
	while(i>0) i--;
	return (b);
}

/* ��ȡ���ݵ�һ���ֽ� */
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

/* д���ݵ�һ���ֽڣ�����д1��д0��ʱ϶Ҫ�� */
void WrByte(uchar b)
{
	uint i;
	uchar j;
	bit btmp;
	for(j=1;j<=8;j++)
	{
		btmp = b&0x01;
		b = b>>1;		// ȡ��һλ���ɵ�λ���λ��
		if (btmp)
		{
			/* д1 */
			DQ = 0;
			i++;i++;	// ��ʱ��ʹ��15us��������
			DQ = 1;
			i = 8;
			while(i>0) i--;	// ����д1ʱ϶������60us 
	   	}
		else
		{
			/* д0 */
			DQ = 0;			
			i = 8;
			while(i>0) i--;	// ���ֵ���60us��120us֮��
			DQ = 1;
			i++;
			i++;
		}
	}
}

/* �����¶�ת�� */
void convert(void)
{
	TxReset();			// ������λ���壬��ʼ��DS18B20
	RxWait();			// �ȴ�DS18B20����Ӧ������
	delay(1);			// ��ʱ
	WrByte(0xcc);		// skip rom ����
	WrByte(0x44);		// convert T ����
}

/* ��ȡ�¶�ֵ */
unsigned int RdTemp(void)
{
  float tt = 0;
	TxReset();			// ������λ���壬��ʼ��DS18B20 
	RxWait();			// �ȴ�DS18B20����Ӧ������
	delay(1);			// ��ʱ
	WrByte(0xcc);		// skip rom ����
	WrByte(0xbe);		// read scratchpad ����
	tplsb = RdByte();	// �¶�ֵ��λ�ֽڣ����е�4λΪ�����Ƶġ�С�������֣�
	tpmsb = RdByte();	// ��λֵ��λ�ֽڣ����и�5λΪ����λ��
  tpl = tpmsb;
  tpl <<= 8;
  tpl = tpl | tplsb;
  tt = tpl * 0.0625;
  tpl = tt * 10 + 0.5;
  return tpl;
}

/* �����򣬶�ȡ���¶�ֵ���մ����tplsb��tpmsb�����С�
   tplsb���е�4λΪ�����Ƶġ�С�������֣�tpmsb���и�
   5λΪ����λ������ͨ����������ʱ����Ҫ���е�ʮ��
   ���з���ʵ��������С�����֣���ת����              */
unsigned int ReadTemperature(void)
{
	delay(1);		// ��ʱ1ms
	convert();		// �����¶�ת������Ҫ750ms
	delay(1000);	// ��ʱ1s
	return RdTemp();		// ��ȡ�¶�			
}


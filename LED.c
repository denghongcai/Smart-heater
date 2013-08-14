#include "LED.h"

typedef unsigned char	uint8;
typedef unsigned int	uint16;

unsigned char code number[] = { 0x3f, 0x06, 0x5b, 0x4f, /* ����������� */
			   0x66, 0x6d, 0x7d, 0x07,
			   0x7f, 0x6f, 0X77, 0x7c,
			   0x39, 0x5e, 0x79, 0x71 };

sbit	DU	= P2 ^ 6;
sbit	WE	= P2 ^ 7;


void showTpl(unsigned int data_led0, unsigned int data_led1)
{
	uint16 counter;
	TH0	= 0xB8; /* �趨20MS��ʱ */
	TL0	= 0X00;
	TR0	= 1;
  counter = 0;
	while ( 1 )
	{
		if ( TF0 == 1 )
		{
			TF0	= 0;
			TH0	= 0xB8;
			TL0	= 0x00;
			counter++;
		}
    WE	= 1;    /* WE==1ʱ�򣬱�ʾ��������һ�������ݲ����档 */
    P0	= 0xfe; /* ����λѡ�ź� Ӧ����ȫ��λ��ѡͨ��Ӧ��ȫ������ */
    WE	= 0;    /* WE==0ʱ�򣬱�ʾ���벻�ı�������������档 */
    P0  = number[data_led0];
    DU  = 1;
    DU  = 0;
    P0  = 0x00;
    DU  = 1;
    DU  = 0;
    WE	= 1;    /* WE==1ʱ�򣬱�ʾ��������һ�������ݲ����档 */
    P0	= 0xfd; /* ����λѡ�ź� Ӧ����ȫ��λ��ѡͨ��Ӧ��ȫ������ */
    WE	= 0;    /* WE==0ʱ�򣬱�ʾ���벻�ı�������������档 */
    P0  = number[data_led1];
    DU  = 1;
    DU  = 0;
    P0  = 0x00;
    DU  = 1;
    DU  = 0;
		if(counter == 1000) break;
	}
}

void showHeatime(unsigned int data_led0, unsigned int data_led1)
{
  uint16 counter;
	TH0	= 0xB8; /* �趨20MS��ʱ */
	TL0	= 0X00;
	TR0	= 1;
  counter = 0;
	while ( 1 )
	{
		if ( TF0 == 1 )
		{
			TF0	= 0;
			TH0	= 0xB8;
			TL0	= 0x00;
			counter++;
		}
    WE	= 1;    /* WE==1ʱ�򣬱�ʾ��������һ�������ݲ����档 */
    P0	= 0xfe; /* ����λѡ�ź� Ӧ����ȫ��λ��ѡͨ��Ӧ��ȫ������ */
    WE	= 0;    /* WE==0ʱ�򣬱�ʾ���벻�ı�������������档 */
    P0  = number[data_led0];
    DU  = 1;
    DU  = 0;
    P0  = 0x00;
    DU  = 1;
    DU  = 0;
    WE	= 1;    /* WE==1ʱ�򣬱�ʾ��������һ�������ݲ����档 */
    P0	= 0xfd; /* ����λѡ�ź� Ӧ����ȫ��λ��ѡͨ��Ӧ��ȫ������ */
    WE	= 0;    /* WE==0ʱ�򣬱�ʾ���벻�ı�������������档 */
    P0  = number[data_led1];
    DU  = 1;
    DU  = 0;
    P0  = 0x00;
    DU  = 1;
    DU  = 0;
		if(counter == 1000) break;
	}
}

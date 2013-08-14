#include "LED.h"

typedef unsigned char	uint8;
typedef unsigned int	uint16;

unsigned char code number[] = { 0x3f, 0x06, 0x5b, 0x4f, /* 共阴极数码管 */
			   0x66, 0x6d, 0x7d, 0x07,
			   0x7f, 0x6f, 0X77, 0x7c,
			   0x39, 0x5e, 0x79, 0x71 };

sbit	DU	= P2 ^ 6;
sbit	WE	= P2 ^ 7;


void showTpl(unsigned int data_led0, unsigned int data_led1)
{
	uint16 counter;
	TH0	= 0xB8; /* 设定20MS延时 */
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
    WE	= 1;    /* WE==1时候，表示输入和输出一样，数据不锁存。 */
    P0	= 0xfe; /* 送入位选信号 应该是全部位都选通（应该全部亮） */
    WE	= 0;    /* WE==0时候，表示输入不改变输出，数据锁存。 */
    P0  = number[data_led0];
    DU  = 1;
    DU  = 0;
    P0  = 0x00;
    DU  = 1;
    DU  = 0;
    WE	= 1;    /* WE==1时候，表示输入和输出一样，数据不锁存。 */
    P0	= 0xfd; /* 送入位选信号 应该是全部位都选通（应该全部亮） */
    WE	= 0;    /* WE==0时候，表示输入不改变输出，数据锁存。 */
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
	TH0	= 0xB8; /* 设定20MS延时 */
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
    WE	= 1;    /* WE==1时候，表示输入和输出一样，数据不锁存。 */
    P0	= 0xfe; /* 送入位选信号 应该是全部位都选通（应该全部亮） */
    WE	= 0;    /* WE==0时候，表示输入不改变输出，数据锁存。 */
    P0  = number[data_led0];
    DU  = 1;
    DU  = 0;
    P0  = 0x00;
    DU  = 1;
    DU  = 0;
    WE	= 1;    /* WE==1时候，表示输入和输出一样，数据不锁存。 */
    P0	= 0xfd; /* 送入位选信号 应该是全部位都选通（应该全部亮） */
    WE	= 0;    /* WE==0时候，表示输入不改变输出，数据锁存。 */
    P0  = number[data_led1];
    DU  = 1;
    DU  = 0;
    P0  = 0x00;
    DU  = 1;
    DU  = 0;
		if(counter == 1000) break;
	}
}

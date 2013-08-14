#include "KEY.h"

sbit	k1	= P3 ^ 2;
sbit	k2	= P3 ^ 3;               /*  */
sbit	led	= P1 ^ 2;
sbit  rd  = P3 ^ 6;
sbit  ledMin = P1 ^ 5;
sbit Relay = P2^4;

volatile unsigned int t = 0;

volatile unsigned int heatimesend = 1;

/* ================================================= */
void Key_init()
{
	P1	= 0xff;                 /*上电初始化，P1全灭 */
	led	= 1;                    /*上电初始化，led灯不亮 */
  ledMin = 1;
	TCON	= 0x05;                 /* 打开外部中断1，并设置为下降沿触发 */
	IE	= 0x85;                 /* 开总中断和外部中断1 */
  rd = 0;
}


/* ================================================= */
void delay_ms( unsigned int xms ) /* ms级延时子程序 */
{
	unsigned int x, y;
	for ( x = xms; x > 0; x-- )
		for ( y = 130; y > 0; y-- )
			;
}


/* ------------------------------------------------- */
void key_scan_1() interrupt 0           /* 使用了外部中断0的键盘扫描子函数 */
{
	if ( k1 == 0 )                  /*有键按下吗？（k1=0 ?） */
	{
		delay_ms( 50 );         /* 延时消抖 */
		if ( k1 == 0 )          /* 确实是有键按下，则： */
		{
      mode = 1;
      RCAP2H =0x68;
      RCAP2L =0x00;
      TR2=1;                       //启动定时器
      ET2=1;                       //打开定时器2中断
      t = 0;
			if(heatimeValue <60)
        heatimeValue += 5;/* 增加heatimeValue的状态 */
      else
        heatimeValue = 0;
			while ( !k1 );
      Relay = 1;
      heatimesend = 0;
		} /* 等待按键放开 */
	}
}


void key_scan_2() interrupt 2           /* 使用了外部中断1的键盘扫描子函数 */
{
	if ( k2 == 0 )                  /*有键按下吗？（k2=0 ?） */
	{
		delay_ms( 50 );         /* 延时消抖 */
		if ( k2 == 0 )          /* 确实是有键按下，则： */
		{
      printf("+++"); //退出wifi透传模式
      delay_ms( 1000 ); 
			printf("AT+RSTF\r\n"); //出厂设置
      delay_ms( 1000 );
      printf("AT+Z\r\n"); //复位wifi
				;
		} /* 等待按键放开 */
	}
}

void heatimeCD() interrupt 5
{
  TF2 = 0;
  ++t;
  if(t == 40)
  {
    ledMin = !ledMin;
    t = 0;
    ++countdownMin;
  }
}

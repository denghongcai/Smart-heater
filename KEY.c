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
	P1	= 0xff;                 /*�ϵ��ʼ����P1ȫ�� */
	led	= 1;                    /*�ϵ��ʼ����led�Ʋ��� */
  ledMin = 1;
	TCON	= 0x05;                 /* ���ⲿ�ж�1��������Ϊ�½��ش��� */
	IE	= 0x85;                 /* �����жϺ��ⲿ�ж�1 */
  rd = 0;
}


/* ================================================= */
void delay_ms( unsigned int xms ) /* ms����ʱ�ӳ��� */
{
	unsigned int x, y;
	for ( x = xms; x > 0; x-- )
		for ( y = 130; y > 0; y-- )
			;
}


/* ------------------------------------------------- */
void key_scan_1() interrupt 0           /* ʹ�����ⲿ�ж�0�ļ���ɨ���Ӻ��� */
{
	if ( k1 == 0 )                  /*�м������𣿣�k1=0 ?�� */
	{
		delay_ms( 50 );         /* ��ʱ���� */
		if ( k1 == 0 )          /* ȷʵ���м����£��� */
		{
      mode = 1;
      RCAP2H =0x68;
      RCAP2L =0x00;
      TR2=1;                       //������ʱ��
      ET2=1;                       //�򿪶�ʱ��2�ж�
      t = 0;
			if(heatimeValue <60)
        heatimeValue += 5;/* ����heatimeValue��״̬ */
      else
        heatimeValue = 0;
			while ( !k1 );
      Relay = 1;
      heatimesend = 0;
		} /* �ȴ������ſ� */
	}
}


void key_scan_2() interrupt 2           /* ʹ�����ⲿ�ж�1�ļ���ɨ���Ӻ��� */
{
	if ( k2 == 0 )                  /*�м������𣿣�k2=0 ?�� */
	{
		delay_ms( 50 );         /* ��ʱ���� */
		if ( k2 == 0 )          /* ȷʵ���м����£��� */
		{
      printf("+++"); //�˳�wifi͸��ģʽ
      delay_ms( 1000 ); 
			printf("AT+RSTF\r\n"); //��������
      delay_ms( 1000 );
      printf("AT+Z\r\n"); //��λwifi
				;
		} /* �ȴ������ſ� */
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

#include <reg52.h>
#include <stdio.h>

void Key_init();
void delay_ms( unsigned int xms );      /* ms����ʱ�ӳ��� */

extern volatile unsigned int heatimeValue;

extern volatile unsigned int mode;

extern volatile unsigned int countdownMin;

extern volatile unsigned int t;
#include <reg52.h>
#include <string.h>
#include <stdio.h>

extern bit read_flag;
extern volatile unsigned char RECEIVR_buffer[30];

void UART_init();                //���ڳ�ʼ������
void COM_send(unsigned int);             //���ڷ��ͺ���
char putchar (char ucWriteData);   //���ؿ⺯��
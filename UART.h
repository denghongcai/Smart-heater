#include <reg52.h>
#include <string.h>
#include <stdio.h>

extern bit read_flag;
extern volatile unsigned char RECEIVR_buffer[30];

void UART_init();                //串口初始化函数
void COM_send(unsigned int);             //串口发送函数
char putchar (char ucWriteData);   //重载库函数
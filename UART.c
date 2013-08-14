#include "UART.h"

//-------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// 函数名称： UART_init()串口初始化函数
// 函数功能： 在系统时钟为11.059MHZ时，设定串口波特率为9600bit/s
//             串口接收中断允许，发送中断禁止
//--------------------------------------------------------------------------------------------------

volatile unsigned char UartIsSending;
volatile unsigned char RECEIVR_buffer[30];
volatile unsigned int i = 0;
bit read_flag = 0;

void UART_init()
{
                        //初始化串行口和波特率发生器
  SCON =0x50;           //选择串口工作方式1，打开接收允许
  TMOD =0x20;           //定时器1工作在方式2，定时器0工作在方式1
  TH1 =0xfA;            //实现波特率9600（系统时钟11.0592MHZ）
  PCON = 0x80;
  TR1 =1;               //启动定时器T1
  ET1 =0;
  ES=1;                 //允许串行口中断
  PS=1;                 //设计串行口中断优先级
  EA =1;                //单片机中断允许
}
//------------------------------------------------------------
/*
char* itostr(char *str, int i) //将i转化位字符串存入str
{
    sprintf(str, "%d", i);
    return str;
}

void COM_send(unsigned int i)
{
  unsigned char point = 0;
  char tmp[10];
  char str[30]="当前温度:";
  itostr(tmp, i);
  strcat(str, tmp);
  strcat(str, "\r\n");
  
  for(point=0;point< 30;point++)       //连续发送二十位数据                                      //把缓存区的数据都发送到串口
   {
      SBUF=str[point];
      while(!TI);
      TI=0;
    }
}
*/
//--------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// 函数名称： com_interrup()串口接收中断处理函数
// 函数功能： 接收包括起始位'S'在内的十位数据到数据缓冲区
//--------------------------------------------------------------------------------------------------
void com_interrupt(void) interrupt 4 using 3
{
   if(TI) //处理发送中断
    {
  		UartIsSending=0;
  		TI=0;
    }
   if(RI)                                 //处理接收中断
   {
      RI=0;                                 //清除中断标志位
      RECEIVR_buffer[i]=SBUF;
      //printf("%c", RECEIVR_buffer[i]);
      i++;
      if(RECEIVR_buffer[i-1] == '\n' || i == 29)
      {
        SCON =0x40;     //接收不允许
        RECEIVR_buffer[i] = '\0';
        //printf("%s",RECEIVR_buffer);
        read_flag = 1;
        i=0;
        SCON =0x50;     //接收允许
      }
    }
}

char putchar (char ucWriteData) //printf可用处理
{
	SBUF=ucWriteData;
	UartIsSending=1;
	while(UartIsSending);
	return (ucWriteData);
}

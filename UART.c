#include "UART.h"

//-------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// �������ƣ� UART_init()���ڳ�ʼ������
// �������ܣ� ��ϵͳʱ��Ϊ11.059MHZʱ���趨���ڲ�����Ϊ9600bit/s
//             ���ڽ����ж����������жϽ�ֹ
//--------------------------------------------------------------------------------------------------

volatile unsigned char UartIsSending;
volatile unsigned char RECEIVR_buffer[30];
volatile unsigned int i = 0;
bit read_flag = 0;

void UART_init()
{
                        //��ʼ�����пںͲ����ʷ�����
  SCON =0x50;           //ѡ�񴮿ڹ�����ʽ1���򿪽�������
  TMOD =0x20;           //��ʱ��1�����ڷ�ʽ2����ʱ��0�����ڷ�ʽ1
  TH1 =0xfA;            //ʵ�ֲ�����9600��ϵͳʱ��11.0592MHZ��
  PCON = 0x80;
  TR1 =1;               //������ʱ��T1
  ET1 =0;
  ES=1;                 //�����п��ж�
  PS=1;                 //��ƴ��п��ж����ȼ�
  EA =1;                //��Ƭ���ж�����
}
//------------------------------------------------------------
/*
char* itostr(char *str, int i) //��iת��λ�ַ�������str
{
    sprintf(str, "%d", i);
    return str;
}

void COM_send(unsigned int i)
{
  unsigned char point = 0;
  char tmp[10];
  char str[30]="��ǰ�¶�:";
  itostr(tmp, i);
  strcat(str, tmp);
  strcat(str, "\r\n");
  
  for(point=0;point< 30;point++)       //�������Ͷ�ʮλ����                                      //�ѻ����������ݶ����͵�����
   {
      SBUF=str[point];
      while(!TI);
      TI=0;
    }
}
*/
//--------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// �������ƣ� com_interrup()���ڽ����жϴ�����
// �������ܣ� ���հ�����ʼλ'S'���ڵ�ʮλ���ݵ����ݻ�����
//--------------------------------------------------------------------------------------------------
void com_interrupt(void) interrupt 4 using 3
{
   if(TI) //�������ж�
    {
  		UartIsSending=0;
  		TI=0;
    }
   if(RI)                                 //��������ж�
   {
      RI=0;                                 //����жϱ�־λ
      RECEIVR_buffer[i]=SBUF;
      //printf("%c", RECEIVR_buffer[i]);
      i++;
      if(RECEIVR_buffer[i-1] == '\n' || i == 29)
      {
        SCON =0x40;     //���ղ�����
        RECEIVR_buffer[i] = '\0';
        //printf("%s",RECEIVR_buffer);
        read_flag = 1;
        i=0;
        SCON =0x50;     //��������
      }
    }
}

char putchar (char ucWriteData) //printf���ô���
{
	SBUF=ucWriteData;
	UartIsSending=1;
	while(UartIsSending);
	return (ucWriteData);
}

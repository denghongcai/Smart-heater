#include <reg52.h>
#include "DS18B20.h"
#include "LED.h"
#include "KEY.h"
#include "UART.h"
//-----------------------------------------------------------------
// author:dhc_hust

sfr WDT_CONTR=0xe1; //看门狗定义

sbit Relay = P2^4;
sbit LED = P1^0;
code uchar led_data[10]={0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x09};
//-------------------------------------------------------------------

extern volatile unsigned int heatimesend;

volatile unsigned int mode = 0;

volatile unsigned int heatimeValue = 0;

volatile unsigned int countdownMin = 0;

void TplRefresh(void)
{
  unsigned int i = ReadTemperature();
  //对温度显示的处理
  unsigned int itmp = (int)i/10;
  unsigned int itmph = itmp/10;
  unsigned int itmpl = itmp%10;
  showTpl(itmpl, itmph);
  printf("currentpl%d\r\n",i);        //首先发送一次数据作为测试用
  i = ReadTemperature();
  itmp = (int)i/10;
  itmph = itmp/10;
  itmpl  = itmp%10;
}

unsigned int CmdDeal(unsigned char * cmd)
{
  unsigned char tplchar[3];
  unsigned int tplint;
  if(strstr(cmd, "heatime"))
  {
    mode = 1;
    tplchar[0] = cmd[7];
    tplchar[1] = cmd[8];
    tplchar[2] = '\0';
    sscanf(tplchar, "%d", &tplint);
    heatimeValue = tplint;
    if(heatimeValue == 0)
    {
      Relay = 0;
      printf("heatimeset\r\n");
      return 1;
    }
    RCAP2H =0x68;
    RCAP2L =0x00;
    TR2=1;                       //启动定时器
    ET2=1;                       //打开定时器2中断
    t = 0;
    Relay = 1;
    printf("heatimeset\r\n");
    return 1;
  }
  printf("error\r\n");
  return 0;
}

void HeatimeAlter(void)
{
  unsigned int itmph = heatimeValue/10;
  unsigned int itmpl = heatimeValue%10;
  mode = 0;
  showHeatime(itmpl, itmph);
}

void main(void)
{
  Key_init();
  UART_init();        //初始化串口
  LED = 0;
  Relay = 0; 
  WDT_CONTR = 0x37; //启动8秒看门狗
  while(1)
  {
    if(countdownMin >= heatimeValue)
    {
      Relay = 0;
      TR2 = 0;
      ET2 = 0;
      heatimeValue = 0;
      countdownMin = 0;
    }
    //加热时间同步
    if(!heatimesend)
    {
      printf("heatime%d\r\n", heatimeValue);
      heatimesend = 1;
    }
    //命令解析
    if(read_flag==1)
    {
      CmdDeal(RECEIVR_buffer);
      read_flag = 0;
    }
    switch(mode)
    {
      case 0:
        TplRefresh();
        break;
      case 1:
        HeatimeAlter();
        break;
      default:
        TplRefresh();
        break;
    }
    WDT_CONTR = 0x37; //喂狗
  }
}

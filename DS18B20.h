#include <reg52.h>		// ���ñ�׼���ͷ�ļ�
#include <absacc.h>
#include <stdio.h>
#include <math.h>

#define uchar unsigned char 
#define uint unsigned int 

sbit DQ = P2^2;			// ����ͨ����DQ

unsigned int ReadTemperature(void);
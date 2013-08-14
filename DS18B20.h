#include <reg52.h>		// 引用标准库的头文件
#include <absacc.h>
#include <stdio.h>
#include <math.h>

#define uchar unsigned char 
#define uint unsigned int 

sbit DQ = P2^2;			// 数据通信线DQ

unsigned int ReadTemperature(void);
#ifndef SHU_MATH__H
#define SHU_MATH__H

#include "stdio.h"
#include "stdint.h"
//#include "arm_math.h"
#include "math.h"

//位操作宏定义
#define SetLedStateONE(m,n) ((m)|(1<<((n)-1)))					//将某一位置1
#define SetLedStateZERO(m,n)	((m)&(~(1<<((n)-1))))			//将某一位置0
#define GetLedState(m,n) (((m)&(1<<((n)-1)))>>((n)-1))	//得到某一位的状态

//圆周率定义
#define shu_PI 3.14159265358979f
//弧度转化定义
#define shu_COEF 57.3f
//区间限制函数
int SectionLimit_i(int max , int min , int data);
float SectionLimit_f(float max , float min , float data);
//符号返还函数
int ReturnSymbol(float data);
//返还tan角所位于的象限
float ReTanSection(float angle , float x , float y);
//绝对值函数float类型
float abs_float(float data);
//绝对值函数
int my_abs(int data);
//自定义延时函数
void my_delay(uint16_t count);
//sin波形产生函数
double ProduceSin(uint64_t time , uint16_t period);
//cos波形产生函数
double ProduceCos(uint64_t time , uint16_t period);

void setbit(uint8_t* byte , int position , int value);
int16_t RUI_F_MATH_ABS_int16_t(int16_t DATA);
#endif

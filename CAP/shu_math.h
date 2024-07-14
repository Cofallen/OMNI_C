#ifndef SHU_MATH__H
#define SHU_MATH__H

#include "stdio.h"
#include "stdint.h"
//#include "arm_math.h"
#include "math.h"

//λ�����궨��
#define SetLedStateONE(m,n) ((m)|(1<<((n)-1)))					//��ĳһλ��1
#define SetLedStateZERO(m,n)	((m)&(~(1<<((n)-1))))			//��ĳһλ��0
#define GetLedState(m,n) (((m)&(1<<((n)-1)))>>((n)-1))	//�õ�ĳһλ��״̬

//Բ���ʶ���
#define shu_PI 3.14159265358979f
//����ת������
#define shu_COEF 57.3f
//�������ƺ���
int SectionLimit_i(int max , int min , int data);
float SectionLimit_f(float max , float min , float data);
//���ŷ�������
int ReturnSymbol(float data);
//����tan����λ�ڵ�����
float ReTanSection(float angle , float x , float y);
//����ֵ����float����
float abs_float(float data);
//����ֵ����
int my_abs(int data);
//�Զ�����ʱ����
void my_delay(uint16_t count);
//sin���β�������
double ProduceSin(uint64_t time , uint16_t period);
//cos���β�������
double ProduceCos(uint64_t time , uint16_t period);

void setbit(uint8_t* byte , int position , int value);
int16_t RUI_F_MATH_ABS_int16_t(int16_t DATA);
#endif

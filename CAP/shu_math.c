#include "shu_math.h"


float SectionLimit_f(float max , float min , float data)
{
	float temp = 0.0f;
	if (max >= min)
	{
		if (data >= max)
		{
			return max;
		}
		else if (data <= min)
		{
			return min;
		}
		else
		{
			return data;
		}
	}
	else
	{
		temp = min;
		min = max;
		max = temp;

		if (data >= max)
		{
			return max;
		}
		else if (data <= min)
		{
			return min;
		}
		else
		{
			return data;
		}
	}
}

int SectionLimit_i(int max , int min , int data)
{
	int temp = 0.0f;
	if (max >= min)
	{
		if (data >= max)
		{
			return max;
		}
		else if (data <= min)
		{
			return min;
		}
		else
		{
			return data;
		}
	}
	else
	{
		temp = min;
		min = max;
		max = temp;

		if (data >= max)
		{
			return max;
		}
		else if (data <= min)
		{
			return min;
		}
		else
		{
			return data;
		}
	}
}

int ReturnSymbol(float data)
{
	if (data>0)
		return 1;
	else if (data<0)
		return -1;
	else
		return 0;
}

float ReTanSection(float angle , float x , float y)
{

	return 0.0f;
}

float abs_float(float data)
{
	data = data * ReturnSymbol(data);

	return data;
}

void my_delay(uint16_t count)
{
	uint16_t i = 0 , k= 0;
	for (i = 0;i<=count;i++)
	{
		for (k = 0;k<=count;k++)
		{
		}
	}
}

int my_abs(int data)
{
	data = data * ReturnSymbol(data);

	return data;
}
///************************************************************���ָܷ���**************************************************************
//*	@author:		//����
//*	@performance:	//sin���β�������
//*	@parameter:		//ʱ����//��������(T/s)
//*	@time:			//22-02-27
//*	@ReadMe:		//
//************************************************************************************************************************************/
//double ProduceSin(uint64_t time , uint16_t period)
//{
//	double proportion = 0.0f;

//	proportion = ((float) (time%period)/(float) period) * 6.2831852f;

//	return sin(proportion);
//}
///************************************************************���ָܷ���**************************************************************
//*	@author:		//����
//*	@performance:	//cos���β�������
//*	@parameter:		//ʱ����//��������(T/s)
//*	@time:			//22-02-27
//*	@ReadMe:		//
//************************************************************************************************************************************/
//double ProduceCos(uint64_t time , uint16_t period)
//{
//	double proportion = 0.0f;

//	proportion = ((float) (time%period)/(float) period) * 6.2831852f;

//	return cos(proportion);
//}



/************************************************************���ָܷ���**************************************************************
 * 	@author:			//С��
 *	@performance:	    //setbit
 *	@parameter:		    //
 *	@time:				//23-04-20 01:48
 *	@ReadMe:			//
 ************************************************************���ָܷ���**************************************************************/

void setbit(uint8_t* byte , int position , int value)
{
	unsigned char mask = 1 << position;  // ����һ��ֻ��ָ��λ��Ϊ1������
	if (value)
	{
		*byte |= mask;  // ��ָ��λ������Ϊ1
	}
	else
	{
		*byte &= ~mask;  // ��ָ��λ������Ϊ0
	}
}







/************************************************************���ָܷ���**************************************************************
 * 	@author:			//С��
 *	@performance:	    //int16_t����ֵ
 *	@parameter:		    //
 *	@time:				//23-04-20 03:38
 *	@ReadMe:			//
 ************************************************************���ָܷ���**************************************************************/
int16_t RUI_F_MATH_ABS_int16_t(int16_t DATA)
{

	return DATA>>15 == 0 ? DATA : (~DATA + 1);
}







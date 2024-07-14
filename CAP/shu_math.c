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
///************************************************************万能分隔符**************************************************************
//*	@author:		//赵澍
//*	@performance:	//sin波形产生函数
//*	@parameter:		//时间轴//函数周期(T/s)
//*	@time:			//22-02-27
//*	@ReadMe:		//
//************************************************************************************************************************************/
//double ProduceSin(uint64_t time , uint16_t period)
//{
//	double proportion = 0.0f;

//	proportion = ((float) (time%period)/(float) period) * 6.2831852f;

//	return sin(proportion);
//}
///************************************************************万能分隔符**************************************************************
//*	@author:		//赵澍
//*	@performance:	//cos波形产生函数
//*	@parameter:		//时间轴//函数周期(T/s)
//*	@time:			//22-02-27
//*	@ReadMe:		//
//************************************************************************************************************************************/
//double ProduceCos(uint64_t time , uint16_t period)
//{
//	double proportion = 0.0f;

//	proportion = ((float) (time%period)/(float) period) * 6.2831852f;

//	return cos(proportion);
//}



/************************************************************万能分隔符**************************************************************
 * 	@author:			//小瑞
 *	@performance:	    //setbit
 *	@parameter:		    //
 *	@time:				//23-04-20 01:48
 *	@ReadMe:			//
 ************************************************************万能分隔符**************************************************************/

void setbit(uint8_t* byte , int position , int value)
{
	unsigned char mask = 1 << position;  // 生成一个只有指定位置为1的掩码
	if (value)
	{
		*byte |= mask;  // 将指定位置设置为1
	}
	else
	{
		*byte &= ~mask;  // 将指定位置设置为0
	}
}







/************************************************************万能分隔符**************************************************************
 * 	@author:			//小瑞
 *	@performance:	    //int16_t绝对值
 *	@parameter:		    //
 *	@time:				//23-04-20 03:38
 *	@ReadMe:			//
 ************************************************************万能分隔符**************************************************************/
int16_t RUI_F_MATH_ABS_int16_t(int16_t DATA)
{

	return DATA>>15 == 0 ? DATA : (~DATA + 1);
}







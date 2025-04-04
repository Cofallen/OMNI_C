#include "YU_MATH.h"
#include <math.h>
#include "DEFINE.h"
#include "main.h"
float MATH_F_Sqrt(float DATA)
{
    if (DATA < 0)
    {
        return 0; // 或者返回其他合适的值
    }
    return sqrt(DATA);
}

// 函数：限制数值在0到8192之间
float LimitRange(float value, float MAX_VALUE, float MIN_VALUE) 
{
    // 0-360 
    value += 180;
    // 首先将值转换到0到MAX_VALUE的范围内
    value = fmodf(value, MAX_VALUE);

    // 如果值小于最小值（0），则加上周期
    if (value < MIN_VALUE) {
        value += MAX_VALUE;
    }

    return value;
}

// -180-180 -> 0-8192
void convertAngleToIndex(float angle, float *index) {
    // 角度标准化到0-360度
    while (angle < 0) {
        angle += 360;
    }
    while (angle >= 360) {
        angle -= 360;
    }

    // 将角度转换为0-8191的索引
    *index = (angle / 360.0f * 8192.0f);
}

// 0-8192->-180-180
float convertInsexToAngle(float value) 
{
    // Convert the value from [-180, 180] to [0, 8192]
    float result = (float)((value + 180.0f) * (8192.0f / 360.0f));
    return result;
}

// 走小圈
float spinLittleRound(double *input)
{
    if (*input > 4096.0f)  
    {
        *input -= 8192.0f;
    }
    else if (*input < -4096.0f)
    {
        *input += 8192.0f;
    }
    return *input;
}

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

// 鼠标滤波
float mouseFilter(float last, float now, float thresholdValue)
{
    //减小平滑滤波值会增大对于细小毛刺的过滤程度
    //增加尖峰滤波值会增大对于尖峰数值的响应程度
    const float SENSITIVL_FILTER = 0.8f;    //尖峰滤波值//小于1
    const float NUMB_FILTER = 0.2f;    //平滑滤波值//小于1

    if (((MATH_D_ABS(last)) - (MATH_D_ABS(now))) >= thresholdValue)
    {
        return (float) ( now * SENSITIVL_FILTER + last * ( 1 - SENSITIVL_FILTER ));
    } else
    {
        return (float) ( now * NUMB_FILTER + last * ( 1 - NUMB_FILTER ));
    }
}

int ReturnSymbol(int16_t data)
{
	if (data>0)
		return 1;
	else if (data<0)
		return -1;
	else
		return 0;
}

int abs_int(int16_t data)
{
	data = data * ReturnSymbol(data);

	return data;
}

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

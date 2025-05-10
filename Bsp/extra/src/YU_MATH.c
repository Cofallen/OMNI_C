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

// -180-180 走小圈
// float spinLittleRound_l(double *input)
// {
// 	if (input[NOW] - input[LAST] > 180)
// 	{
// 		input[NOW] -= 360;
// 	}
// 	else if (input[NOW] - input[LAST] < -180)
// 	{
// 		input[NOW] -= 360;
// 	}
// 	return input[NOW];
// }
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


/**
 * @brief           一阶低通滤波器
 * @details         使用一阶IIR滤波算法实现低通滤波，可有效去除信号中的高频噪声
 * @param alpha   : 滤波系数 (0.0-1.0)
 *                  - 值越小，滤波效果越强，平滑度越高，但响应越慢
 *                  - 值越大，响应越快，但平滑效果越差
 *                  - 推荐值范围: 0.01-0.3
 * @param input   : 当前输入值
 * @return          滤波后的输出值
 */
float YU_MATH_LowPassFilter(float alpha, float *input)
{
    // 首次调用时直接使用输入值初始化
    if (input[LAST] == 0.0f && input[NOW] != 0.0f) {
        input[LAST] = input[NOW];
        return input[NOW];
    }
    
    // 计算新的滤波输出: y[n] = alpha * x[n] + (1-alpha) * y[n-1]
    float output = alpha * input[NOW] + (1.0f - alpha) * input[LAST];
    
    // 更新上一次的输出
    input[LAST] = output;
    
    return output;
}

/**
 * @brief           具有多通道支持的一阶低通滤波器
 * @details         多通道版本，可同时处理多路不同信号
 * @param alpha   : 滤波系数 (0.0-1.0)
 * @param input   : 输入数组，input[NOW]为当前值，input[LAST]存储滤波后结果
 * @param channel : 通道号 (0-7)，用于区分不同信号
 * @return          滤波后的输出值
 */
float YU_MATH_LowPassFilter_MC(float alpha, float *input, uint8_t channel)
{
    // 定义多个通道的上一次输出，仅用于初始化
    static uint8_t initialized[8] = {0};
    
    // 确保通道号在有效范围内
    if (channel >= 8) {
        channel = 0;
    }
    
    // 首次调用特定通道时初始化
    if (!initialized[channel]) {
        input[LAST] = input[NOW];
        initialized[channel] = 1;
        return input[NOW];
    }
    
    // 计算新的滤波输出: y[n] = alpha * x[n] + (1-alpha) * y[n-1]
    float output = alpha * input[NOW] + (1.0f - alpha) * input[LAST];
    
    // 更新上一次的输出
    input[LAST] = output;
    
    return output;
}


/**
 * @brief   N 点滑动均值滤波（重新实现）
 * @param   input   : 新的输入采样值
 * @param   buffer  : 滤波缓冲区，长度必须为 size
 * @param   idx     : 缓冲区当前位置索引（调用前请置 0）
 * @param   size    : 缓冲区大小 N
 * @param   sum     : 累加和变量（调用前请置 0）
 * @return  滤波后的平均值
 */
float YU_MATH_MeanFilter(float input,
    float *buffer, 
    uint32_t *idx,
    uint32_t size,
    float *sum)
{
   buffer[*idx] = input; // 将新的输入值存入缓冲区
   *sum += input; // 更新累加和
   if (*idx < size - 1) {
       (*idx)++; // 移动到下一个索引
   } else {
       *idx = 0; // 循环回到缓冲区开头
   }
   return *sum / size; // 返回当前平均值

}
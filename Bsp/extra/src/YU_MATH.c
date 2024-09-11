#include "YU_MATH.h"
#include <math.h>

float MATH_F_Sqrt(float DATA)
{
    if (DATA < 0)
    {
        return 0; // 或者返回其他合适的值
    }
    return sqrt(DATA);
}

// 函数：限制数值在0到8192之间
float LimitRange(float *value, float MAX_VALUE, float MIN_VALUE) 
{
    // 首先将值转换到0到MAX_VALUE的范围内
    *value = fmodf(*value, MAX_VALUE);

    // 如果值小于最小值（0），则加上周期
    if (*value < MIN_VALUE) {
        *value += MAX_VALUE;
    }

    return *value;
}
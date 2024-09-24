#include "YU_MATH.h"
#include <math.h>
#include "DEFINE.h"

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
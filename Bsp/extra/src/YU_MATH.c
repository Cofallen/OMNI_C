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
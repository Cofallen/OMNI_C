#include "main.h"

#include "ATTACK.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "DBUS.h"
#include "YU_PID.h"
#include "TIM_DEV.h"
#include "ROOT.h"
#include "YU_MATH.h"
#include "Read_Data.h"

#include "VOFA.h"
#include "stdlib.h"

#define ATTACK_D_TIMEOUT 100
#define ATTACK_D_SPEED 80

TYPEDEF_ATTACK_PARAM ATTACK_V_PARAM = {0};

int8_t LOCK = 0;

int IOTA = 0; // test
double M;     // test

// @TODO 整合到整个ROOT_init函数中, 记得Init 时间 && 摩擦轮目标值应根据裁判系统拟合
uint8_t ATTACK_F_Init(TYPEDEF_MOTOR *MOTOR)
{
    ATTACK_V_PARAM.TIME = 0;

    // 数据初始化
    ATTACK_V_PARAM.SINGLE_ANGLE = 36864.0f;
    ATTACK_V_PARAM.SPEED = 3000.0f;

    // 电机初始化
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM = -ATTACK_V_PARAM.SPEED;
    MOTOR[MOTOR_D_ATTACK_R].DATA.AIM =  ATTACK_V_PARAM.SPEED;
    MOTOR[MOTOR_D_ATTACK_G].DATA.AIM =  (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE;

    return ROOT_READY;
}

// 根据遥控，获取拨弹目标值
float ATTACK_F_JAM_Aim(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    if (LOCK == 0)
    {
        ATTACK_V_PARAM.COUNT = 1; // consist && single mode
        if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SINGLE)
        {
            LOCK = 1; // 单发上锁
        }
        else if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SHUT)
        {
            LOCK = 0; // 解锁
            ATTACK_V_PARAM.COUNT = 0;
        }
    }
    else if (LOCK == 1)
    {
        return MOTOR->DATA.AIM;
    }

    MOTOR->DATA.AIM = (float)MOTOR->DATA.ANGLE_INFINITE + ATTACK_V_PARAM.SINGLE_ANGLE * ATTACK_V_PARAM.COUNT;
    return MOTOR->DATA.AIM;
}

// 判断卡弹 @TODO 将超时判断改为ETC时钟 &&
uint8_t ATTACK_F_JAM_Check(TYPEDEF_MOTOR *MOTOR)
{
    float DIFF = MOTOR->DATA.AIM - (float)MOTOR->DATA.ANGLE_INFINITE;
    float EDGE = MATH_D_ABS((ATTACK_V_PARAM.SINGLE_ANGLE / 100.0f));

    if (((MATH_D_ABS(DIFF) >= EDGE) && ((MATH_D_ABS(MOTOR->DATA.SPEED_NOW)) <= ATTACK_D_TIMEOUT)))
    {
        if (ATTACK_V_PARAM.TIME >= ATTACK_D_TIMEOUT)
        {
            return ROOT_ERROR;
        }
        else
        {
            ATTACK_V_PARAM.TIME++;
            return ROOT_READY;
        }
    }
    else
    {
        ATTACK_V_PARAM.TIME = 0;
        return ROOT_READY;
    }
}


// 获取摩擦轮目标值  @TODO 应该是一个函数，等会写一个如何获取拟合函数
// @TODO 电机当前转速也值得加入，但这之后再说
float ATTACK_F_FIRE_Aim(TYPEDEF_MOTOR *MOTOR)
{
    // @veision 1 拟合
    float a = 0.0f, b = 0.0f;
    ATTACK_V_PARAM.SPEED = user_data.shoot_data.initial_speed * a + b;
    return ATTACK_V_PARAM.SPEED;

    // @version 2, Use this code, this code is get the fire speed by judgement system
    // but the weakness is that the speed is not stable
    
    // float TEMP = 0.0f;
    // if(user_data.shoot_data.initial_speed <= 27.5f)
    // {
    //     TEMP += 50;
    // }
    // else if (user_data.shoot_data.initial_speed > 27.5f && user_data.shoot_data.initial_speed <= 28.5f)
    // {
    //     TEMP += 5;
    // }
    // else if (user_data.shoot_data.initial_speed > 28.5f && user_data.shoot_data.initial_speed <= 29.5f)
    // {
    //     TEMP += 0;
    // }
    // else if (user_data.shoot_data.initial_speed > 29.5f && user_data.shoot_data.initial_speed <= 30.0f)
    // {
    //     TEMP -= 5;
    // }
    // else if (user_data.shoot_data.initial_speed > 30.0f)
    // {
    //     TEMP -= 50;
    // }
    // MOTOR->DATA.AIM = 8000.0f + TEMP;
    // return MOTOR->DATA.AIM;

    // @veision 3, final code, this code is a stable speed
    // MOTOR->DATA.AIM = 8000.0f;
    // return MOTOR->DATA.AIM;
}


// 总控制函数
uint8_t ATTACK_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    if (!ATTACK_F_JAM_Check(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_G]))
    {
        // 卡弹
        MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE - ATTACK_V_PARAM.SINGLE_ANGLE;
        ATTACK_V_PARAM.TIME = 0; // 重置时间
    }
    else
    {
        MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.AIM = ATTACK_F_JAM_Aim(&MOTOR[MOTOR_D_ATTACK_G], DBUS);
    }

    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM =  ATTACK_F_FIRE_Aim(&MOTOR[MOTOR_D_ATTACK_L]);
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM = -ATTACK_F_FIRE_Aim(&MOTOR[MOTOR_D_ATTACK_R]);

    // pid
    // PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_L]);
    // PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_R]);
    PID_F_S(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_L]);
    PID_F_S(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_R]);
    PID_F_AS(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_G]);

    return ROOT_READY;
}


/// @brief Prevent segfaults, to be tested
/// @param  
/// @return return (ATTACK_T_Point())
double *ATTACK_T_Point(void)
{
    double* m = (double*)malloc(2 * sizeof(double));
    if (m == NULL) 
    {
        return NULL;
    }
    return m;
}

// int n;
// double x[50], y[50];
// /// @brief test recursive function
// /// @param a 
// /// @param b 
// /// @param k 
// /// @return 
// double recursive(double a, double b, int k)
// {
//     if (k >= n)
//         return a * x[n-1] + b;
//     double p = recursive(a, b, k + 1);
//     double e = y[k] - p;
//     double mu = 0.01;  // learning rate
//     double a_new = a + mu * e * x[k] / (1 + mu * x[k] * x[k]);
//     double b_new = b + mu * e / (1 + mu * x[k] * x[k]);
//     return a_new * x[k] + b_new;
// }


/// @brief fitting using VOFA
/// @param  size buffer length (@TODO + multi param length)
/// @return 
/// @date 2024-06-21
double *ATTACK_T_FIT(int size)
{
    // Param fitting using VOFA
    // in fact, the speed of motor is too fast, so we use rate to lower it.
    // VOFA_T_Send(2,
    //             user_data.shoot_data.initial_speed,
    //             MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW * 0.001f);

    // Simply cauculate the param a,b
    double *bufferA = (double *)malloc(size * sizeof(double));
    double *bufferB = (double *)malloc(size * sizeof(double));
    static int32_t count = 0;  // help IOTA replace

    if (bufferA == NULL || bufferB == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        bufferA[i] = 0.0f;
        bufferB[i] = 0.0f;
    }
    
    bufferA[IOTA]   = user_data.shoot_data.initial_speed;
    bufferB[IOTA++] = MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW;
    count++;

    // choose special line by Least Squares Method
    double *array = (double *)calloc(2, sizeof(double));
    if (count > (size - 1))
    {
        double sumX = 0.0f, sumY = 0.0f, sumXY = 0.0f, sumXX = 0.0f;
        for (int i = 0; i < size; i++)
        {
            sumX  += bufferA[i];
            sumY  += bufferB[i];
            sumXY += bufferA[i] * bufferB[i];
            sumXX += bufferA[i] * bufferA[i];
        }
        if (size * sumXX - sumX * sumX != 0 && size > 0)
        { 
            array[0] = (size * sumXY - sumX * sumY) / (size * sumXX - sumX * sumX);
            array[1] = (sumY - array[0] * sumX) / size;
        }
        else
        {
            // @TODO return error pointer  -- type cauculate error
        }
        
        if(count % 50 == 0 || IOTA == size)
            IOTA = 0;  // replace the head of array
        double error = array[0] * user_data.shoot_data.initial_speed + array[1] - MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW;

        VOFA_T_Send(3, array[0], array[1], error);  // the place is not sure
    }

    free (bufferA);
    free (bufferB);
    free (array);

    return &M;
}
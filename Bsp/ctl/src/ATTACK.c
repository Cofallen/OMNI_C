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
#include "VISION.h"

#include "VOFA.h"
#include "stdlib.h"
#include "bsp_dwt.h"
#include <math.h>

// delete later
#include "CAN_DEV.h"
#include "can.h"

#define ATTACK_D_TIMEOUT 500
#define ATTACK_D_SPEED 100

TYPEDEF_ATTACK_PARAM ATTACK_V_PARAM = {0};

double param[5] = {0};

int IOTA = 0; // test
double M;     // test

float tt, countttt = 0.0f; // test    
float aim_get_edge = 0.0f;
// @TODO 整合到整个ROOT_init函数中, 记得Init 时间 && 摩擦轮目标值应根据裁判系统拟合
uint8_t ATTACK_F_Init(TYPEDEF_MOTOR *MOTOR)
{
    ATTACK_V_PARAM.TIME = 0;

    // 数据初始化
    ATTACK_V_PARAM.SINGLE_ANGLE = 36864.0f;
    ATTACK_V_PARAM.SPEED = 7200.0f;

    ATTACK_V_PARAM.FLAG = 1;
    ATTACK_V_PARAM.LOCK = 1; // 默认上锁，保证在未收到遥控数据时拨盘不动

    // 电机初始化
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM =  0.0f;
    MOTOR[MOTOR_D_ATTACK_R].DATA.AIM =  0.0f;
    MOTOR[MOTOR_D_ATTACK_G].DATA.AIM =  (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE;

    return ROOT_READY;
}

/**
 * @brief 根据遥控/鼠标输入计算拨弹电机目标值
 * @param MOTOR 拨弹电机指针
 * @param DBUS 遥控器数据指
 * @param autofire 视觉控制开火
 * @return 目标角度值
 */
float ATTACK_F_JAM_Aim(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS, uint8_t autofire)
{
    // 记录上一次的遥控器和鼠标状态
    static uint8_t prev_S1_state = DBUS_D_MOD_SHUT;
    
    // 单发模式处理 (通过状态变化检测)
    if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SINGLE)
    {
        // 检测遥控器模式切换到单发
        if (prev_S1_state != DBUS_D_MOD_SINGLE)
        {
            if (autofire == 0 || ((autofire == 1) && VISION_V_DATA.RECEIVE.fire))
            {
                ATTACK_V_PARAM.COUNT = 1;  // 增加一个弹丸的角度
            }
        }
    }
    // 鼠标单发处理
    else if (DBUS->MOUSE.L_STATE == 1 && ATTACK_V_PARAM.PREV_MOUSE_STATE != 1)
    {
        if (autofire == 0 || ((autofire == 1) && VISION_V_DATA.RECEIVE.fire))
        {
            ATTACK_V_PARAM.COUNT = 1;  // 检测到鼠标左键按下事件，发射一个弹丸
        }
    }
    // 连发模式处理
    else if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_CONSIST || DBUS->MOUSE.L_STATE == 2)
    {
        if (autofire == 0 || ((autofire == 1) && VISION_V_DATA.RECEIVE.fire))
        {
            // 视觉允许开火且为连发模式
            ATTACK_V_PARAM.COUNT = 1;  // 持续小量增加目标角度，形成连续转动
        }
    }
    // 关闭发射处理
    else if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SHUT)
    {
        ATTACK_V_PARAM.COUNT = 0;  // 关闭状态不增加角度
    }
    
    // 计算新的电机目标角度
    if (ATTACK_V_PARAM.COUNT > 0) // @debug  && ATTACK_V_PARAM.fire_wheel_status
    {
        MOTOR->DATA.AIM = (float)MOTOR->DATA.ANGLE_INFINITE - ATTACK_V_PARAM.SINGLE_ANGLE * ATTACK_V_PARAM.COUNT;
        // 单发模式下，处理完一次后重置COUNT
        if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SINGLE || DBUS->MOUSE.L_STATE == 1)
        {
            ATTACK_V_PARAM.COUNT = 0;
        }
    }
    
    // 保存当前状态用于下一次比较
    ATTACK_V_PARAM.PREV_MOUSE_STATE = DBUS->MOUSE.L_STATE;
    prev_S1_state = DBUS->REMOTE.S1_u8;

    return MOTOR->DATA.AIM;
}

// 判断卡弹 @TODO 将超时判断改为ETC时钟 &&
uint8_t ATTACK_F_JAM_Check(TYPEDEF_MOTOR *MOTOR)
{
    float DIFF = MOTOR->DATA.AIM - (float)MOTOR->DATA.ANGLE_INFINITE;
    float EDGE = MATH_D_ABS((ATTACK_V_PARAM.SINGLE_ANGLE / 300.0f));

    if (((MATH_D_ABS(DIFF) >= EDGE) && ((MATH_D_ABS(MOTOR->DATA.SPEED_NOW)) <= ATTACK_D_SPEED)))
    {
        if (ATTACK_V_PARAM.TIME >= ATTACK_D_TIMEOUT)
        {
            ATTACK_V_PARAM.is_jam = 1;
            return ROOT_ERROR;
        }
        else
        {
            ATTACK_V_PARAM.TIME++;
            return ROOT_READY;
        }
    }
    else // edge到&&速度很小(到达) / edge到&&速度很大(连发) / edge不到&&速度很大(正在转)
    { 
        ATTACK_V_PARAM.TIME = 0;
        ATTACK_V_PARAM.is_jam = 0;
        return ROOT_READY;
    }
}


// 获取摩擦轮目标值  @TODO 应该是一个函数，等会写一个如何获取拟合函数
// @TODO 电机当前转速也值得加入，但这之后再说
float ATTACK_F_FIRE_Aim(TYPEDEF_MOTOR *MOTOR)
{
    static uint8_t fire_mouse_status = 0;
    // @veision 1 拟合
    // float a = 0.0f, b = 0.0f;
    // ATTACK_V_PARAM.SPEED = user_data.shoot_data.initial_speed * a + b;
    // return ATTACK_V_PARAM.SPEED;

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
    if ( fire_mouse_status == 1  &&  DBUS_V_DATA.REMOTE.S2_u8 == 3)  // 3 is the fire button
    {
        MOTOR->DATA.AIM = ATTACK_V_PARAM.SPEED;
        ATTACK_V_PARAM.fire_wheel_status = 1;
    } else
    {
        MOTOR->DATA.AIM = 0.0f;
        ATTACK_V_PARAM.fire_wheel_status = 0;
    }
    if (DBUS_V_DATA.KEY_BOARD.CTRL  && !DBUS_V_DATA.KEY_BOARD.CTRL_PREE_NUMBER)
    {
        fire_mouse_status = !fire_mouse_status;
    }
    DBUS_V_DATA.KEY_BOARD.CTRL_PREE_NUMBER = DBUS_V_DATA.KEY_BOARD.CTRL;
    
    return MOTOR->DATA.AIM;
}


// 总控制函数
uint8_t ATTACK_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    if (!MOTOR[MOTOR_D_ATTACK_G].is_off[NOW] && MOTOR[MOTOR_D_ATTACK_G].is_off[LAST]) {
        MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE;
    }
    MOTOR[MOTOR_D_ATTACK_G].is_off[LAST] = MOTOR[MOTOR_D_ATTACK_G].is_off[NOW];
    

    aim_get_edge = fabs((MOTOR[MOTOR_D_ATTACK_G].DATA.AIM) - (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE);
    if (aim_get_edge < 5000.0f)
        ATTACK_V_PARAM.STATUS[NOW] = 0;
    else ATTACK_V_PARAM.STATUS[NOW] = 1;
    
    if (ATTACK_V_PARAM.STATUS[NOW] == 0)
    {
        MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = ATTACK_F_JAM_Aim(&MOTOR[MOTOR_D_ATTACK_G], DBUS, DBUS->MOUSE.R_STATE);
    }
    // if (!ATTACK_F_JAM_Check(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_G]))
    // {
    //     // 卡弹
    //     MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE+ ATTACK_V_PARAM.SINGLE_ANGLE; //  @debug  * ATTACK_V_PARAM.FLAG
    //     // ATTACK_V_PARAM.FLAG = -ATTACK_V_PARAM.FLAG;
    //     ATTACK_V_PARAM.TIME = 0; // 重置时间
    // }
    // shooting case when opposite to you l-r
    
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM = -ATTACK_F_FIRE_Aim(&MOTOR[MOTOR_D_ATTACK_L]);
    MOTOR[MOTOR_D_ATTACK_R].DATA.AIM =  ATTACK_F_FIRE_Aim(&MOTOR[MOTOR_D_ATTACK_R]);
    
    ATTACK_F_JAM_Disable(&MOTOR[MOTOR_D_ATTACK_G]);
    // ATTACK_T_FIT(40);
    // pid
    PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_L]);
    PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_R]);
    // PID_F_S(&MOTOR[MOTOR_D_ATTACK_L]);
    // PID_F_S(&MOTOR[MOTOR_D_ATTACK_R]);
    PID_F_AS(&MOTOR[MOTOR_D_ATTACK_G]);

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
    // 使用静态数组代替动态分配
    static double bufferA[50];
    static double bufferB[50];
    static int32_t count = 0;  // help IOTA replace

    // 限制size不超过静态数组大小
    if (size > 50) size = 50;
    
    bufferA[IOTA]   = user_data.shoot_data.initial_speed;
    bufferB[IOTA++] = MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW;
    count++;

    // choose special line by Least Squares Method
    double array[2] = {0};
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
        if (size * sumXX - sumX * sumX != 0.0f && size > 0)
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

        VOFA_T_Send(0, 10, array[0], array[1], user_data.shoot_data.initial_speed, MOTOR_V_ATTACK[2].DATA.SPEED_NOW,
                            error, 0, 0, 0, 0, 0);  // the place is not sure

        param[0] = array[0];  // a
        param[1] = array[1];  // b
        param[2] = error;    // error
        param[3] = user_data.shoot_data.initial_speed;  // speed
        param[4] = MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW;  // speed now
    }

    return &M;
}

/**
 * @brief 卡弹处理函数 - 累计3秒后失能拨弹电机，重新启动需要手动拨到目标位置
 * @param MOTOR 拨弹电机指针
 * @return 返回电机状态 1表示已失能 0表示未失能
 * @note 部分参数名称别看，瞎写的
 */
uint8_t ATTACK_F_JAM_Disable(TYPEDEF_MOTOR *MOTOR) 
{
    // 定义卡弹持续时间阈值(3秒)
    const float JAM_DISABLE_THRESHOLD = 10.0f;  // 单位：秒
    static uint8_t jam_disable_flag = 0;       // 记录是否已经失能
    static float jam_start_time = 0.0f;        // 记录卡弹开始时间
    static uint8_t last_jam_status = {0};        // 记录上一次卡弹状态
    float current_time = DWT_GetTimeline_s();
    
    // 如果检测到卡弹
    if (ATTACK_V_PARAM.STATUS[NOW] == 1) {
        // 卡弹状态开始（从无卡弹到有卡弹的转变）
        if (last_jam_status == 0) {
            jam_start_time = current_time;  // 记录卡弹开始时间
            last_jam_status = 1;
        }
        
        // 计算卡弹持续时间
        float jam_duration = current_time - jam_start_time;
        tt = jam_duration;  // 调试用，记录卡弹持续时间
        // 如果卡弹持续超过阈值
        if (jam_duration >= JAM_DISABLE_THRESHOLD && !jam_disable_flag) {
            // 失能拨弹电机
             MOTOR->DATA.ENABLE = 0;
            jam_disable_flag = 1;  // 标记已失能
            countttt++;
            return ROOT_READY;  // 返回成功状态
        }
    } 
    else {
        // 卡弹解除
        if (last_jam_status == 1) {
            // 状态从有卡弹变为无卡弹
            last_jam_status = 0;
        }
            MOTOR->DATA.ENABLE = 1;
            jam_disable_flag = 0;
    }
    VOFA_T_Send(0, 10, (float)ATTACK_V_PARAM.STATUS[NOW],
            (float)jam_start_time,
            (float)current_time,
            (float)tt,
            (float)countttt,
            (float)MOTOR->DATA.ENABLE, 
            (float)MOTOR->DATA.AIM, 
            (float)MOTOR->DATA.ANGLE_INFINITE, 
            aim_get_edge, 0
        );
    return ROOT_ERROR;  // 返回未失能状态
}
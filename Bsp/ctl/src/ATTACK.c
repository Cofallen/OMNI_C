#include "main.h"

#include "ATTACK.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "DBUS.h"
#include "PID.h"
#include "TIM_DEV.h"
#include "ROOT.h"
#include "YU_MATH.h"

#define ATTACK_D_TIMEOUT 100
#define ATTACK_D_SPEED 80

struct
{
    int32_t TIME;
    int8_t FLAG;
    float SINGLE_ANGLE; // 单发角度
    float SPEED;        // 摩擦轮角度
    int COUNT;
} ATTACK_V_PARAM = {0};

int8_t LOCK = 0;

// @TODO 整合到整个ROOT_init函数中, 记得Init 时间 && 摩擦轮目标值应根据裁判系统拟合
uint8_t ATTACK_F_Init(TYPEDEF_MOTOR *MOTOR)
{
    ATTACK_V_PARAM.TIME = 0;

    // 数据初始化
    ATTACK_V_PARAM.SINGLE_ANGLE = 36864.0f;
    ATTACK_V_PARAM.SPEED = 3000.0f;

    // 电机初始化
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM = -ATTACK_V_PARAM.SPEED;
    MOTOR[MOTOR_D_ATTACK_R].DATA.AIM = ATTACK_V_PARAM.SPEED;
    MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE;

    return ROOT_READY;
}

// 根据遥控，获取拨弹目标值
float ATTACK_F_JAM_Aim(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    if (LOCK == 0)
    {
        ATTACK_V_PARAM.COUNT = 1; // consist && single mode
        if (DBUS->REMOTE.S2_u8 == DBUS_D_MOD_SINGLE)
        {
            LOCK = 1; // 单发上锁
        }
        else if (DBUS->REMOTE.S2_u8 == DBUS_D_MOD_SHUT)
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

    // pid
    PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_L]);
    PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_R]);
    PID_F_AS(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_G]);

    return ROOT_READY;
}
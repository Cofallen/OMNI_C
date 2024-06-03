#include "main.h"

#include "CHASSIS.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "YU_MATH.h"
#include "DBUS.h"
#include "PID.h"

// 本代码为全向轮底盘运动基本代码

void CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    // 运动学解算
    float Vx = 0.0f, Vy = 0.0f, Vr = 0.0f, RATE = 1.0f, COMPONENT[2] = {0, 0};
    double ANGLE_Relative = 0.0f, ANGLE_Rad = 0.0f;
    Vx = (float)DBUS->REMOTE.CH0_int16 * RATE;
    Vy = (float)DBUS->REMOTE.CH1_int16 * RATE;
    // Vr = -(float)DBUS->REMOTE.CH2_int16;

    // 底盘跟随模式

    // 各方向分量限幅，TODO：待计算比例因子RATE
    Vx = MATH_D_LIMIT(3000, -3000, Vx);
    Vy = MATH_D_LIMIT(3000, -3000, Vy);
    Vr = MATH_D_LIMIT(1000, -1000, Vr);

    // 运动学解算
    MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = (Vx + Vy - Vr * COMPONENT[0]) * COMPONENT[1];
    MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-Vx + Vy - Vr * COMPONENT[0]) * COMPONENT[1];
    MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-Vx - Vy - Vr * COMPONENT[0]) * COMPONENT[1];
    MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = (Vx - Vy - Vr * COMPONENT[0]) * COMPONENT[1];

    // pid 解算
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_1]);
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_2]);
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_3]);
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_4]);
}
#include "main.h"
#include "math.h"

#include "CHASSIS.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "YU_MATH.h"
#include "DBUS.h"
#include "PID.h"
#include "TIM_DEV.h"

#include "Read_Data.h"

float mm = 0, nn = 0, xx = 0, yy = 0, rr = 0, cc = 0, ss = 0;
// 本代码为全向轮底盘运动基本代码

double ANGLE_Rad = 0.0f;
void CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    // 运动学解算
    float Vx = 0.0f, Vy = 0.0f, Vr = 0.0f, RATE = 1.0f, COMPONENT[2] = {1, 3};
    double ANGLE_Relative = 0.0f; //ANGLE_Rad = 0.0f;
    double PRIDICT = 0.0f;    // 底盘预测，前馈
    Vx =  (float)DBUS->REMOTE.CH0_int16 * RATE;
    Vy = -(float)DBUS->REMOTE.CH1_int16 * RATE;
//    Vr = (float)DBUS->REMOTE.CH2_int16 * RATE;
//     Vr = MATH_D_ABS(MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.SPEED_NOW) * 1.0f;  // head rotate -> chassis rotate
    
    // 底盘跟随模式
    ANGLE_Relative = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW - (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INIT;
    
	mm = ANGLE_Relative;
	nn = ANGLE_Rad;
    ANGLE_Rad = ANGLE_Relative * MATH_D_RELATIVE_PARAM;

    if (DBUS->REMOTE.S2_u8 == 1)  // @TODO + 底盘跟随判断 A&B + GEER挡位
    {
        ANGLE_Rad += 3.1416926f * 0.5f  * 0.001f;
        Vr = 1000.0f;
    }
    else if (DBUS->REMOTE.S2_u8 == 3)
    {
        Vr = 0.0f;
    }
    else if (DBUS->REMOTE.S2_u8 == 2)
    {
        // ANGLE_Rad += 3.1416926f * 0.5f * 0.001f;
        Vr = -1000.0f;
    }

    //  double COS = cos(ANGLE_Rad);
    //  double SIN = sin(ANGLE_Rad);
    //  Vx = -Vy * SIN + Vx * COS;
    //  Vy =  Vy * COS + Vx * SIN;

    
//    // 各方向分量限幅，TODO：待计算比例因子RATE
//    Vx = MATH_D_LIMIT(3000, -3000, Vx);
//    Vy = MATH_D_LIMIT(3000, -3000, Vy);
//    Vr = MATH_D_LIMIT(1000, -1000, Vr);

    PRIDICT = DBUS->REMOTE.CH2_int16 * 16.0f;  // @TODO 预测模型待思考

    // 运动学解算
    MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = ( Vx + Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-Vx + Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-Vx - Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = ( Vx - Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;

    // pid 解算
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_1]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_2]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_3]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_4]);
}

// The following code is for reference ZJU Power Control
// Reference Site: https://zju-helloworld.github.io/Wiki/
// fitting function :
// P = \omega * M + P_{loss}
//              M = k_{M} * i_{q}, consultation, k_{M} = 0.3 Nm/A
//                  P_{loss} = R * i^2
//                             R = p_{1} = 0.1249
//                                 p_{2} = -0.007836 -> 0
//                                 p_{3} = 5.952, circuit power
// @TODO using karman filter -> k 
// using function predict

struct CHASSIS_F_PwrLimit
{
    float Pwr;
    float Z;
    float k;
    float R;
    float PwrSum;
    float PwrPred[4];
}CHASSIS_V_PwrLimit;


void CHASSIS_F_PwrLimitInit()
{

}


void CHASSIS_F_PwrLimitCalc(TYPEDEF_MOTOR *MOTOR)
{
    CHASSIS_V_PwrLimit.Pwr = user_data.power_heat_data.chassis_power;
    CHASSIS_V_PwrLimit.Z   = user_data.power_heat_data.buffer_energy;

    CHASSIS_V_PwrLimit.R = 0.3f;
    CHASSIS_V_PwrLimit.k = 0.1249f;  // @TODO k need to calculate by karman filter
    CHASSIS_V_PwrLimit.PwrSum = 0.0f;

    // dimension transform
    float current[4] = {0.0f};
    for (int i = 0; i < 4; i++)
    {
        // dimension transform
        current[i] = MOTOR[i].DATA.CURRENT * 0.001220703125f;
        CHASSIS_V_PwrLimit.PwrPred[i] = CHASSIS_V_PwrLimit.k * current[i] * MOTOR[i].DATA.SPEED_NOW \
                        + CHASSIS_V_PwrLimit.R * current[i] * current[i];
        if (CHASSIS_V_PwrLimit.PwrPred < 0)
            continue;
        CHASSIS_V_PwrLimit.PwrSum += CHASSIS_V_PwrLimit.PwrPred[i];
    }
    CHASSIS_V_PwrLimit.PwrSum += CHASSIS_V_PwrLimit.Pwr;

    // @TODO no consider the cap
    CHASSIS_V_PwrLimit.PwrSum -= CHASSIS_V_PwrLimit.Z;

    if (CHASSIS_V_PwrLimit.PwrSum > CHASSIS_V_PwrLimit.Pwr)
    {
        float scale = CHASSIS_V_PwrLimit.Pwr / CHASSIS_V_PwrLimit.PwrSum;
        for (int i = 0; i < 4; i++)
        {
            CHASSIS_V_PwrLimit.PwrPred[i] *= scale;
        }
    }

    // @TODO limit the power to motor
    
    
}

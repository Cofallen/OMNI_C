#include "main.h"
#include "math.h"

#include "CHASSIS.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "YU_MATH.h"
#include "DBUS.h"
#include "YU_PID.h"
#include "TIM_DEV.h"

#include "Read_Data.h"
#include "ROOT.h"
#include "YU_MATH.h"
#include "TOP.h"

double ANGLE_Rad = 0.0f;
double ANGLE_Relative = 0.0f;

void CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    // 运动学解算
    float VX = 0.0f, VY = 0.0f, VR = 0.0f, COMPONENT[2] = {1, 3.5};
    float ROTATE_VX = 0.0f, ROTATE_VY = 0.0f;  // 旋转矩阵
    double PRIDICT = 0.0f;    // 底盘预测，前馈

    ANGLE_Relative = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW - (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INIT;  // if add 4096
    spinLittleRound(&ANGLE_Relative);
    ANGLE_Rad = ANGLE_Relative * MATH_D_RELATIVE_PARAM;

    VX =  (float)((DBUS->REMOTE.CH0_int16) + (DBUS->KEY_BOARD.D - DBUS->KEY_BOARD.A) * 660.0f) * 10.0f;
    VY =  (float)((DBUS->REMOTE.CH1_int16) + (DBUS->KEY_BOARD.W - DBUS->KEY_BOARD.S) * 660.0f) * 10.0f;
    VR = -(float)((DBUS->REMOTE.DIR_int16) + (DBUS->KEY_BOARD.SHIFT) * 660.0f) * 3.0f;

    if (DBUS->KEY_BOARD.V)
    {
        VR *= 0.4f;
    }

    // if (DBUS->REMOTE.S2_u8 == 1)  // @TODO + 底盘跟随判断 A&B + GEER挡位 // chassis folllow
    // {   
    //     PRIDICT = DBUS->REMOTE.CH2_int16 * 2.0f;  // @TODO 预测模型待思考
    //     if (TOP.yaw[4] == 1.0f && ((MATH_D_ABS(ANGLE_Relative) > 0.0f)))  // 死区0
    //     {
    //         VR = PID_F_Cal(&FOLLOW_PID, 0, -ANGLE_Relative);
    //     }
    //     else if (DBUS->REMOTE.S2_u8 == 3 || DBUS->REMOTE.S2_u8 == 2) // @TODO little spining go straight
    //     {
    //         PRIDICT = 0.0f;
    //         VR = -(float)DBUS->REMOTE.DIR_int16 * 3.0f;
    //     }
    // }
    
    // PRIDICT = DBUS->REMOTE.CH2_int16 * 2.0f;  // @TODO 待测试，如若没成功，取消上面注释。将这两行注释 2. VR的负号和-ANGLE_Relative的负号测试是否可以全换成正号
    (!DBUS->REMOTE.DIR_int16)?(PRIDICT = DBUS->REMOTE.CH2_int16 * 2.0f,VR = PID_F_Cal(&FOLLOW_PID, 0, -ANGLE_Relative)):(PRIDICT = 0.0f);     // 分离 滚轮影响小陀螺

    // rotate matrix
    double COS = cos(ANGLE_Rad);
    double SIN = sin(ANGLE_Rad);
    
    ROTATE_VX = -VY * SIN + VX * COS;
    ROTATE_VY =  VY * COS + VX * SIN;
    
    // 运动学解算
    MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = ( ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = ( ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;

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

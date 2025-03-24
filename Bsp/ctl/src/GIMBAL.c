#include "main.h"

#include "GIMBAL.h"
#include "DEFINE.h"
#include "DBUS.h"
#include "YU_PID.h"
#include "YU_MATH.h"
#include "VISION.h"
#include "TOP.h"
#include "TIM_DEV.h"

float DBUS_V_CH2[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // last now error 3-count 4-status 
float aim = 0;


// 选一个模式写：
// 1. 重写pid，加入是否开启视觉参数，实现两套pid
// 2. 增加pid_f_vision_pitch，用四套pid
void GIMBAL_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS, TYPEDEF_VISION *VISION)
{
    static float currentAngle = 0.0f;
    switch (DBUS->REMOTE.S2_u8)
    {
    case 3:; case 1:  // 遥控
        {
            #ifdef LIFTED_DEBUG
            MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += (-(float)DBUS->REMOTE.CH2_int16 * 0.04f - MATH_D_LIMIT(1, -1, DBUS->MOUSE.X_FLT * 0.01f) + (float) (DBUS->KEY_BOARD.E - DBUS->KEY_BOARD.Q ) * 0.8f + (float)currentAngle);
            #else
            MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += (-(float)DBUS->REMOTE.CH2_int16 * 0.08f - MATH_D_LIMIT(1, -1, DBUS->MOUSE.X_FLT * 0.01f) + (float) (DBUS->KEY_BOARD.E - DBUS->KEY_BOARD.Q ) * 0.8f + (float)currentAngle);
            #endif

            currentAngle = 0.0f;
            MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM += -(float)DBUS->REMOTE.CH3_int16 * 0.01f - DBUS->MOUSE.Y_FLT * 0.01f;
            MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = MATH_D_LIMIT(GIMBAL_PIT_MAX, GIMBAL_PIT_MIN, MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM);
        
            PID_F_G(&MOTOR[MOTOR_D_GIMBAL_YAW]);
            PID_F_P_T(&MOTOR[MOTOR_D_GIMBAL_PIT]);
        }
        break;

    case 4:  // 发射
        {

        }
        break;

    case 2:
        {
            if(VISION_V_DATA.RECEIVE.TARGET){
                PID_F_VISION_YAW(&MOTOR[MOTOR_D_GIMBAL_YAW]);
				PID_F_VISION_PIT(&MOTOR[MOTOR_D_GIMBAL_PIT]);
            }
            currentAngle = TOP.yaw[5];
        }
        break;

    default:
        break;
    }
   
}



// 采用一次函数调整，效果很差
// void remoteFilter(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
// {
// // smooth finally some gaps
//     DBUS_V_CH2[NOW]  = (float)DBUS->REMOTE.CH2_int16;
//     DBUS_V_CH2[2] = MATH_D_ABS((DBUS_V_CH2[NOW] - DBUS_V_CH2[LAST]));

//     if ((DBUS_V_CH2[2] > 800) || DBUS_V_CH2[4] == 1.0f) // smooth transmation
//     {
//         if (DBUS_V_CH2[3] == 0.0f)
//         {
//             aim = DBUS_V_CH2[2];
//             DBUS_V_CH2[4] = 1.0f;
//         }
//         DBUS_V_CH2[3]++;
//         MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += aim * 0.001f;   // 1s

//         DBUS_V_CH2[LAST] = (float)DBUS_V_CH2[NOW];
        
//         if (DBUS_V_CH2[3] >= 3)
//         {
//             DBUS_V_CH2[4] = 0.0f;
//             DBUS_V_CH2[3] = 0.0f;
//             DBUS_V_CH2[2] = 0.0f;
//         }
//     }
//     else
//     {
//         DBUS_V_CH2[4] = 0.0f;
//         MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += -(float)DBUS->REMOTE.CH2_int16 * 0.02f;
//     }
// }
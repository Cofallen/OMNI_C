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


// 2. 增加pid_f_vision_pitch，用四套pid
void GIMBAL_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS, TYPEDEF_VISION *VISION)
{
    static float yawAngle = 0.0f, pitAngle = 0.0f;
    switch (DBUS->REMOTE.S2_u8)
    {
    case 3:; case 1:  // 遥控
        {
            #ifdef LIFTED_DEBUG
            MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += (-(float)DBUS->REMOTE.CH2_int16 * 0.04f - MATH_D_LIMIT(15, -15, DBUS->MOUSE.X_FLT * 0.2f) + (float) (-DBUS->KEY_BOARD.E + DBUS->KEY_BOARD.Q ) * 15.0f);
            #else
            MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += (-(float)DBUS->REMOTE.CH2_int16 * 0.03f - MATH_D_LIMIT(25, -25, DBUS->MOUSE.X_FLT * 0.5f) + (float) (-DBUS->KEY_BOARD.E + DBUS->KEY_BOARD.Q ) * 30.0f + (float)yawAngle);
            #endif

            yawAngle = 0.0f;
            MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM += (float)DBUS->REMOTE.CH3_int16 * 0.0015f + DBUS->MOUSE.Y_FLT * 0.06f;
            MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = MATH_D_LIMIT(GIMBAL_PIT_MAX, GIMBAL_PIT_MIN, MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM);
            
            if (MATH_D_ABS((MOTOR[MOTOR_D_GIMBAL_PIT].DATA.ANGLE_NOW + 18.0f)) < 0.5f)
            {
                MOTOR[MOTOR_D_GIMBAL_PIT].PID_S.IN.KI = 0.001f;
				MOTOR[MOTOR_D_GIMBAL_PIT].PID_S.IN.KP = 80.0f;
            }
            else 
            {
                MOTOR[MOTOR_D_GIMBAL_PIT].PID_S.IN.KI = 0.45f;
            }
            
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
//                VISION_V_DATA.block_Time++;
//                if (VISION_V_DATA.block_Time >= 501)
//                {
//                    VISION_V_DATA.block_Time = 0;
//                }
//                if (VISION_V_DATA.block_Time < 500)
//                {
                    PID_F_VISION_YAW(&MOTOR[MOTOR_D_GIMBAL_YAW]);
				    PID_F_VISION_PIT(&MOTOR[MOTOR_D_GIMBAL_PIT]);
//                }
            }
            else {  
                // MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += (-(float)DBUS->REMOTE.CH2_int16 * 0.002f - MATH_D_LIMIT(15, -15, DBUS->MOUSE.X_FLT * 0.2f) + (float) (-DBUS->KEY_BOARD.E + DBUS->KEY_BOARD.Q ) * 15.0f);
             
                PID_F_G(&MOTOR[MOTOR_D_GIMBAL_YAW]);
                PID_F_P_T(&MOTOR[MOTOR_D_GIMBAL_PIT]);
                MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = MATH_D_LIMIT(GIMBAL_PIT_MAX, GIMBAL_PIT_MIN, MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM);
               
            }
            yawAngle = TOP.yaw[3];
            pitAngle = (float)MOTOR[MOTOR_D_GIMBAL_PIT].DATA.ANGLE_NOW;
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
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

uint8_t auto_aim_enabled = 0;  // 0表示关闭自瞄，1表示开启自瞄

// 2. 增加pid_f_vision_pitch，用四套pid
void GIMBAL_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS, TYPEDEF_VISION *VISION)
{
    static float yawAngle = 0.0f, pitAngle = 0.0f;

    // 检测鼠标右键点击（上升沿）
    (DBUS->MOUSE.R_STATE || VISION_V_DATA.SEND.is_buff) ? (auto_aim_enabled = 1) : (auto_aim_enabled = 0);
    
    if (!DBUS->MOUSE.R_STATE && DBUS->MOUSE.R_PRESS_NUMBER)
        MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM = TOP.yaw[3];
    DBUS->MOUSE.R_PRESS_NUMBER = DBUS->MOUSE.R_STATE; 

    // 根据自瞄状态执行相应的控制逻辑
    if((auto_aim_enabled) && VISION_V_DATA.RECEIVE.TARGET ) {
        // 自瞄模式开启且有目标
        PID_F_VISION_YAW(&MOTOR[MOTOR_D_GIMBAL_YAW]);
        PID_F_VISION_PIT(&MOTOR[MOTOR_D_GIMBAL_PIT]);

        yawAngle = TOP.yaw[3];
        pitAngle = TOP.roll[5];
    } 
    else if ((auto_aim_enabled) && !VISION_V_DATA.RECEIVE.TARGET )
    {
        PID_F_VISION_YAW(&MOTOR[MOTOR_D_GIMBAL_YAW]);
        PID_F_VISION_PIT(&MOTOR[MOTOR_D_GIMBAL_PIT]);
    } 
    else {
        // 手动控制模式或自瞄无目标
        switch (DBUS->REMOTE.S2_u8)
        {
        case 3:; case 1:  // 遥控
            {
                #ifdef LIFTED_DEBUG
                
                MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += (-(float)DBUS->REMOTE.CH2_int16 * 0.009f - MATH_D_LIMIT(15, -15, DBUS->MOUSE.X_FLT * 0.08f) + (float) (-DBUS->KEY_BOARD.E + DBUS->KEY_BOARD.Q ) * 3.0f);
                #else
                MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += (-(float)DBUS->REMOTE.CH2_int16 * 0.03f - MATH_D_LIMIT(25, -25, DBUS->MOUSE.X_FLT * 0.5f) + (float) (-DBUS->KEY_BOARD.E + DBUS->KEY_BOARD.Q ) * 30.0f + (float)yawAngle);
                #endif

                yawAngle = 0.0f;
                MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM += (float)DBUS->REMOTE.CH3_int16 * 0.0003f + DBUS->MOUSE.Y_FLT * 0.004f;
                MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = MATH_D_LIMIT(GIMBAL_PIT_MAX, GIMBAL_PIT_MIN, MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM);
                
                PID_F_G(&MOTOR[MOTOR_D_GIMBAL_YAW]);
                PID_F_P_T(&MOTOR[MOTOR_D_GIMBAL_PIT]);
            }
            break;

		case 0:; case 2:  // 离线后 会进入
            {
                (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM = TOP.yaw[3]) : (MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM = MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM); 
                (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = TOP.roll[5]) : (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM);
                PID_F_G(&MOTOR[MOTOR_D_GIMBAL_YAW]);
                PID_F_P_T(&MOTOR[MOTOR_D_GIMBAL_PIT]);
            }
            break;

        default:
            break;
        }
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
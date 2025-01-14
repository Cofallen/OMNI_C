#include "main.h"

#include "GIMBAL.h"
#include "DEFINE.h"
#include "DBUS.h"
#include "YU_PID.h"
#include "YU_MATH.h"
#include "VISION.h"
#include "TOP.h"

float DBUS_V_CH2[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // last now error 3-count 4-status 
float aim = 0;

void GIMBAL_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    // smooth finally some gaps
    DBUS_V_CH2[NOW]  = (float)DBUS->REMOTE.CH2_int16;
    DBUS_V_CH2[2] = MATH_D_ABS((DBUS_V_CH2[NOW] - DBUS_V_CH2[LAST]));

    if ((DBUS_V_CH2[2] > 800) || DBUS_V_CH2[4] == 1.0f) // smooth transmation
    {
        if (DBUS_V_CH2[3] == 0.0f)
        {
            aim = DBUS_V_CH2[2];
            DBUS_V_CH2[4] = 1.0f;
        }
        DBUS_V_CH2[3]++;
        MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += aim * 0.001f;   // 1s

        DBUS_V_CH2[LAST] = (float)DBUS_V_CH2[NOW];
        
        if (DBUS_V_CH2[3] >= 3)
        {
            DBUS_V_CH2[4] = 0.0f;
            DBUS_V_CH2[3] = 0.0f;
            DBUS_V_CH2[2] = 0.0f;
        }
    }
    else
    {
        DBUS_V_CH2[4] = 0.0f;
        MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += -(float)DBUS->REMOTE.CH2_int16 * 0.02f;
    }
    
//    MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += -(float)DBUS->REMOTE.CH2_int16 * 0.02f;
    MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM += -(float)DBUS->REMOTE.CH3_int16 * 0.01f;


    // if (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM > 5200)
    // {
    //     MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = 5200;
    // }
    // else if (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM < 4270)
    // {
    //     MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = 4270;
    // }
    if (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM > 3208)
    {
        MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = 3208;
    }
    else if (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM < 2543)
    {
        MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = 2543;
    }

    if (VISION_V_DATA.RECV_FLAG)
    {
        PID_F_VISION_YAW(&MOTOR[MOTOR_D_GIMBAL_YAW]);
        // PID_F_P(&MOTOR[MOTOR_D_GIMBAL_PIT]);
        return;
    }    
    PID_F_G(&MOTOR[MOTOR_D_GIMBAL_YAW]);
    PID_F_P(&MOTOR[MOTOR_D_GIMBAL_PIT]);
}
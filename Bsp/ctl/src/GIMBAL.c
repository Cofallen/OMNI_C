#include "main.h"

#include "GIMBAL.h"
#include "DEFINE.h"
#include "DBUS.h"
#include "YU_PID.h"

float yawInfinit = 0.0f;

void GIMBAL_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    MOTOR[MOTOR_D_GIMBAL_YAW].DATA.AIM += -(float)DBUS->REMOTE.CH2_int16 * 0.02f;
    MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM += -(float)DBUS->REMOTE.CH3_int16 * 0.01f;

    if (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM > 5200)
    {
        MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = 5200;
    }
    else if (MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM < 4270)
    {
        MOTOR[MOTOR_D_GIMBAL_PIT].DATA.AIM = 4270;
    }

    PID_F_AS(&MOTOR[MOTOR_D_GIMBAL_YAW]);
    PID_F_AS(&MOTOR[MOTOR_D_GIMBAL_PIT]);
}
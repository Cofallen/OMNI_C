#include "main.h"
#include <string.h>

#include "TOP.h"
#include "DEFINE.h"
#include "YU_MATH.h"
#include "TIM_DEV.h"

// float Top[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // add by yu 1-now 0-last 2-laps 3-infinite 4-0-close/1-open

TYPEDEF_TOP TOP = {0};

// 本代码根据瑞的代码修改而来
void TOP_F_Cal(void *TOP, const uint8_t *DATA)
{
    TYPEDEF_TOP_DATA *TOP_DATA = (TYPEDEF_TOP_DATA *)TOP;
    union TYPEDEF_TOP_DATA_UNION TOP_DATA_UNION = {0};

    // memcpy(TOP_DATA_UNION.DATA, DATA, sizeof(TOP_DATA_UNION.DATA));

    TOP_DATA->PIT_ANGLE[LAST] = TOP_DATA->PIT_ANGLE[NOW];
    TOP_DATA->YAW_ANGLE[LAST] = TOP_DATA->YAW_ANGLE[NOW];

    // TOP_DATA->PIT_ANGLE[NOW] = TOP_DATA_UNION.
}

float currentAngle = 0.0f;

void TOP_T_Cal()
{
    if (TOP.yaw[4] == 1.0f)
    {
        convertAngleToIndex(yaw, &TOP.yaw[NOW]);
        convertAngleToIndex(pitch, &TOP.pitch[NOW]);
        TOP.yaw[NOW] = TOP.yaw[NOW] + currentAngle;
    }
    
    if (TOP.yaw[NOW] - TOP.yaw[LAST] > 4096)
    {
        TOP.yaw[2]--;
    }
    else if (TOP.yaw[NOW] - TOP.yaw[LAST] < -4096)
    {
        TOP.yaw[2]++;
    }
    TOP.yaw[3] = TOP.yaw[2] * 8192.0f + TOP.yaw[NOW];
    TOP.yaw[LAST] = TOP.yaw[NOW];

    TOP.pitch[5] = pitch;
    TOP.yaw[5] = yaw;
    TOP.roll[5] = -roll;
}

void TOP_T_Monitor()
{
    if (yaw == 0.0f && pitch == 0.0f)
    {
        TOP.yaw[4] = 0.0f; // 0-close, offline
    }
    else
    {
        TOP.yaw[4] = 1.0f; // 1-open, online
    }

    if (TOP.yaw[4] == 0.0f) // offline
    {
        // top to motor angle
        TOP.yaw[NOW] = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW;
        TOP.yaw[2] = 0;
        currentAngle = 0;
    }
}

void TOP_T_Cal_T()
{
    if (TOP.yaw[4] == 1.0f)
    {
        convertAngleToIndex(-yaw, &TOP.yaw[NOW]);
        convertAngleToIndex(pitch, &TOP.pitch[NOW]);
        TOP.yaw[NOW] = TOP.yaw[NOW] + currentAngle;
    }
    
    if (TOP.yaw[NOW] - TOP.yaw[LAST] < 4096)
    {
        TOP.yaw[2]++;
    }
    else if (TOP.yaw[NOW] - TOP.yaw[LAST] > -4096)
    {
        TOP.yaw[2]--;
    }
    TOP.yaw[3] = TOP.yaw[2] * 8192.0f + TOP.yaw[NOW];
    TOP.yaw[LAST] = TOP.yaw[NOW];

    TOP.pitch[5] = pitch;
    TOP.yaw[5] = yaw;
}

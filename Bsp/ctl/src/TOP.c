#include "main.h"
#include <string.h>

#include "TOP.h"
#include "DEFINE.h"
#include "YU_MATH.h"
#include "TIM_DEV.h"

float Top[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // add by yu 1-now 0-last 2-laps 3-infinite 4-0-close/1-open

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

float abcdefg = 0.0f;

void TOP_T_Cal()
{
    if (Top[4] == 1.0f)
    {
        convertAngleToIndex(yaw, &Top[NOW]);
        Top[NOW] = Top[NOW] + abcdefg;
    }
    
    if (Top[NOW] - Top[LAST] > 4096)
    {
        Top[2]--;
    }
    else if (Top[NOW] - Top[LAST] < -4096)
    {
        Top[2]++;
    }
    Top[3] = Top[2] * 8192.0f + Top[NOW];
    Top[LAST] = Top[NOW];
}

void TOP_T_Monitor()
{
    if (yaw == 0.0f && pitch == 0.0f)
    {
        Top[4] = 0.0f; // 0-close, offline
    }
    else
    {
        Top[4] = 1.0f; // 1-open, online
    }

    if (Top[4] == 0.0f) // offline
    {
        // top to motor angle
        // Top[NOW] = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW;
        // Top[2] = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.LAPS;
        // abcdefg = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW;

        // shut down motor
        MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.AIM = 0.0f;

    }
}
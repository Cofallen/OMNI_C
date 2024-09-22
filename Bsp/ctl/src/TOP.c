#include "main.h"
#include <string.h>

#include "TOP.h"
#include "DEFINE.h"
#include "YU_MATH.h"

float Top[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // add by yu 1-now 0-last 2-laps 3-infinite

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

void TOP_T_Cal()
{
    Top[LAST] = Top[NOW];
    convertAngleToIndex(yaw, &Top[NOW]);
    if (Top[NOW] - Top[LAST] > 4096)
    {
        Top[2]++;
    }
    else if (Top[NOW] - Top[LAST] < -4096)
    {
        Top[2]--;
    }
    Top[3] = Top[2] * 8192.0f + Top[NOW];
}
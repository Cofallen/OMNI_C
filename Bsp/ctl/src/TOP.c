#include "main.h"
#include <string.h>

#include "TOP.h"
#include "DEFINE.h"

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
#include "main.h"

#include "VISION.h"
#include "DEFINE.h"

#define VISION_D_SEND 15
#define VISION_D_RECV 16

struct VISION_V_DATA
{
    float x;
};

uint8_t VISION_V_RECV[VISION_D_RECV] = {0};
uint8_t VISION_V_SEND[VISION_D_SEND] = {0};

// 视觉接收处理
uint8_t VISION_F_Cal(uint8_t *RxData)
{
    // copy 修正错位
    uint8_t i = 0, error = 0, data[15] = {0};
    for (int n = 0; n < VISION_D_RECV; n++)
    {
        if (RxData[n] == 0xCD)
        {
            error = n;
            break;
        }
    }
    for (int n = 0; n < VISION_D_RECV; n++)
    {
        data[n] = RxData[error++];
        if (error >= 15)
        {
            error = 0;
        }
    }

    if ((RxData[0] == 0xCD) && (RxData[VISION_D_RECV - 1] == 0xDC))
    {
        // 读取数据

        return ROOT_READY;
    }
    return ROOT_ERROR;
}

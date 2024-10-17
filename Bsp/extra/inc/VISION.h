#ifndef __VISION_H__
#define __VISION_H__

#include "stdbool.h"
union RUI_U_VISION_RECEIVE
{
  struct
  {
    float PIT_DATA;
    float YAW_DATA;
    bool TARGET;
  };
  uint8_t DATA[9];
};

union RUI_U_VISION_SEND
{
  struct
  {
    float PIT_DATA;
    float YAW_DATA;
    float INIT_FIRING_RATE; // 弹速
    int FLAG;            // 自瞄和能量机关切换标志位
    bool COLOR;             // TRUE是蓝色，FALSE是红色
    unsigned int TIME;
  };
  uint8_t DATA[21];
};

typedef struct RUI_TYPEDEF_VISION
{
  union RUI_U_VISION_RECEIVE RECEIVE;
  union RUI_U_VISION_SEND SEND;
} RUI_TYPEDEF_VISION;

union ReceiveDataUnion_typedef							//共用体(用于接受各种数据)(视觉，陀螺仪)
	{    
			uint8_t U[4];
			float F;
			int I;
	};

#endif // !__VISION_H__
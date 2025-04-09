#ifndef __VISION_H__
#define __VISION_H__

#include "stdbool.h"

// #pragma pack(push, 1)
#include "main.h"
union RUI_U_VISION_RECEIVE
{
  struct
  {
    // uint8_t HEAD;
    float PIT_DATA;
    float YAW_DATA;
    bool TARGET;
    bool fire;
  };
  uint8_t DATA[15];
};
// #pragma pack(pop)

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

typedef struct TYPEDEF_VISION
{
  union RUI_U_VISION_RECEIVE RECEIVE;
  union RUI_U_VISION_SEND SEND;
  uint8_t OriginData[15];
  uint8_t RECV_FLAG;
  int RECV_OutTime;
  float VISION_V_YAWERROR;
}TYPEDEF_VISION;

union ReceiveDataUnion_typedef							//共用体(用于接受各种数据)(视觉，陀螺仪)
	{    
			uint8_t U[4];
			float F;
			int I;
	};


extern struct TYPEDEF_VISION VISION_V_DATA;

uint8_t VISION_F_Cal(uint8_t *RxData, uint8_t type);
int ControltoVision(union RUI_U_VISION_SEND*  Send_t , uint8_t *buff, uint8_t type);
void VisionSendInit(union RUI_U_VISION_SEND*  Send_t);
void VISION_F_Monitor();

#endif // !__VISION_H__
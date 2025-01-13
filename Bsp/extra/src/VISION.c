#include "main.h"

#include "VISION.h"
#include "DEFINE.h"
#include "TOP.h"
#include "Read_Data.h"
#include "math.h"
#include "usbd_cdc_if.h"
#include "YU_MATH.h"

#define VISION_D_SEND 15
#define VISION_D_RECV 15

union ReceiveDataUnion_typedef	data_tackle ={0};
RUI_TYPEDEF_VISION VISION_V_DATA = {0};
// int a = sizeof(VISION_V_DATA);
// 视觉接收处理
uint8_t VISION_F_Cal(uint8_t *RxData)
{
    if ((RxData[0] == 0xaa) && (RxData[VISION_D_RECV - 1] == 0xbb))
    {
        // 读取数据
        data_tackle.U[0] = VISION_V_DATA.OriginData[1];
        data_tackle.U[1] = VISION_V_DATA.OriginData[2];
        data_tackle.U[2] = VISION_V_DATA.OriginData[3];
        data_tackle.U[3] = VISION_V_DATA.OriginData[4];
        VISION_V_DATA.RECEIVE.PIT_DATA= data_tackle.F;

        data_tackle.U[0] = VISION_V_DATA.OriginData[5];
        data_tackle.U[1] = VISION_V_DATA.OriginData[6];
        data_tackle.U[2] = VISION_V_DATA.OriginData[7];
        data_tackle.U[3] = VISION_V_DATA.OriginData[8];
        VISION_V_DATA.RECEIVE.YAW_DATA= data_tackle.F;

        // temp1 = VISION_V_DATA.OriginData[i++];
        //是否识别到目标

        memcpy(VISION_V_DATA.OriginData, RxData, VISION_D_RECV);
        VISION_V_DATA.RECEIVE.TARGET= (VISION_V_DATA.OriginData[9] & 0x10)>>4;//识别成功标志位

        return ROOT_READY;
    }
    return ROOT_ERROR;
}

void VisionSendInit(union RUI_U_VISION_SEND*  Send_t)
{
    Send_t->PIT_DATA = TOP.pitch[5];
    Send_t->YAW_DATA = TOP.yaw[5];
    Send_t->INIT_FIRING_RATE =user_data.shoot_data.initial_speed;
    Send_t->FLAG = VISION_V_DATA.SEND.FLAG;
    Send_t->COLOR = VISION_V_DATA.SEND.COLOR;
    Send_t->TIME = VISION_V_DATA.SEND.TIME;
}

int ControltoVision(union RUI_U_VISION_SEND*  Send_t , uint8_t *buff)
{
    uint8_t status;
   VisionSendInit(Send_t);
   buff[0] = 0xaa;
	//确定pitch轴角度//并且发送角度值
	data_tackle.F = Send_t->PIT_DATA;
	buff[1] = data_tackle.U[0];
	buff[2] = data_tackle.U[1];
	buff[3] = data_tackle.U[2];
	buff[4] = data_tackle.U[3];
	//确定yaw轴角度//并且发送角度值
	data_tackle.F = Send_t->YAW_DATA;
	buff[5] = data_tackle.U[0];
	buff[6] = data_tackle.U[1];
	buff[7] = data_tackle.U[2];
	buff[8] = data_tackle.U[3];
    //将请求的状态置于第九位中
	//2023-06-02 22:54 | 自瞄/打符标志位
    setbit(&buff[9], 0, Send_t->COLOR &0x01);
    //2023-06-02 22:54 | 颜色
	setbit(&buff[9] , 3 , Send_t->COLOR >> 4);

    data_tackle.I = Send_t->TIME;
	buff[10] = data_tackle.U[0];
	buff[11] = data_tackle.U[1];
	buff[12] = data_tackle.U[2];
	buff[13] = data_tackle.U[3];
    // buff[14] = user_data.shoot_data.initial_speed;
    buff[14] = 99;
    buff[15] = 0xbb;


    status = CDC_Transmit_FS(buff, 16);
    return status;

}
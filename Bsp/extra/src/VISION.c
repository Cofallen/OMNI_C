#include "main.h"

#include "VISION.h"
#include "DEFINE.h"
#include "TOP.h"
#include "Read_Data.h"
#include "math.h"
#define VISION_D_SEND 15
#define VISION_D_RECV 16

union ReceiveDataUnion_typedef	data_tackle ={0};
RUI_TYPEDEF_VISION VISION_V_DATA = {0};
uint8_t temp1 = 0;
uint8_t data[15] = {0} , i = 0;
// 假设数据错位修正
uint8_t VISION_F_Correct(uint8_t *RxData)
{
    uint8_t error = 0;
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
        if (error > 15)
        {
            error = 0;
        }
    }

    return ROOT_READY;
}

// 视觉接收处理
uint8_t VISION_F_Cal(uint8_t *RxData)
{
    // copy 修正错位
    VISION_F_Correct(RxData);
    if ((RxData[0] == 0xCD) && (RxData[VISION_D_RECV - 1] == 0xDC))
    {
        // 读取数据
        if(data [14] == 0xDC && data[i++] == 0xCD)
        {
            data_tackle.U[0] = data[i++];
            data_tackle.U[1] = data[i++];
            data_tackle.U[2] = data[i++];
            data_tackle.U[3] = data[i++];
            VISION_V_DATA.RECEIVE.PIT_DATA= data_tackle.F;

            data_tackle.U[0] = data[i++];
            data_tackle.U[1] = data[i++];
            data_tackle.U[2] = data[i++];
            data_tackle.U[3] = data[i++];
            VISION_V_DATA.RECEIVE.YAW_DATA= data_tackle.F;

            temp1 = data[i++];
            //是否识别到目标
            VISION_V_DATA.RECEIVE.TARGET= (temp1 & 0x10)>>4;//识别成功标志位
    
           
            
        }
        return ROOT_READY;
    }
    return ROOT_ERROR;
}

void VisionSendInit(union RUI_U_VISION_SEND*  Send_t)
{
    Send_t->PIT_DATA = 0;
    Send_t->YAW_DATA = Top[3];
    Send_t->INIT_FIRING_RATE =user_data.shoot_data.initial_speed;
    Send_t->FLAG = VISION_V_DATA.SEND.FLAG;
    Send_t->COLOR = VISION_V_DATA.SEND.COLOR;
    Send_t->TIME = VISION_V_DATA.SEND.TIME;
}

void ControltoVision(union RUI_U_VISION_SEND*  Send_t , uint8_t *buff)
{
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
    setbit(buff[9], 0, Send_t->COLOR &0x01);
    //2023-06-02 22:54 | 颜色
	setbit(&buff[9] , 3 , Send_t->COLOR >> 4);

    data_tackle.I = Send_t->TIME;
	buff[10] = data_tackle.U[0];
	buff[11] = data_tackle.U[1];
	buff[12] = data_tackle.U[2];
	buff[13] = data_tackle.U[3];
    buff[14] = user_data.shoot_data.initial_speed;
    buff[15] = 0xbb;

    HAL_UART_Transmit_DMA(&huart6,buff, 16);	           	        

}

#include "main.h"

#include "VISION.h"
#include "DEFINE.h"
#include "TOP.h"
#include "Read_Data.h"
#include "math.h"
#include "usbd_cdc_if.h"
#include "YU_MATH.h"
#include "robot.h"
#include "DBUS.h"

#define VISION_D_SEND 16
#define VISION_D_RECV 15

#define VISION_D_MONITOR_LEN 10
union ReceiveDataUnion_typedef	data_tackle ={0};
TYPEDEF_VISION VISION_V_DATA = {0};
float VisionMonitor[VISION_D_MONITOR_LEN] = {0}; // 只看pit数据是否变化判断离线
int VISION_Monitor_IOTA = 0;

/// @brief 视觉接收
/// @param RxData 原始数据
/// @param type 类型 0:虚拟串口 1:USART1
/// @return 
uint8_t VISION_F_Cal(uint8_t *RxData, uint8_t type)
{
    if ((RxData[0] == 0xcd) && (RxData[VISION_D_RECV - 1] == 0xdc))
    {
        if (type == 0) // 虚拟串口需要从缓冲区中读取数据
            memcpy(VISION_V_DATA.OriginData, RxData, VISION_D_RECV); 

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

        //是否识别到目标    
        VISION_V_DATA.RECEIVE.TARGET= (VISION_V_DATA.OriginData[9] & 0x10)>>4;//识别成功标志位
        VISION_V_DATA.RECEIVE.fire = (VISION_V_DATA.OriginData[9] & 0x08)>>3;
        VISION_V_DATA.RECEIVE.state = (VISION_V_DATA.OriginData[9] & 0x07); 
        VISION_V_DATA.RECV_FLAG[NOW] = ROOT_READY;

        return ROOT_READY;
    }
    return ROOT_ERROR;
}

void VisionSendInit(union RUI_U_VISION_SEND*  Send_t)
{
    static uint8_t buff_flag = 0;

    Send_t->PIT_DATA = TOP.roll[5];     // @note c板侧放，如果想用pitch建议改imu_temp...c中的IMU_QuaternionEKF_Update参数顺序和正负
    Send_t->YAW_DATA = -TOP.yaw[5];
    Send_t->INIT_FIRING_RATE =user_data.shoot_data.initial_speed;
    Send_t->FLAG = VISION_V_DATA.SEND.FLAG;
    Send_t->COLOR = VISION_V_DATA.SEND.COLOR;
    Send_t->TIME = VISION_V_DATA.SEND.TIME;
    Send_t->bulletSpeed = (uint8_t)user_data.shoot_data.initial_speed;

    if (DBUS_V_DATA.KEY_BOARD.B && !DBUS_V_DATA.KEY_BOARD.B_PREE_NUMBER) // 按下B键
        buff_flag = !buff_flag; // 切换打符模式
    DBUS_V_DATA.KEY_BOARD.B_PREE_NUMBER = DBUS_V_DATA.KEY_BOARD.B;
    Send_t->is_buff = buff_flag;
}

/// @brief 视觉发送
/// @param Send_t 发送变量
/// @param buff 发送处理数据
/// @param type 类型 0:虚拟串口 1:USART1
/// @return 
int ControltoVision(union RUI_U_VISION_SEND*  Send_t , uint8_t *buff, uint8_t type)
{
    uint8_t status;
   VisionSendInit(Send_t);
    buff[0] = 0xcd;
	//pitch
	data_tackle.F = Send_t->PIT_DATA;
	buff[1] = data_tackle.U[0];
	buff[2] = data_tackle.U[1];
	buff[3] = data_tackle.U[2];
	buff[4] = data_tackle.U[3];
	//yaw
	data_tackle.F = Send_t->YAW_DATA;
	buff[5] = data_tackle.U[0];
	buff[6] = data_tackle.U[1];
	buff[7] = data_tackle.U[2];
	buff[8] = data_tackle.U[3];
    //将请求的状态置于第九位中
	//2023-06-02 22:54 | 自瞄/打符标志位
    // setbit(&buff[9], 0, Send_t->COLOR &0x01);
    // //2023-06-02 22:54 | 颜色
	// setbit(&buff[9] , 3 , Send_t->COLOR >> 4);
    // 自瞄0 打符1
    buff[9] = Send_t->is_buff;
    data_tackle.I = (uint32_t)Send_t->TIME; // 视觉自瞄和能量机关切换标志位
	buff[10] = data_tackle.U[0];
	buff[11] = data_tackle.U[1];
	buff[12] = data_tackle.U[2];
	buff[13] = data_tackle.U[3];
    buff[14] = Send_t->bulletSpeed;
    buff[15] = 0xdc;

    if (type == 0)
        status = CDC_Transmit_FS(buff, 16);
    else if (type == 1)
        status = HAL_UART_Transmit_DMA(&huart1, buff, 16);

    return ROOT_READY;
}

int errcount = 0;
void VISION_F_Monitor()
{
    VISION_V_DATA.RECV_FLAG[LAST] = VISION_V_DATA.RECV_FLAG[NOW]; // 上一帧数据

    if (VISION_V_DATA.RECV_FLAG[NOW] == ROOT_ERROR)
    {
        VISION_V_DATA.RECV_OutTime++;
        if (VISION_V_DATA.RECV_OutTime >= 1000000)  // 防止越界
        {
            VISION_V_DATA.RECV_OutTime = 500;
        }
        
    }
    if (VISION_V_DATA.RECV_FLAG[NOW] == ROOT_READY)
    {
        VISION_V_DATA.RECV_OutTime = 0;
    }

    VisionMonitor[VISION_Monitor_IOTA++] = VISION_V_DATA.RECEIVE.PIT_DATA;
    if (VISION_Monitor_IOTA >= (VISION_D_MONITOR_LEN - 1))
    {
        VISION_Monitor_IOTA = 0;
    }
    
    int err = 0;
    for (int i = 1; i < VISION_D_MONITOR_LEN; i++)
    {
        if(VisionMonitor[i] == VisionMonitor[i - 1])
            err++;
    }
    
    errcount = err;
      
    if ((VISION_V_DATA.RECV_OutTime >= 500) || (err >= (VISION_D_MONITOR_LEN - 3))) // 500ms
    {
        VISION_V_DATA.RECV_FLAG[NOW] = ROOT_ERROR;
        
    }
}
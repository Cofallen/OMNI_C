#include "usart.h"

#include "DBUS.h"
#include "USART_DEV.h"
#include <string.h>

#include "usart.h"

#include "Read_Data.h"
#include "VOFA.h"
#include "ROOT.h"

#define BUFFER_SIZE (255)

extern DMA_HandleTypeDef hdma_usart6_rx;

// @TODO 原回调函数DMA不能?，改用自定义回调函数

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == USART3) // 遥控DBUS
//     {
//         DBUS_F_Cal(&DBUS_V_DATA);
//         HAL_UART_Receive_IT(&huart3, DBUS_V_UNION.GET_DATA, sizeof(DBUS_V_UNION.GET_DATA));
//     }
// }

// 使用视觉串口huart1
int fputc(int ch, FILE *f) // VOFA 的重定向
{
    HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ch, 1);
    return ch;
}

uint8_t data_length; // 声明外部变量

void USAR_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    HAL_UART_DMAStop(&huart6); // 停止本次DMA传输                                            

    data_length = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart6_rx); // 计算接收到的数据长度

    Read_Data_first(&ALL_RX, &user_data, data_length); // 测试函数：待修改

    memset((uint8_t *)ALL_RX.Data, 0, data_length); // 清零接收缓冲区
    data_length = 0;
    HAL_UART_Receive_DMA(&huart6, (uint8_t *)ALL_RX.Data, 255); // 重启开始DMA传输 每次255字节数据
}

// 移植的函数
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) // 遥控DBUS
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_DMAStop(&huart3);
        
        ROOT_V_MONITOR_DBUS = 0; //  离线判断参数清零
        
        DBUS_F_Cal(&DBUS_V_DATA);
        HAL_UART_Receive_DMA(&huart3, (uint8_t *)DBUS_V_UNION.GET_DATA, sizeof(DBUS_V_UNION.GET_DATA));
    }

    // 裁判系统
    if (huart->Instance == USART6)
    {
        if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_IDLE) != RESET) // 判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart6); // 清楚空闲中断标志（否则会一直不断进入中断）
            
            USAR_UART_IDLECallback(huart); // 调用中断处理函数
        }
    }

    if (huart->Instance == USART1) // 视觉模块/VOFA测试模块
    {
         
    }
}

void USER_UART3_IRQHandler()
{
    HAL_UART_DMAStop(&huart3);
    
    // 使能接受
    
    ROOT_V_MONITOR_DBUS = 0; //  离线判断参数清零
    
    DBUS_F_Cal(&DBUS_V_DATA);
    HAL_UART_Receive_DMA(&huart3, (uint8_t *)DBUS_V_UNION.GET_DATA, 18);
}
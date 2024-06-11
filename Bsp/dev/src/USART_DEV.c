#include "usart.h"

#include "DBUS.h"
#include "USART_DEV.h"
#include <string.h>

// @TODO 原回调函数DMA不能用，改用自定义回调函数

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == USART3) // 遥控DBUS
//     {
//         DBUS_F_Cal(&DBUS_V_DATA);
//         HAL_UART_Receive_IT(&huart3, DBUS_V_UNION.GET_DATA, sizeof(DBUS_V_UNION.GET_DATA));
//     }
// }

int fputc(int ch, FILE *f) // VOFA 的重定向
{
    HAL_UART_Transmit_IT(&huart6, (uint8_t *)&ch, 1);
    return ch;
}

// 移植的函数
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) // 判断是否是串口1（！此处应写(huart->Instance == USART1)
    {
        if (RESET != __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE)) // 判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart3); // 清楚空闲中断标志（否则会一直不断进入中断）

            HAL_UART_DMAStop(&huart3); // 停止DMA
            // 使能接受
            HAL_UART_Receive_DMA(&huart3, (uint8_t *)DBUS_V_UNION.GET_DATA, 19);

            DBUS_F_Cal(&DBUS_V_DATA);
        }
    }
}

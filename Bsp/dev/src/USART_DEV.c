#include "usart.h"

#include "DBUS.h"
#include "USART_DEV.h"

//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) // 遥控DBUS
    {
        DBUS_F_Cal(&DBUS_V_DATA);
        HAL_UART_Receive_DMA(&huart3, DBUS_V_UNION.GET_DATA, sizeof(DBUS_V_UNION.GET_DATA));
    }
}

int fputc(int ch, FILE *f) // VOFA 的重定向
{
    HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ch, 1);
    return ch;
}

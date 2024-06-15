#include "usart.h"

#include "DBUS.h"
#include "USART_DEV.h"
#include <string.h>

#include "usart.h"

#include "Read_Data.h"
#include "Anonymity_TX.h" // copy ， 待改成VOFA
#include "VOFA.h"
#define  BUFFER_SIZE  (255)

extern DMA_HandleTypeDef hdma_usart6_rx;
uint16_t II;
User_Data_T test;

// @TODO 原回调函数DMA不能�?，改用自定义回调函数

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == USART3) // 遥控DBUS
//     {
//         DBUS_F_Cal(&DBUS_V_DATA);
//         HAL_UART_Receive_IT(&huart3, DBUS_V_UNION.GET_DATA, sizeof(DBUS_V_UNION.GET_DATA));
//     }
// }

//int fputc(int ch, FILE *f) // VOFA 的重定向
//{
//    HAL_UART_Transmit_IT(&huart6, (uint8_t *)&ch, 1);
//    return ch;
//}



uint8_t data_length;  //声明外部变量
                                            
void USAR_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    HAL_UART_DMAStop(&huart6);        //停止本次DMA传输                                             //ֹͣ����DMA����
    
    data_length  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart6_rx);   //计算接收到的数据长度

//    printf("Receive Data(length = %d): ",data_length);
	Read_Data_first(&ALL_RX , &user_data , data_length);					//测试函数：待修改
	
//    printf("\r\n");
//	Anonymity_TX(data_length,0,0,0,0,0,0,0,0,0);	
    memset((uint8_t*)ALL_RX.Data,0,data_length);                                            //清零接收缓冲区
    data_length = 0;
    HAL_UART_Receive_DMA(&huart6, (uint8_t*)ALL_RX.Data, 255);                    //重启开始DMA传输 每次255字节数据
	
}


// 移�?�的函数
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) //判断是否是串口1（！此处应写(huart->Instance == USART1)
    {
        if (RESET != __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))  //判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart3); //清楚空闲中断标志（否则会一直不断进入中断）

            HAL_UART_DMAStop(&huart3); // 停�??DMA
            // 使能接受
            HAL_UART_Receive_DMA(&huart3, (uint8_t *)DBUS_V_UNION.GET_DATA, 19);

            DBUS_F_Cal(&DBUS_V_DATA);
        }
    }

    // 裁判系统
    if (huart->Instance == USART6) 
    {
        if(__HAL_UART_GET_FLAG(&huart6, UART_FLAG_IDLE) != RESET)   //判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart6);                     //清楚空闲中断标志（否则会一直不断进入中断）
//            printf("\r\nUART1 Idle IQR Detected\r\n");
            USAR_UART_IDLECallback(huart);                          //调用中断处理函数
					
					II = ALL_RX.RX_Data_game_robot_HP.game_robot_HP.blue_3_robot_HP;
        }
				Read_Data_system(&ALL_RX,&test);
    }
    
}

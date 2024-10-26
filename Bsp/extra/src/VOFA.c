#include "main.h"
#include <string.h>
#include "usart.h"
#include <stdarg.h>

#include "VOFA.h"
#include "MOTOR.h"
#include "DEFINE.h"
#include "stdlib.h"

union 
{
    float DATA[11];
    int8_t TAIL[44];
}VOFA_T_DATA={0};

struct Data
{
    float *data;
    char *tail;
};
    
// 发送VOFA数据包
void VOFA_F_Send(TYPEDEF_VOFA_UNION *VOFA, TYPEDEF_MOTOR *MOTOR)
{
    VOFA->DATA.MOTOR.MOTOR_DATA.ANGLE = MOTOR->DATA.ANGLE_NOW;
    VOFA->DATA.MOTOR.MOTOR_DATA.SPEED = MOTOR->DATA.SPEED_NOW;
    VOFA->DATA.MOTOR.MOTOR_DATA.CURRENT = MOTOR->DATA.CURRENT;
    VOFA->DATA.MOTOR.MOTOR_DATA.ANGLE_INFINITE = MOTOR->DATA.ANGLE_INFINITE;
    VOFA->DATA.MOTOR.MOTOR_DATA.LAPS = MOTOR->DATA.LAPS;
    VOFA->DATA.MOTOR.MOTOR_DATA.AIM = MOTOR->DATA.AIM;

    memcpy(&VOFA->DATA.MOTOR.PID_A, &MOTOR->PID_A, sizeof(TYPEDEF_MOTOR_PID));
    memcpy(&VOFA->DATA.MOTOR.PID_S, &MOTOR->PID_S, sizeof(TYPEDEF_MOTOR_PID));
    memcpy(&VOFA->DATA.MOTOR.PID_C, &MOTOR->PID_C, sizeof(TYPEDEF_MOTOR_PID));

    VOFA->DATA.TAIL[0] = 0x00;
    VOFA->DATA.TAIL[1] = 0x00;
    VOFA->DATA.TAIL[2] = 0x80;
    VOFA->DATA.TAIL[3] = 0x7F;

    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)VOFA->ALL, sizeof(VOFA->ALL));
}

// VOFA 使用 huart1 发送任意个数数据
// 先使用定长数组接收 @TODO 使用malloc分配内存，释放内存
//                  @update 暂时完成
uint8_t VOFA_T_Send(int n, ...)
{
    va_list list;
    va_start(list, n);
    
    struct Data *VOFA_T_DATA = (struct Data *)calloc(1, sizeof(struct Data));
    VOFA_T_DATA->data = (float *)calloc(n, sizeof(float));
    VOFA_T_DATA->tail = (char *)calloc(4, sizeof(char));

    for (int i = 0; i < n; i++)
    {
        VOFA_T_DATA->data[i] = (float)va_arg(list, double);
    }
    va_end(list);

    VOFA_T_DATA->tail[0] = 0x00;
    VOFA_T_DATA->tail[1] = 0x00;
    VOFA_T_DATA->tail[2] = 0x80;
    VOFA_T_DATA->tail[3] = 0x7f;

//    HAL_UART_Transmit_IT(&huart1, (uint8_t *)VOFA_T_DATA, sizeof(struct Data));
		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)VOFA_T_DATA, sizeof(struct Data));

    free(VOFA_T_DATA->data);
    free(VOFA_T_DATA->tail);
    free(VOFA_T_DATA);

    return ROOT_READY;
}

uint8_t VOFA_T_SendTemp(int n, ...)
{
    va_list list;
    va_start(list, n);
    
    for (int i = 0; i < n; i++)
    {
        VOFA_T_DATA.DATA[i] = (float)va_arg(list, double);
    }
    va_end(list);

    for (int i = n; i < 10; i++)
    {
        VOFA_T_DATA.DATA[i] = 0.0f;
    }
    VOFA_T_DATA.TAIL[0] = 0x00;
    VOFA_T_DATA.TAIL[1] = 0x00;
    VOFA_T_DATA.TAIL[2] = 0x80;
    VOFA_T_DATA.TAIL[3] = 0x7f;
    //调试串口3的空闲中断接收后是否能正常发送
    // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)VOFA_T_DATA.TAIL, sizeof(VOFA_T_DATA));

    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)VOFA_T_DATA.TAIL, sizeof(VOFA_T_DATA));

    return ROOT_READY;
}
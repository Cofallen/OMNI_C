#include "main.h"
#include <string.h>
#include "usart.h"
#include <stdarg.h>

#include "VOFA.h"
#include "MOTOR.h"
#include "DEFINE.h"

union 
{
    float DATA[11];
    int8_t TAIL[44];
}VOFA_T_DATA={0};


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
uint8_t VOFA_T_Send(int n, ...)
{
    va_list list;
    va_start(list, n);

    for (int i = 0; i < n; i++)
    {
        VOFA_T_DATA.DATA[i] = (float)(va_arg(list, double));
    }
    va_end(list);

    VOFA_T_DATA.TAIL[40] = 0x00;
    VOFA_T_DATA.TAIL[41] = 0x00;
    VOFA_T_DATA.TAIL[42] = 0x80;
    VOFA_T_DATA.TAIL[43] = 0x7f;

    HAL_UART_Transmit_IT(&huart1, (uint8_t *)VOFA_T_DATA.TAIL, sizeof(VOFA_T_DATA));

    return ROOT_READY;
}
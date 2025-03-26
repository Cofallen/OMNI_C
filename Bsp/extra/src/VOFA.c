#include "main.h"
#include <string.h>
#include "usart.h"
#include <stdarg.h>

#include "VOFA.h"
#include "MOTOR.h"
#include "DEFINE.h"
#include "stdlib.h"

#include "TIM_DEV.h"

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
    
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)VOFA_T_DATA.TAIL, sizeof(VOFA_T_DATA));

    return ROOT_READY;
}


void VOFA_T_Vision()
{
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)sd_v_buff, sizeof(sd_v_buff));
}



// // niming function
// uint8_t buffer[26] = {0};
// void niming(int16_t id, int16_t x1, int16_t x2, int16_t x3, int16_t y1, int16_t y2, int16_t y3, int16_t z1, int16_t z2, int16_t z3, int16_t w)
// {
// //    if(id>0xfa || id<0xf1)
// //        return;

    
//     uint8_t addCheck=0,sumCheck=0;

//     memset(buffer, 0, sizeof(buffer));          //stdlib.h
//     sumCheck=0,addCheck=0;

//     buffer[0]=0xaa;         
//     buffer[1]=0xff;         
//     buffer[2]=id;         

//     buffer[4]=(x1 & 0x00ff);
//     buffer[5]=((x1>>8) & 0x00ff);

//     buffer[6]=(x2 & 0x00ff);
//     buffer[7]=((x2>>8) & 0x00ff);
    
//     buffer[8]=(x3 & 0x00ff);
//     buffer[9]=((x3>>8) & 0x00ff);

//     buffer[10]=(y1 & 0x00ff);
//     buffer[11]=((y1>>8) & 0x00ff);

//     buffer[12]=(y2 & 0x00ff);
//     buffer[13]=((y2>>8) & 0x00ff);
    
//     buffer[14]=(y3 & 0x00ff);
//     buffer[15]=((y3>>8) & 0x00ff);

//     buffer[16]=(z1 & 0x00ff);
//     buffer[17]=((z1>>8) & 0x00ff);

//     buffer[18]=(z2 & 0x00ff);
//     buffer[19]=((z2>>8) & 0x00ff);
    
//     buffer[20]=(z3 & 0x00ff);
//     buffer[21]=((z3>>8) & 0x00ff);

//     buffer[22]=(w & 0x00ff);
//     buffer[23]=((w>>8) & 0x00ff);

//     buffer[3]=20;

//     for(int i=0;i<buffer[3]+4;i++)
//     {
//         sumCheck+=buffer[i];
//         addCheck+=sumCheck;
//     }

//     buffer[24]=sumCheck & 0x000000ff;
//     buffer[25]=addCheck & 0x000000ff;

// 	HAL_UART_Transmit_DMA(&huart1,buffer,sizeof(buffer));
// }


uint8_t buffer[26] = {0};
void niming(int16_t id, int16_t x1, int16_t x2, int16_t x3, int16_t y1, int16_t y2, int16_t y3, int16_t z1, int16_t z2, int16_t z3, int16_t w)
{
//    if(id>0xfa || id<0xf1)
//        return;

    
    uint8_t addCheck=0,sumCheck=0;

    memset(buffer, 0, sizeof(buffer));          //stdlib.h
    sumCheck=0,addCheck=0;

    buffer[0]=0xaa;         
    buffer[1]=0xff;         
    buffer[2]=id;         

    buffer[4]=(x1 & 0x00ff);
    buffer[5]=((x1>>8) & 0x00ff);

    buffer[6]=(x2 & 0x00ff);
    buffer[7]=((x2>>8) & 0x00ff);
    
    buffer[8]=(x3 & 0x00ff);
    buffer[9]=((x3>>8) & 0x00ff);

    buffer[10]=(y1 & 0x00ff);
    buffer[11]=((y1>>8) & 0x00ff);

    buffer[12]=(y2 & 0x00ff);
    buffer[13]=((y2>>8) & 0x00ff);
    
    buffer[14]=(y3 & 0x00ff);
    buffer[15]=((y3>>8) & 0x00ff);

    buffer[16]=(z1 & 0x00ff);
    buffer[17]=((z1>>8) & 0x00ff);

    buffer[18]=(z2 & 0x00ff);
    buffer[19]=((z2>>8) & 0x00ff);
    
    buffer[20]=(z3 & 0x00ff);
    buffer[21]=((z3>>8) & 0x00ff);

    buffer[22]=(w & 0x00ff);
    buffer[23]=((w>>8) & 0x00ff);

    buffer[3]=20;

    for(int i=0;i<buffer[3]+4;i++)
    {
        sumCheck+=buffer[i];
        addCheck+=sumCheck;
    }

    buffer[24]=sumCheck & 0x000000ff;
    buffer[25]=addCheck & 0x000000ff;

	HAL_UART_Transmit_DMA(&huart1,buffer,sizeof(buffer));
}

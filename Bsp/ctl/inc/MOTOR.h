#ifndef __MOTOR_H__
#define __MOTOR_H__

// 根据瑞代码精简所得
#include "main.h"

// 电机数据结构体打印模板
typedef struct TYPEDEF_MOTOR_DATA
{
    int8_t ONLINE_JUDGE_TIME; //
    int16_t ANGLE_LAST;
    int16_t ANGLE_NOW;
    int16_t SPEED_LAST;
    int16_t SPEED_NOW;
    int16_t CURRENT;
    int32_t ANGLE_INFINITE;

    uint16_t STUCK_TIME;
    int16_t LAPS;
    float AIM;
    float AIM_LAST;

    int16_t CAN_SEND; // CAN发送数据

    int16_t TEMP;

    int ANGLE_INIT; // 云台初始角度
    int ANGLE_MAX;  // 云台最大角度
    int ANGLE_MIN;  // 云台最小角度
} TYPEDEF_MOTOR_DATA;

// PID参数打印模板
typedef struct TYPEDEF_MOTOR_PID
{
    struct
    {
        float KP;
        float KI;
        float KD;
        float I_LIT;
        float ALL_LIT;
    } IN;
    struct
    {
        float ERROR[2];
        float P_OUT;
        float I_OUT;
        float D_OUT;
        float ALL_OUT;
    } OUT;
} TYPEDEF_MOTOR_PID;

// 电机数据结构体打印模板
typedef struct
{
    uint8_t PID_INIT;
    TYPEDEF_MOTOR_DATA DATA; // 电机数据
    TYPEDEF_MOTOR_PID PID_A; // 角度环
    TYPEDEF_MOTOR_PID PID_S; // 速度环
    TYPEDEF_MOTOR_PID PID_C; // 电流环
} TYPEDEF_MOTOR;

extern void MOTOR_F_Cal_Data(TYPEDEF_MOTOR *MOTOR, uint8_t *RxMessage);
extern void MOTOR_F_Cal_Round(TYPEDEF_MOTOR *MOTOR);

#endif // !__MOTOR_H__
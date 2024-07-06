#ifndef __VOFA_H__
#define __VOFA_H__

#include "main.h"

#include "MOTOR.h"

// VOFA 数据包
typedef struct TYPEDEF_VOFA
{
    struct
    {
        float ANGLE;
        float SPEED;
        float CURRENT;
        float ANGLE_INFINITE;
        float LAPS;
        float AIM;
    } MOTOR_DATA;

    TYPEDEF_MOTOR_PID PID_A;
    TYPEDEF_MOTOR_PID PID_S;
    TYPEDEF_MOTOR_PID PID_C;
} TYPEDEF_VOFA;

typedef union
{
    struct __packed
    {
        TYPEDEF_VOFA MOTOR;
        char TAIL[4];
    } DATA;
    char ALL[160];
} TYPEDEF_VOFA_UNION;

extern void VOFA_F_Send(TYPEDEF_VOFA_UNION *VOFA, TYPEDEF_MOTOR *MOTOR);
extern uint8_t VOFA_T_Send(int n, ...);

#endif // !__VOFA_H__

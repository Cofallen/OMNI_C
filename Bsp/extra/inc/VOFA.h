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
extern uint8_t VOFA_T_SendTemp(int n, ...);
extern void VOFA_T_Vision();

void niming(int16_t id, int16_t x1, int16_t x2, int16_t x3, int16_t y1, int16_t y2, int16_t y3, int16_t z1, int16_t z2, int16_t z3, int16_t w);

#endif // !__VOFA_H__

#ifndef __TOP_H__
#define __TOP_H__

#include "QuaternionEKF.h"

typedef struct TYPEDEF_TOP_DATA
{
    float REALITY_ANGLE; // 真实角度//带圈数
    int16_t YAW_ANGLE[2];
    float YAW_ANGLE_F;
    int16_t YAW_SPEED[2];
    int16_t PIT_ANGLE[2];
    float PIT_ANGLE_F;
    int16_t PIT_SPEED[2];
    int16_t ROUND;
} TYPEDEF_TOP_DATA;

typedef union TYPEDEF_TOP_DATA_UNION // 共用体(用于接受各种数据)(视觉，陀螺仪)
{
    struct
    {
        int16_t YAW_ANGLE;
        int16_t PIT_ANGLE;
        int16_t YAW_SPEED;
        int16_t PIT_SPEED;
    };
    uint8_t GET_DATA[8];
} TYPEDEF_TOP_DATA_UNION;


typedef struct TYPEDEF_TOP
{
    float yaw[6];   // add by yu 1-now 0-last 2-laps 3-infinite 4-0-close/1-open
    float pitch[6]; // add by yu 1-now 0-last 2-laps 3-infinite 4-0-close/1-open
    float roll[6];  // add by yu 1-now 0-last 2-laps 3-infinite 4-0-close/1-open
}TYPEDEF_TOP;

extern float yaw, pitch, roll;
extern QEKF_INS_t QEKF_INS;

// extern float Top[5];
extern TYPEDEF_TOP TOP;
extern float currentAngle; // 陀螺仪yaw偏差角度

extern void TOP_T_Cal();
extern void TOP_T_Monitor();
void TOP_T_Cal_T();

#endif // !__TOP_H__
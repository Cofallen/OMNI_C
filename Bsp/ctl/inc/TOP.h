#ifndef __TOP_H__
#define __TOP_H__

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

extern float yaw;
extern float Top[4];

extern void TOP_T_Cal();

#endif // !__TOP_H__
#ifndef __ATTACK_H__
#define __ATTACK_H__

#include "main.h"

#include "MOTOR.h"
#include "DBUS.h"
#include "JUDGEMENT.h"

typedef struct 
{
    int32_t TIME;
    int8_t FLAG;        // 卡弹变换方向
    float SINGLE_ANGLE; // 单发角度
    float SPEED;        // 摩擦轮角度
    int COUNT;
    int8_t LOCK;
    int8_t STATUS;      // jam status
    uint8_t PREV_MOUSE_STATE; // 上次左鼠标状态，控制拨盘旋转
    uint8_t fire_wheel_status; // 摩擦轮状态

}TYPEDEF_ATTACK_PARAM;

extern TYPEDEF_ATTACK_PARAM ATTACK_V_PARAM;

uint8_t ATTACK_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);
uint8_t ATTACK_F_Init(TYPEDEF_MOTOR *MOTOR);
double *ATTACK_T_FIT(int size);

#endif // !__ATTACK_H__

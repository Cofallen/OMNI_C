#ifndef __ATTACK_H__
#define __ATTACK_H__

#include "main.h"

#include "MOTOR.h"
#include "DBUS.h"
#include "JUDGEMENT.h"

typedef struct 
{
    int32_t TIME;
    int8_t FLAG;
    float SINGLE_ANGLE; // 单发角度
    float SPEED;        // 摩擦轮角度
    int COUNT;
}TYPEDEF_ATTACK_PARAM;

extern TYPEDEF_ATTACK_PARAM ATTACK_V_PARAM;

uint8_t ATTACK_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);

#endif // !__ATTACK_H__

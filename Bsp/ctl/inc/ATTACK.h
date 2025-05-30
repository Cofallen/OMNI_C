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
    uint8_t STATUS[2];      // 拨盘到达 status 0到达 1未到
    uint8_t PREV_MOUSE_STATE; // 上次左鼠标状态，控制拨盘旋转
    uint8_t fire_wheel_status; // 摩擦轮状态
    float jam_dwt_time;
    uint8_t is_jam;      // 卡弹状态
}TYPEDEF_ATTACK_PARAM;

extern TYPEDEF_ATTACK_PARAM ATTACK_V_PARAM;
extern float initial_speed[2];

uint8_t ATTACK_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);
uint8_t ATTACK_F_Init(TYPEDEF_MOTOR *MOTOR);
double *ATTACK_T_FIT(int size);
uint8_t ATTACK_F_JAM_Disable(TYPEDEF_MOTOR *MOTOR);
float ATTACK_F_FireRate_Control(TYPEDEF_MOTOR *motor, float hz, uint8_t type);
uint8_t ATTACK_F_HeatControl(TYPEDEF_MOTOR *motor, uint8_t type);
float ATTACK_Calc_Hz_From_FactHz(float fact_hz, float a, float P);

#endif // !__ATTACK_H__

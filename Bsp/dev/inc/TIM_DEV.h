#ifndef __TIM_DEV_H__
#define __TIM_DEV_H__

#include "main.h"
#include "MOTOR.h"

// 全局变量声明
extern TYPEDEF_MOTOR MOTOR_V_CHASSIS[4];
extern TYPEDEF_MOTOR MOTOR_V_GIMBAL[2];
extern TYPEDEF_MOTOR MOTOR_V_ATTACK[3];

extern uint8_t sd_v_buff[16];
extern uint64_t RunTime;

#endif // !__TIM_DEV_H__

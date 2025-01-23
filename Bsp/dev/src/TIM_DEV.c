#include "main.h"
#include "tim.h"
#include "can.h"

#include "TIM_DEV.h"
#include "CAN_DEV.h"
#include "MOTOR.h"
#include "DEFINE.h"
#include "CHASSIS.h"
#include "DBUS.h"
#include "GIMBAL.h"
#include "ROOT.h"
#include "ATTACK.h"
#include "VOFA.h"
#include "TOP.h"
#include "VISION.h"

// 一些全局变量
TYPEDEF_MOTOR MOTOR_V_CHASSIS[4] = {0}; // 底盘数据
TYPEDEF_MOTOR MOTOR_V_GIMBAL[2] = {0};  // 云台数据
TYPEDEF_MOTOR MOTOR_V_ATTACK[3] = {0};  // 拨弹数据

uint8_t sd_v_buff[16] = {0};
uint64_t RunTime = 0;

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim->Instance == TIM10) // 云台 0.01ms
//     {

//     }
// }
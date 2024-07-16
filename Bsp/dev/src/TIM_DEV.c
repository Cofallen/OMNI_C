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
#include "Read_Data.h"
#include "superCap.h"
#include "power_control.h"
uint64_t RunTime=0;//时间轴

// 一些全局变量
TYPEDEF_MOTOR MOTOR_V_CHASSIS[4] = {0}; // 底盘数据
TYPEDEF_MOTOR MOTOR_V_GIMBAL[2] = {0};  // 云台数据
TYPEDEF_MOTOR MOTOR_V_ATTACK[3] = {0};  // 拨弹数据
int16_t powersum = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) // 底盘 1ms
    {
        powersum = CHASSIS_F_Ctl(MOTOR_V_CHASSIS, &DBUS_V_DATA);
        chassis_power_control(1);
         //SuperCapTransation(1, (int32_t)powersum, RunTime);
        // CAN_F_Send(&hcan1, 0x200, MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CAN_SEND,
        //            MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CAN_SEND,
        //            MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CAN_SEND,
        //            MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CAN_SEND);
		// 	  CapSendInit(user_data.power_heat_data.buffer_energy , user_data.power_heat_data.chassis_power, user_data.power_heat_data.chassis_voltage);
				
    }
    if (htim->Instance == TIM4) // 云台 1ms
    {
//        GIMBAL_F_Ctl(MOTOR_V_GIMBAL, &DBUS_V_DATA);
//        CAN_F_Send(&hcan2, 0x2FF, MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.CAN_SEND,
//                   0,
//                   MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.CAN_SEND,
//                   0);
    }
    if (htim->Instance == TIM6) // 不知道 1ms
    {

//        ATTACK_F_Ctl(MOTOR_V_ATTACK, &DBUS_V_DATA);
        CAN_F_Send(&hcan2, 0x1FF, MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.CAN_SEND,
                   MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.CAN_SEND,
                   MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.CAN_SEND,
                   0);
    }
    if (htim->Instance == TIM9) // 离线监测 1ms
    {
        ROOT_F_MONITOR_DBUS(&DBUS_V_DATA);
			  ++RunTime;//时间轴
    }
}
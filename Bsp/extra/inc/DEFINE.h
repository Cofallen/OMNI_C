#ifndef __DEFINE_H__
#define __DEFINE_H__

// 存放所有宏定义，除去数学计算的宏定义

// CAN ID
#define CAN_D_1 0
#define CAN_D_2 1
// 底盘
#define CAN_D_CHASSIS_1 0x201
#define CAN_D_CHASSIS_2 0x202
#define CAN_D_CHASSIS_3 0x203
#define CAN_D_CHASSIS_4 0x204
// 电容
#define CAN_D_CAP 0x206
// 陀螺仪
#define CAN_D_TOP 0x101
// 云台
#define CAN_D_GIMBAL_YAW 0x206
#define CAN_D_GIMBAL_PIT 0x205
// 发射
#define CAN_D_ATTACK_L 0x201
#define CAN_D_ATTACK_R 0x202
#define CAN_D_ATTACK_G 0x203

// 底盘电机
#define MOTOR_D_CHASSIS_1 0
#define MOTOR_D_CHASSIS_2 1
#define MOTOR_D_CHASSIS_3 2
#define MOTOR_D_CHASSIS_4 3
// 云台电机
#define MOTOR_D_GIMBAL_YAW 0
#define MOTOR_D_GIMBAL_PIT 1
#define MOTOR_D_ATTACK_L 0
#define MOTOR_D_ATTACK_R 1
#define MOTOR_D_ATTACK_G 2
// 电机类型
#define MOTOR_TYPE_3508 1
#define MOTOR_TYPE_3510 2
#define MOTOR_TYPE_6020 3
#define MOTOR_TYPE_2006 4

// PID所需区分电机宏定义
#define PID_D_CHASSIS 0
#define PID_D_GIMBAL 1
#define PID_D_ATTACK 2
#define PID_D_SPEED 0
#define PID_D_ANGLE 1
#define PID_D_CURRENT 2
// 离线监测
#define NOW 1
#define LAST 0

#define MONITOR_OFFLINE false
#define MONITOR_ONLINE true

// 模式
#define DBUS_D_MOD_SINGLE 2
#define DBUS_D_MOD_CONSIST 1
#define DBUS_D_MOD_SHUT 3

// ROOT信号，涉及程序运行状态
#define ROOT_READY 1
#define ROOT_ERROR 0

#endif // !__DEFINE_H__
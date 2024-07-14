/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             这里是CAN中断接收函数，接收电机数据,CAN发送函数发送电机电流控制电机.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */
/*************原定义************/
#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "stm32f4xx.h"
#include "main.h"
#include "stdint.h"
#include "motor.h"
#include "gimbal.h"
#include "top.h"
#include "root.h"
#include "superCap.h"
#include "switch.h"

#define CHASSIS_CAN hcan2
#define GIMBAL_CAN hcan1

//#ifndef df_CAN
//		#define df_CAN
//		#define df_CAN1 &Can1RxMsg
//		#define df_CAN2 &Can2RxMsg
//#endif

#ifndef df_ID
	#define df_ID
	//四个底盘电机
	#define df_CHmotor_1_ID 0x201
	#define df_CHmotor_2_ID 0x202
	#define df_CHmotor_3_ID 0x203
	#define df_CHmotor_4_ID 0x204
	//陀螺仪
 	#define df_top_ID 0x101
	//发射机构
	#define df_RightFriction_ID 0x201
	#define df_LeftFriction_ID 0x202
	#define df_GiveAmmunitHight_ID 0x203
	//#define df_GiveAmmunitDown_ID 0x204
	//两个云台电机
	#define df_GMmotor_p_ID 0x209
	#define df_GMmotor_y_ID 0x20B 
	//云台主控发送的数据
	#define df_GMControl_ID 0x221		//云台主控板
	#define df_CHControlAngle_ID 0x231		//底盘主控板//该ID发送两个角度
//	#define df_CHControlData_ID 0x232	//各种标志位以及状态
	//超级电容
	#define df_SuperCap_ID 0x307 
#endif

#ifndef df_re_error                               
	#define df_re_ok 1
	#define df_re_error 0
#endif


#ifndef df_canBoll
	#define df_canBoll
	#define df_start	3
	#define df_ing	2
	#define df_end	1
#endif
/*************宏定义结束************/
/* CAN send and receive ID */
__packed struct canBoll_typedef
{
		__packed struct 
		{
			uint8_t RXboll :1;			//接收状态
			uint8_t RXMailBox :3;		//接收使用的邮箱
			uint8_t TXMailBox :3;		//发送使用的邮箱
			uint8_t RxResolver :3;	//接收过程状态
			uint8_t TxState :3;	//发送过程状态
		}my_can1;
		__packed struct 
		{
			uint8_t RXboll :1;
			uint8_t RXMailBox :3;
			uint8_t TXMailBox :3;
			uint8_t RxResolver :3;
			uint8_t TxState :3;
		}my_can2;
};
extern struct canBoll_typedef canBoll_t;

typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
} motor_measure_t;





/**
  * @brief          返回底盘电机 3508电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
extern const motor_measure_t *get_chassis_motor_measure_point(uint8_t i);

extern struct canBoll_typedef canBoll_t;
////CAN1初始化函数
//void my_can1_Init(uint8_t tsjw, uint8_t tbs1, uint8_t tbs2, uint16_t brp, uint8_t mode);
////CAN2初始化函数
//void my_can2_Init(uint8_t tsjw, uint8_t tbs1, uint8_t tbs2, uint16_t brp, uint8_t mode);
//CAN发送函数
uint8_t my_CAN_Send(CAN_HandleTypeDef *hcan, uint32_t StdID, int16_t num1, int16_t num2, int16_t num3, int16_t num4, uint16_t len);
//Can 数据接收函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
//云台给底盘的发送函数
uint8_t CANGimbalTX(CAN_HandleTypeDef *hcan);       
//底盘给云台的发送函数
uint8_t CANSendCHtoGM(CAN_HandleTypeDef *hcan);
extern uint8_t rx_data[8];//can接收数据暂存
extern uint8_t can_send_data[8];
extern uint8_t can_union_txdata[8];

#endif

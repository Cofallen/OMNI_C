/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             ������CAN�жϽ��պ��������յ������,CAN���ͺ������͵���������Ƶ��.
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
/*************ԭ����************/
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
	//�ĸ����̵��
	#define df_CHmotor_1_ID 0x201
	#define df_CHmotor_2_ID 0x202
	#define df_CHmotor_3_ID 0x203
	#define df_CHmotor_4_ID 0x204
	//������
 	#define df_top_ID 0x101
	//�������
	#define df_RightFriction_ID 0x201
	#define df_LeftFriction_ID 0x202
	#define df_GiveAmmunitHight_ID 0x203
	//#define df_GiveAmmunitDown_ID 0x204
	//������̨���
	#define df_GMmotor_p_ID 0x209
	#define df_GMmotor_y_ID 0x20B 
	//��̨���ط��͵�����
	#define df_GMControl_ID 0x221		//��̨���ذ�
	#define df_CHControlAngle_ID 0x231		//�������ذ�//��ID���������Ƕ�
//	#define df_CHControlData_ID 0x232	//���ֱ�־λ�Լ�״̬
	//��������
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
/*************�궨�����************/
/* CAN send and receive ID */
__packed struct canBoll_typedef
{
		__packed struct 
		{
			uint8_t RXboll :1;			//����״̬
			uint8_t RXMailBox :3;		//����ʹ�õ�����
			uint8_t TXMailBox :3;		//����ʹ�õ�����
			uint8_t RxResolver :3;	//���չ���״̬
			uint8_t TxState :3;	//���͹���״̬
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
  * @brief          ���ص��̵�� 3508�������ָ��
  * @param[in]      i: ������,��Χ[0,3]
  * @retval         �������ָ��
  */
extern const motor_measure_t *get_chassis_motor_measure_point(uint8_t i);

extern struct canBoll_typedef canBoll_t;
////CAN1��ʼ������
//void my_can1_Init(uint8_t tsjw, uint8_t tbs1, uint8_t tbs2, uint16_t brp, uint8_t mode);
////CAN2��ʼ������
//void my_can2_Init(uint8_t tsjw, uint8_t tbs1, uint8_t tbs2, uint16_t brp, uint8_t mode);
//CAN���ͺ���
uint8_t my_CAN_Send(CAN_HandleTypeDef *hcan, uint32_t StdID, int16_t num1, int16_t num2, int16_t num3, int16_t num4, uint16_t len);
//Can ���ݽ��պ���
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
//��̨�����̵ķ��ͺ���
uint8_t CANGimbalTX(CAN_HandleTypeDef *hcan);       
//���̸���̨�ķ��ͺ���
uint8_t CANSendCHtoGM(CAN_HandleTypeDef *hcan);
extern uint8_t rx_data[8];//can���������ݴ�
extern uint8_t can_send_data[8];
extern uint8_t can_union_txdata[8];

#endif

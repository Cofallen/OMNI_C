#include "main.h"
#include "can.h"

#include "CAN_DEV.h"
#include "TIM_DEV.h"
#include "MOTOR.h"
#include "DEFINE.h"
#include "DBUS.h"
#include "TOP.h"
#include "CAP.h"

// 盛放原始can数据
uint8_t CANRxmsg[8] = {0};
CAN_RxHeaderTypeDef CANRxheader;

// CAN中断回调
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CANRxheader, CANRxmsg);
    CAN_F_Recv(hcan, CANRxheader.StdId);
}

/**
 * @brief CAN接收数据解算
 *
 * @param hcan CANx
 * @param ID 标识符
 */
void CAN_F_Recv(CAN_HandleTypeDef *hcan, uint32_t ID)
{
    if (hcan->Instance == CAN1)
    {
        switch (ID)
        {
        // 地盘
        case CAN_D_CHASSIS_1:
            MOTOR_F_Cal_Data(&MOTOR_V_CHASSIS[0], CANRxmsg);
            break;
        case CAN_D_CHASSIS_2:
            MOTOR_F_Cal_Data(&MOTOR_V_CHASSIS[1], CANRxmsg);
            break;
        case CAN_D_CHASSIS_3:
            MOTOR_F_Cal_Data(&MOTOR_V_CHASSIS[2], CANRxmsg);
            break;
        case CAN_D_CHASSIS_4:
            MOTOR_F_Cal_Data(&MOTOR_V_CHASSIS[3], CANRxmsg);
            break;
				 case 0x240:
            {
                // 电容
                CanManage_cap(CANRxmsg, &capData_t.capGetDate);
                break;
            }
        default:
            break;
        }
    }
    if (hcan->Instance == CAN2)
    {
        switch (ID)
        {
        // 云台
        case CAN_D_GIMBAL_YAW:
        {
            MOTOR_F_Cal_Data(&MOTOR_V_GIMBAL[0], CANRxmsg);		
            MOTOR_F_Cal_Round(&MOTOR_V_GIMBAL[0]);
            break;
        }
        case CAN_D_GIMBAL_PIT:
        {
            MOTOR_F_Cal_Data(&MOTOR_V_GIMBAL[1], CANRxmsg);
            MOTOR_F_Cal_Round(&MOTOR_V_GIMBAL[1]);
            break;
        }
        case CAN_D_ATTACK_L:
            MOTOR_F_Cal_Data(&MOTOR_V_ATTACK[0], CANRxmsg);
            break;
        case CAN_D_ATTACK_R:
            MOTOR_F_Cal_Data(&MOTOR_V_ATTACK[1], CANRxmsg);
            break;
        case CAN_D_ATTACK_G:
            MOTOR_F_Cal_Data(&MOTOR_V_ATTACK[2], CANRxmsg);
            MOTOR_F_Cal_Round(&MOTOR_V_ATTACK[2]);
            break;
        default:
            break;
        }
    }
}

/**
 * @brief CAN发送函数
 *
 * @param _hcan CANx &hcanX
 * @param stdid 标识符 CAN_ID_XX
 * @param num1
 * @param num2
 * @param num3
 * @param num4
 * @return void
 */
void CAN_F_Send(CAN_HandleTypeDef *hcan, int16_t stdid, int16_t num1, int16_t num2, int16_t num3, int16_t num4)
{
    CAN_TxHeaderTypeDef CANTxmsg;
    uint8_t Data[8];
    uint32_t Mailbox = 0;

    CANTxmsg.StdId = stdid;
    CANTxmsg.ExtId = 0x000; // 0-0x1fffffff
    CANTxmsg.DLC = 0x08;
    CANTxmsg.IDE = CAN_ID_STD;
    CANTxmsg.RTR = CAN_RTR_DATA;
    Data[0] = ((num1) >> 8);
    Data[1] = (num1);
    Data[2] = ((num2) >> 8);
    Data[3] = (num2);
    Data[4] = ((num3) >> 8);
    Data[5] = (num3);
    Data[6] = ((num4) >> 8);
    Data[7] = (num4);

    HAL_CAN_AddTxMessage(hcan, &CANTxmsg, Data, &Mailbox);
}

/**
 * @brief CAN滤波器初始化
 *
 * @param hcan CANx
 */
void CAN_Filter_Init(void)
{
    CAN_FilterTypeDef CAN_FilterInitStructure;
    CAN_FilterInitStructure.FilterActivation = ENABLE;
    CAN_FilterInitStructure.FilterFIFOAssignment = CAN_RX_FIFO0; // 过滤器0关联到FIFO0
    CAN_FilterInitStructure.FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.FilterIdLow = 0x0000;
    CAN_FilterInitStructure.FilterMaskIdHigh = 0x0000;           // 过滤掩码
    CAN_FilterInitStructure.FilterMaskIdLow = 0x0000;            //
    CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;  // 掩码模式
    CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT; // 32位
    CAN_FilterInitStructure.SlaveStartFilterBank = 14;           // 从属过滤器组
    CAN_FilterInitStructure.FilterBank = 0;                      // 过滤器组0

    if (HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure) != HAL_OK)
    {
        /* Filter configuration Error */
        Error_Handler();
    }
    /* Start the CAN peripheral */
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        /* Start Error */
        Error_Handler();
    }
    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        /* Notification Error */
        Error_Handler();
    }
    /* Activate CAN TX notification */
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
        /* Notification Error */
        Error_Handler();
    }

    CAN_FilterInitStructure.SlaveStartFilterBank = 14;
    CAN_FilterInitStructure.FilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterInitStructure) != HAL_OK)
    {
        /* Filter configuration Error */
        Error_Handler();
    }
    /* Start the CAN peripheral */
    if (HAL_CAN_Start(&hcan2) != HAL_OK)
    {
        /* Start Error */
        Error_Handler();
    }
    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        /* Notification Error */
        Error_Handler();
    }
    /* Activate CAN TX notification */
    if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
        /* Notification Error */
        Error_Handler();
    }
}

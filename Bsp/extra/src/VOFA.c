#include "main.h"
#include <string.h>
#include "usart.h"
#include <stdarg.h>

#include "VOFA.h"
#include "MOTOR.h"
#include "DEFINE.h"
#include "stdlib.h"
#include "CHASSIS.h"
#include "TIM_DEV.h"
#include "Read_Data.h"
#include "VISION.h"
#include "TOP.h"
#include "robot.h"
#include "YU_PID.h"
#include "ROOT.h"
#include "usbd_cdc_if.h"
#include "ATTACK.h"

union 
{
    float DATA[11];
    uint8_t TAIL[44];
}VOFA_T_DATA={0};

union 
{
    float DATA[11];
    uint8_t TAIL[44];
}data_temp={0};

// 发送VOFA数据包
void VOFA_F_Send(TYPEDEF_VOFA_UNION *VOFA, TYPEDEF_MOTOR *MOTOR)
{
    VOFA->DATA.MOTOR.MOTOR_DATA.ANGLE = MOTOR->DATA.ANGLE_NOW;
    VOFA->DATA.MOTOR.MOTOR_DATA.SPEED = MOTOR->DATA.SPEED_NOW;
    VOFA->DATA.MOTOR.MOTOR_DATA.CURRENT = MOTOR->DATA.CURRENT;
    VOFA->DATA.MOTOR.MOTOR_DATA.ANGLE_INFINITE = MOTOR->DATA.ANGLE_INFINITE;
    VOFA->DATA.MOTOR.MOTOR_DATA.LAPS = MOTOR->DATA.LAPS;
    VOFA->DATA.MOTOR.MOTOR_DATA.AIM = MOTOR->DATA.AIM;

    memcpy(&VOFA->DATA.MOTOR.PID_A, &MOTOR->PID_A, sizeof(TYPEDEF_MOTOR_PID));
    memcpy(&VOFA->DATA.MOTOR.PID_S, &MOTOR->PID_S, sizeof(TYPEDEF_MOTOR_PID));
    memcpy(&VOFA->DATA.MOTOR.PID_C, &MOTOR->PID_C, sizeof(TYPEDEF_MOTOR_PID));

    VOFA->DATA.TAIL[0] = 0x00;
    VOFA->DATA.TAIL[1] = 0x00;
    VOFA->DATA.TAIL[2] = 0x80;
    VOFA->DATA.TAIL[3] = 0x7F;

    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)VOFA->ALL, sizeof(VOFA->ALL));
}

// VOFA 使用 huart1 发送任意个数数据
// 先使用定长数组接收 @TODO 使用malloc分配内存，释放内存
//                  @update 暂时完成
uint8_t VOFA_T_Send(uint8_t type, int n, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10)
{
    if (n > 10 || n < 0) return ROOT_ERROR;

    data_temp.DATA[0] = a1;
    data_temp.DATA[1] = a2;
    data_temp.DATA[2] = a3;
    data_temp.DATA[3] = a4;
    data_temp.DATA[4] = a5;
    data_temp.DATA[5] = a6;
    data_temp.DATA[6] = a7;
    data_temp.DATA[7] = a8;
    data_temp.DATA[8] = a9;
    data_temp.DATA[9] = a10;

    if (type == 0)
        CDC_Transmit_FS(data_temp.TAIL, 44); // 发送到虚拟串口
    else if (type == 1)
        HAL_UART_Transmit_DMA(&huart1, data_temp.TAIL, 44);
    
    return ROOT_READY;
}

/// @brief 
/// @param n 
/// @param type 类型 0:虚拟串口 1:USART1
/// @param  
/// @return 
uint8_t VOFA_T_SendTemp(uint8_t type, int n, ...)
{
    if (n > 10 || n < 0) return ROOT_ERROR;    
    va_list list;
    va_start(list, n);
    
    for (int i = 0; i < n; i++)
    {
        VOFA_T_DATA.DATA[i] = (float)va_arg(list, double);
    }
    va_end(list);

    for (int i = n; i < 10; i++)
    {
        VOFA_T_DATA.DATA[i] = 0.0f;
    }
    VOFA_T_DATA.TAIL[40] = 0x00;
    VOFA_T_DATA.TAIL[41] = 0x00;
    VOFA_T_DATA.TAIL[42] = 0x80;
    VOFA_T_DATA.TAIL[43] = 0x7f;

    if (type == 0)
        CDC_Transmit_FS((uint8_t *)VOFA_T_DATA.TAIL, sizeof(VOFA_T_DATA));
    else if (type == 1)
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)VOFA_T_DATA.TAIL, sizeof(VOFA_T_DATA));

    return ROOT_READY;
}


void VOFA_T_Vision()
{
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)sd_v_buff, sizeof(sd_v_buff));
}



// // niming function
// uint8_t buffer[26] = {0};
// void niming(int16_t id, int16_t x1, int16_t x2, int16_t x3, int16_t y1, int16_t y2, int16_t y3, int16_t z1, int16_t z2, int16_t z3, int16_t w)
// {
// //    if(id>0xfa || id<0xf1)
// //        return;

    
//     uint8_t addCheck=0,sumCheck=0;

//     memset(buffer, 0, sizeof(buffer));          //stdlib.h
//     sumCheck=0,addCheck=0;

//     buffer[0]=0xaa;         
//     buffer[1]=0xff;         
//     buffer[2]=id;         

//     buffer[4]=(x1 & 0x00ff);
//     buffer[5]=((x1>>8) & 0x00ff);

//     buffer[6]=(x2 & 0x00ff);
//     buffer[7]=((x2>>8) & 0x00ff);
    
//     buffer[8]=(x3 & 0x00ff);
//     buffer[9]=((x3>>8) & 0x00ff);

//     buffer[10]=(y1 & 0x00ff);
//     buffer[11]=((y1>>8) & 0x00ff);

//     buffer[12]=(y2 & 0x00ff);
//     buffer[13]=((y2>>8) & 0x00ff);
    
//     buffer[14]=(y3 & 0x00ff);
//     buffer[15]=((y3>>8) & 0x00ff);

//     buffer[16]=(z1 & 0x00ff);
//     buffer[17]=((z1>>8) & 0x00ff);

//     buffer[18]=(z2 & 0x00ff);
//     buffer[19]=((z2>>8) & 0x00ff);
    
//     buffer[20]=(z3 & 0x00ff);
//     buffer[21]=((z3>>8) & 0x00ff);

//     buffer[22]=(w & 0x00ff);
//     buffer[23]=((w>>8) & 0x00ff);

//     buffer[3]=20;

//     for(int i=0;i<buffer[3]+4;i++)
//     {
//         sumCheck+=buffer[i];
//         addCheck+=sumCheck;
//     }

//     buffer[24]=sumCheck & 0x000000ff;
//     buffer[25]=addCheck & 0x000000ff;

// 	HAL_UART_Transmit_DMA(&huart1,buffer,sizeof(buffer));
// }


uint8_t buffer[26] = {0};
void niming(int16_t id, int16_t x1, int16_t x2, int16_t x3, int16_t y1, int16_t y2, int16_t y3, int16_t z1, int16_t z2, int16_t z3, int16_t w)
{
//    if(id>0xfa || id<0xf1)
//        return;

    
    uint8_t addCheck=0,sumCheck=0;

    memset(buffer, 0, sizeof(buffer));          //stdlib.h
    sumCheck=0,addCheck=0;

    buffer[0]=0xaa;         
    buffer[1]=0xff;         
    buffer[2]=id;         

    buffer[4]=(x1 & 0x00ff);
    buffer[5]=((x1>>8) & 0x00ff);

    buffer[6]=(x2 & 0x00ff);
    buffer[7]=((x2>>8) & 0x00ff);
    
    buffer[8]=(x3 & 0x00ff);
    buffer[9]=((x3>>8) & 0x00ff);

    buffer[10]=(y1 & 0x00ff);
    buffer[11]=((y1>>8) & 0x00ff);

    buffer[12]=(y2 & 0x00ff);
    buffer[13]=((y2>>8) & 0x00ff);
    
    buffer[14]=(y3 & 0x00ff);
    buffer[15]=((y3>>8) & 0x00ff);

    buffer[16]=(z1 & 0x00ff);
    buffer[17]=((z1>>8) & 0x00ff);

    buffer[18]=(z2 & 0x00ff);
    buffer[19]=((z2>>8) & 0x00ff);
    
    buffer[20]=(z3 & 0x00ff);
    buffer[21]=((z3>>8) & 0x00ff);

    buffer[22]=(w & 0x00ff);
    buffer[23]=((w>>8) & 0x00ff);

    buffer[3]=20;

    for(int i=0;i<buffer[3]+4;i++)
    {
        sumCheck+=buffer[i];
        addCheck+=sumCheck;
    }

    buffer[24]=sumCheck & 0x000000ff;
    buffer[25]=addCheck & 0x000000ff;

	HAL_UART_Transmit_DMA(&huart1,buffer,sizeof(buffer));
}

/// @brief 
/// @param mod 0 飞破 1 云台pid 2 飞坡电流 3 发射 4 视觉 5 飞坡匿名
/// @param mod 
void Vofa_intergrate(uint8_t mod) 
{
    switch (mod)
    {
    case 0:
        VOFA_T_SendTemp(0, 10, 0.0f,
                watch[0], watch[1], watch[2], watch[3], watch[4], watch[5], 
                watch[6], watch[7], watch[8]);
        break;
    case 1:
        VOFA_T_SendTemp(0, 10, 1.0f,
                (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.AIM,
                (float)TOP.roll[5],
                (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.AIM,
				(float)TOP.yaw[3],
                (float)DBUS_V_DATA.IS_OFF,
                (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.CURRENT,
                current[0],
                (float)user_data.power_heat_data.buffer_energy,
				(float)user_data.robot_status.chassis_power_limit);
        break;
    case 2:
        VOFA_T_SendTemp(0, 10, 0.0f,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CURRENT,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CURRENT,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CURRENT,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CURRENT,
                (float)DBUS_V_DATA.is_front_lifted,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.AIM,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.AIM,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.AIM,
                (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.AIM);
        break;
    case 3:
        VOFA_T_SendTemp(0, 10, 0.0f,  // debug yaw pid with top[3]
                (float)user_data.shoot_data.initial_speed,
                (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW,
                (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.SPEED_NOW,
                (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.AIM,
                (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.AIM,
                (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE,
                (float)ATTACK_V_PARAM.COUNT,
                (float)ATTACK_V_PARAM.TIME,
                1.0f);
        break;
    case 4:
        VOFA_T_SendTemp(0, 10, 0.0f,
            (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.AIM,
            (float)vision_aim,
            (float)VISION_V_DATA.RECEIVE.fire,
            (float)VISION_PID_YAW_ANGLE.OUT.ERROR[NOW],
            (float)VISION_PID_PIT_ANGLE.OUT.ERROR[NOW],
            (float)TOP.yaw[5],
            (float)TOP.roll[5],
            (float)(dt_pc),
            99.0f);
        break;
    case 5:
        niming(0xF1, 
            (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CURRENT,
            (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CURRENT,
            (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CURRENT,
            (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CURRENT, // y1
            (int16_t)DBUS_V_DATA.is_front_lifted,                     // y2
            0,                                                       // y3
            0, 0, 0,                                                // z1-z3
            0);  
        break;
    case 6:
        niming(0xF1, 
            (int16_t)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.AIM,
            (int16_t)TOP.yaw[3], 
            0, 0, 0, 0,                        // y3
            0, 0, 0,                                              // z1-z3
            0);  
        break;
    case 7:
        VOFA_T_Send(0, 10, 1.0f,
            (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.AIM,
            (float)TOP.roll[5],
            (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.AIM,
            (float)TOP.yaw[3],
            (float)DBUS_V_DATA.IS_OFF,
            (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.CURRENT,
            current[0],
            (float)user_data.power_heat_data.buffer_energy,
            (float)user_data.robot_status.chassis_power_limit);
        break;
    default:
        break;
    }
}
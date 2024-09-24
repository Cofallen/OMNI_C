#include "main.h"

#include "MOTOR.h"

/**
 * @brief 解算电机反馈来的数据
 *
 * @param MOTOR 电机结构体指针
 * @param RxMessage CAN报文
 */
void MOTOR_F_Cal_Data(TYPEDEF_MOTOR *MOTOR, uint8_t *RxMessage)
{
    MOTOR->DATA.ANGLE_LAST = MOTOR->DATA.ANGLE_NOW;
    MOTOR->DATA.SPEED_LAST = MOTOR->DATA.SPEED_NOW;

    MOTOR->DATA.ANGLE_NOW = ((uint16_t)RxMessage[0] << 8 | (uint16_t)RxMessage[1]);
    MOTOR->DATA.SPEED_NOW = ((uint16_t)RxMessage[2] << 8 | (uint16_t)RxMessage[3]);
    MOTOR->DATA.CURRENT = ((uint16_t)RxMessage[4] << 8 | (uint16_t)RxMessage[5]);
    MOTOR->DATA.TEMP = RxMessage[6];
}

/**
 * @brief 根据电机编码器值计算运转圈数
 *
 * @param MOTOR 电机结构体指针
 */
void MOTOR_F_Cal_Round(TYPEDEF_MOTOR *MOTOR)
{
    if (MOTOR->DATA.ANGLE_NOW - MOTOR->DATA.ANGLE_LAST < -4096)
    {
        MOTOR->DATA.LAPS++;
    }
    if (MOTOR->DATA.ANGLE_NOW - MOTOR->DATA.ANGLE_LAST > 4096)
    {
        MOTOR->DATA.LAPS--;
    }
    MOTOR->DATA.ANGLE_INFINITE = (int32_t)((int)((MOTOR->DATA.LAPS) << 13) + MOTOR->DATA.ANGLE_NOW);
}

#include "main.h"
#include <string.h>
#include <stdio.h>

#include "YU_PID.h"
#include "MOTOR.h"
#include "DEFINE.h"
#include "YU_MATH.h"
#include "TOP.h"
#include "ROOT.h"
#include "VISION.h"
/**
 * @brief               自己写的PID初始化，不用读配置文件
 * @details             传入PID基本参数， 通过数组
 * @param PID       :   PID 指针，  形式 : MOTOR->PID
 * @param PID_V     :   5个float, 参数和限幅
 * @date                2024-03-12
 * @author              YU
 */
void PID_F_Init(TYPEDEF_MOTOR_PID *PID, const float *PID_V)
{
    if (PID == NULL || PID_V == NULL)
    {
        return;
    }

    PID->IN.KP = PID_V[0];
    PID->IN.KI = PID_V[1];
    PID->IN.KD = PID_V[2];
    PID->IN.I_LIT = PID_V[3];
    PID->IN.ALL_LIT = PID_V[4];

    PID->OUT.ERROR[0] = 0.0f;
    PID->OUT.ERROR[1] = 0.0f;
}

/**
 * @brief               PID 解算
 * @details             PID 解算
 * @param PID       :   PID 指针，  形式 : MOTOR->PID
 * @param TARGET    :   目标值
 * @param REALVAL   :   实际值
 * @return
 */
float PID_F_Cal(TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL)
{
    //    printf("PID_CAL\n");
    if (PID == NULL)
    {
        return 0.0f;
    }

    PID->OUT.ERROR[NOW] = TARGET - REALVAL;

    PID->OUT.P_OUT = PID->IN.KP * PID->OUT.ERROR[NOW];
    PID->OUT.I_OUT += PID->IN.KI * PID->OUT.ERROR[NOW];
    PID->OUT.I_OUT = MATH_D_LIMIT(PID->IN.I_LIT, -PID->IN.I_LIT, PID->OUT.I_OUT);

    PID->OUT.D_OUT = PID->IN.KD * (PID->OUT.ERROR[NOW] - PID->OUT.ERROR[LAST]);
    PID->OUT.ALL_OUT = PID->OUT.P_OUT + PID->OUT.I_OUT + PID->OUT.D_OUT;

    PID->OUT.ALL_OUT = MATH_D_LIMIT(PID->IN.ALL_LIT, -PID->IN.ALL_LIT, PID->OUT.ALL_OUT);

    PID->OUT.ERROR[LAST] = PID->OUT.ERROR[NOW];

    return PID->OUT.ALL_OUT;
}

void PID_F_Clear(TYPEDEF_MOTOR *MOTOR)
{
    memset(&MOTOR->DATA, 0, sizeof(MOTOR->DATA));
    memset(&MOTOR->PID_A, 0, sizeof(MOTOR->PID_A));
    memset(&MOTOR->PID_S, 0, sizeof(MOTOR->PID_S));
    memset(&MOTOR->PID_C, 0, sizeof(MOTOR->PID_C));
}

// 角度速度双环pid
uint8_t PID_F_AS(TYPEDEF_MOTOR *MOTOR)
{
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_A, MOTOR->DATA.AIM, (float)MOTOR->DATA.ANGLE_INFINITE);
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&MOTOR->PID_S, MOTOR->PID_A.OUT.ALL_OUT, (float)MOTOR->DATA.SPEED_NOW);
    return ROOT_READY;
}

// 速度电流双环pid
uint8_t PID_F_SC(TYPEDEF_MOTOR *MOTOR)
{
    MOTOR->PID_S.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_S, MOTOR->DATA.AIM, MOTOR->DATA.SPEED_NOW);
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&MOTOR->PID_C, MOTOR->PID_S.OUT.ALL_OUT, MOTOR->DATA.CURRENT);
    return ROOT_READY;
}

// gimbal yaw 双环pid
uint8_t PID_F_G(TYPEDEF_MOTOR *MOTOR)
{         
    if (TOP.yaw[4] == 1.0f) // online
    {
        MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_A, MOTOR->DATA.AIM, TOP.yaw[3]);
        MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&MOTOR->PID_S, MOTOR->PID_A.OUT.ALL_OUT, ((float)QEKF_INS.Gyro[2] * 50.0f));
    }
    else if (TOP.yaw[4] == 0.0f) // offline 
    {
        MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&TOP_OFF_A, MOTOR->DATA.AIM, TOP.yaw[3]);
        MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&TOP_OFF_S, MOTOR->PID_A.OUT.ALL_OUT, MOTOR->DATA.SPEED_NOW);
    }
    return ROOT_READY;
}

double m = 0;
// gimbal pitch 双环pid
uint8_t PID_F_P(TYPEDEF_MOTOR *MOTOR)
{
	//PITCH引入陀螺仪
    m = (TOP.pitch[5] * 22.75555f);
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_A, MOTOR->DATA.AIM, spinLittleRound(&m));
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&MOTOR->PID_S, MOTOR->PID_A.OUT.ALL_OUT, (float)QEKF_INS.Gyro[1] * 10.0f);
    return ROOT_READY;
}

uint8_t PID_F_S(TYPEDEF_MOTOR *MOTOR)
{
	MOTOR->PID_S.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_S, MOTOR->DATA.AIM, MOTOR->DATA.SPEED_NOW);
	MOTOR->DATA.CAN_SEND = (int16_t)(MOTOR->PID_S.OUT.ALL_OUT);
	return ROOT_READY;
}

uint8_t PID_F_VISION_YAW(TYPEDEF_MOTOR *MOTOR)
{
    MOTOR->DATA.AIM = -(VISION_V_DATA.RECEIVE.YAW_DATA ); // 
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&VISION_PID_YAW_ANGLE, MOTOR->DATA.AIM, (TOP.yaw[5] ));
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&VISION_PID_YAW_SPEED, MOTOR->PID_A.OUT.ALL_OUT, ((float)QEKF_INS.Gyro[2] * 50.0f));
    return ROOT_READY;
}

uint8_t PID_F_VISION_PIT(TYPEDEF_MOTOR *MOTOR)
{
	  MOTOR->DATA.AIM = - VISION_V_DATA.RECEIVE.PIT_DATA;
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&VISION_PID_PIT_ANGLE, MOTOR->DATA.AIM, (float)TOP.pitch[1]);
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&VISION_PID_PIT_SPEED, MOTOR->PID_A.OUT.ALL_OUT, (float)QEKF_INS.Gyro[0] * 50.0f);
    return ROOT_READY;
}


uint8_t PID_F_P_T(TYPEDEF_MOTOR *MOTOR)
{
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_A, MOTOR->DATA.AIM, (float)MOTOR->DATA.ANGLE_NOW);
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&MOTOR->PID_S, MOTOR->PID_A.OUT.ALL_OUT, (float)MOTOR->DATA.SPEED_NOW);
    return ROOT_READY;
}

uint8_t PID_T_G(TYPEDEF_MOTOR *MOTOR) // 我写的视觉yaw
{   
    MOTOR->DATA.AIM = (VISION_V_DATA.RECEIVE.YAW_DATA * 22.75555f); // 

    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&VISION_PID_YAW_ANGLE, MOTOR->DATA.AIM, (TOP.yaw[5] * 22.75555f));
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&VISION_PID_YAW_SPEED, MOTOR->PID_A.OUT.ALL_OUT, ((float)QEKF_INS.Gyro[2] * 50.0f));
    return ROOT_READY;
}

/// @brief enhanced PID
/// @param mode 1: trapezoidal integration
///             2: anti windup
///             3: integration speration
///             4: changing rate
/// @return 
/// @TODO add the D part
float PID_T_Cal(TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL, int8_t mode)
{
    if (PID == NULL)
    {
        return 0.0f;
    }

    PID->OUT.ERROR[NOW] = TARGET - REALVAL;

    PID->OUT.P_OUT = PID->IN.KP * PID->OUT.ERROR[NOW];

    switch (mode)
    {
        case 0: // bumpless opration
        {
            PID->OUT.I_OUT += PID->IN.KI * PID->OUT.ERROR[NOW];
            PID->OUT.I_OUT = MATH_D_LIMIT(PID->IN.I_LIT, -PID->IN.I_LIT, PID->OUT.I_OUT);
            break;
        }
        case 1: // trapezoidal integration
        {
            PID->OUT.I_OUT += PID->IN.KI * (PID->OUT.ERROR[NOW] + PID->OUT.ERROR[LAST]) * 0.5f;
            PID->OUT.I_OUT =  MATH_D_LIMIT(PID->IN.I_LIT, -PID->IN.I_LIT, PID->OUT.I_OUT);
            break;
        }
        case 2: // anti windup
        {
            float alpha = 0.0f; // whether to accumulate the error
            // the i_lit can be set to another vaule, and is different from origin
            if ((MATH_D_ABS(PID->OUT.I_OUT) > PID->IN.I_LIT) && (PID->OUT.I_OUT * PID->OUT.ERROR[NOW] > 0))
            {
                alpha = 0.0f;
            } else
            {
                alpha = 1.0f;
            }
            PID->OUT.I_OUT += alpha * PID->IN.KI * PID->OUT.ERROR[NOW];
            break;
        }
        case 3: // integration speration (using intergration limit)
        {
            float edge = 500.0f; // the edge of the error
            if (MATH_D_ABS(PID->OUT.ERROR[NOW]) < edge)
            {
                PID->OUT.I_OUT += PID->IN.KI * PID->OUT.ERROR[NOW];
                PID->OUT.I_OUT = MATH_D_LIMIT(PID->IN.I_LIT, -PID->IN.I_LIT, PID->OUT.I_OUT);
            }
            break;
        }
        case 4: // changing rate (using intergration limit)
        {
            // consider f(e(k)) is a linear ralation with e(k)
            // 0 <= a < b
            float a = 0.0f, b = 0.0f, f = 0.0f;
            if (MATH_D_ABS(PID->OUT.ERROR[NOW]) <= a)
            {
                f = 1.0f;
            } else if (MATH_D_ABS(PID->OUT.ERROR[NOW]) > b)
            {
                f = 0.0f;
            } else if (MATH_D_ABS(PID->OUT.ERROR[NOW]) > a && MATH_D_ABS(PID->OUT.ERROR[NOW]) <= b)
            {
                f = (b - (MATH_D_ABS(PID->OUT.ERROR[NOW]))) / (b - a);
            }
            PID->OUT.I_OUT += f * PID->IN.KI * PID->OUT.ERROR[NOW];
            PID->OUT.I_OUT = MATH_D_LIMIT(PID->IN.I_LIT, -PID->IN.I_LIT, PID->OUT.I_OUT);
            break;
        }
        default:
            break;
    }
   
    // @TODO add the D part
    PID->OUT.D_OUT = PID->IN.KD * (PID->OUT.ERROR[NOW] - PID->OUT.ERROR[LAST]);
    PID->OUT.ALL_OUT = PID->OUT.P_OUT + PID->OUT.I_OUT + PID->OUT.D_OUT;

    PID->OUT.ALL_OUT = MATH_D_LIMIT(PID->IN.ALL_LIT, -PID->IN.ALL_LIT, PID->OUT.ALL_OUT);

    PID->OUT.ERROR[LAST] = PID->OUT.ERROR[NOW];

    return PID->OUT.ALL_OUT;
}

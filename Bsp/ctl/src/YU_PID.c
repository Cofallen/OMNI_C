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
#include "bsp_dwt.h"
float vision_aim = 0;
float current[2] = {0.0f, 0.0f};

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
        // MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_A, MOTOR->DATA.AIM, TOP.yaw[3]);
        // MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&MOTOR->PID_S, MOTOR->PID_A.OUT.ALL_OUT, ((float)QEKF_INS.Gyro[2] * 50.0f));
        // Feedforward_Calculate(&MOTOR->PID_F, MOTOR->DATA.AIM);
        current[NOW] = (float)MOTOR->DATA.CURRENT;
        MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_A, MOTOR->DATA.AIM, TOP.yaw[3]);
        MOTOR->PID_S.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_S, MOTOR->PID_A.OUT.ALL_OUT, ((float)QEKF_INS.Gyro[2] * 50.0f));
        MOTOR->DATA.CAN_SEND     = (int16_t)PID_F_Cal(&MOTOR->PID_C, MOTOR->PID_S.OUT.ALL_OUT, YU_MATH_LowPassFilter(0.06f, current));
    }
    else if (TOP.yaw[4] == 0.0f) // offline 
    {
        // MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&TOP_OFF_A, MOTOR->DATA.AIM, TOP.yaw[3]);
        MOTOR->DATA.CAN_SEND = 0;
    }
    return ROOT_READY;
}

// 调节电流环pid
void PID_F_Current(TYPEDEF_MOTOR *MOTOR, float aim_currnet)
{
	MOTOR->PID_C.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_C, aim_currnet, MOTOR->DATA.CURRENT);
    MOTOR->DATA.CAN_SEND     = (int16_t)MOTOR->PID_C.OUT.ALL_OUT ;
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
    vision_aim = (VISION_V_DATA.RECEIVE.YAW_DATA); // 
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&VISION_PID_YAW_ANGLE, vision_aim, (TOP.yaw[5]));
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&VISION_PID_YAW_SPEED, -MOTOR->PID_A.OUT.ALL_OUT, ((float)QEKF_INS.Gyro[2] * 50.0f));
    return ROOT_READY;
}

uint8_t PID_F_VISION_PIT(TYPEDEF_MOTOR *MOTOR)
{
	MOTOR->DATA.AIM = VISION_V_DATA.RECEIVE.PIT_DATA; //
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&VISION_PID_PIT_ANGLE, MOTOR->DATA.AIM, (TOP.roll[5]));
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&VISION_PID_PIT_SPEED, -MOTOR->PID_A.OUT.ALL_OUT, (float)QEKF_INS.Gyro[0] * 50.0f);
    return ROOT_READY;
}


uint8_t PID_F_P_T(TYPEDEF_MOTOR *MOTOR)
{
    MOTOR->PID_A.OUT.ALL_OUT = PID_F_Cal(&MOTOR->PID_A, MOTOR->DATA.AIM, TOP.roll[5]);
    MOTOR->DATA.CAN_SEND = (int16_t)PID_F_Cal(&MOTOR->PID_S, -MOTOR->PID_A.OUT.ALL_OUT, (float)QEKF_INS.Gyro[0] * 100.0f);
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


/*************************** FEEDFORWARD CONTROL *****************************/
/**
 * @brief          前馈控制初始化
 * @param[in]      前馈控制结构体
 * @param[in]      略
 * @retval         返回空
 */
void Feedforward_Init(
    Feedforward_t *ffc,
    float max_out,
    float *c,
    float lpf_rc,
    uint16_t ref_dot_ols_order,
    uint16_t ref_ddot_ols_order)
{
    ffc->MaxOut = max_out;

    // 设置前馈控制器参数 详见前馈控制结构体定义
    // set parameters of feed-forward controller (see struct definition)
    if (c != NULL && ffc != NULL)
    {
        ffc->c[0] = c[0];
        ffc->c[1] = c[1];
        ffc->c[2] = c[2];
    }
    else
    {
        ffc->c[0] = 0;
        ffc->c[1] = 0;
        ffc->c[2] = 0;
        ffc->MaxOut = 0;
    }

    //低通滤波参数
    ffc->LPF_RC = lpf_rc;

    // 最小二乘提取信号微分初始化
    // differential signal is distilled by OLS
    ffc->Ref_dot_OLS_Order = ref_dot_ols_order;
    ffc->Ref_ddot_OLS_Order = ref_ddot_ols_order;
    if (ref_dot_ols_order > 2)
        OLS_Init(&ffc->Ref_dot_OLS, ref_dot_ols_order);
    if (ref_ddot_ols_order > 2)
        OLS_Init(&ffc->Ref_ddot_OLS, ref_ddot_ols_order);

    ffc->DWT_CNT = 0;

    ffc->Output = 0;
}

/**
 * @brief          PID计算
 * @param[in]      PID结构体
 * @param[in]      测量值
 * @param[in]      期望值
 * @retval         返回空
 */
float Feedforward_Calculate(Feedforward_t *ffc, float ref)
{
	//求离散后的单位时间
    uint32_t tmp = ffc->DWT_CNT;
    ffc->dt = DWT_GetDeltaT(&tmp);
	//将期望值进行一阶低通滤波
    ffc->Ref = ref * ffc->dt / (ffc->LPF_RC + ffc->dt) +
               ffc->Ref * ffc->LPF_RC / (ffc->LPF_RC + ffc->dt);
    /*公式解析
    ffc->Ref = ref * ffc->dt / (ffc->LPF_RC + ffc->dt) + ffc->Ref * ffc->LPF_RC / (ffc->LPF_RC + ffc->dt);
             = ref * (1/(LPF_RC/ffc->dt + 1)) + ffc->Ref * (1/(ffc->dt/LPF_RC + 1))
             = ref * A + ffc->Ref * (1-A)
    A   = 1/(LPF_RC/ffc->dt + 1)
    1-A = 1/(ffc->dt/LPF_RC + 1)
    注：https://blog.csdn.net/qq_37662088/article/details/125075600
    */

    // 计算一阶导数
    // calculate first derivative
    if (ffc->Ref_dot_OLS_Order > 2)
        ffc->Ref_dot = OLS_Derivative(&ffc->Ref_dot_OLS, ffc->dt, ffc->Ref);
    else
        ffc->Ref_dot = (ffc->Ref - ffc->Last_Ref) / ffc->dt;
    // 计算二阶导数
    // calculate second derivative
    if (ffc->Ref_ddot_OLS_Order > 2)
        ffc->Ref_ddot = OLS_Derivative(&ffc->Ref_ddot_OLS, ffc->dt, ffc->Ref_dot);
    else
        ffc->Ref_ddot = (ffc->Ref_dot - ffc->Last_Ref_dot) / ffc->dt;
    // 计算前馈控制输出
    // calculate feed-forward controller output
    ffc->Output = ffc->c[0] * ffc->Ref + ffc->c[1] * ffc->Ref_dot + ffc->c[2] * ffc->Ref_ddot;

    ffc->Output = float_constrain(ffc->Output, -ffc->MaxOut, ffc->MaxOut);

    ffc->Last_Ref = ffc->Ref;
	
    ffc->Last_Ref_dot = ffc->Ref_dot;

    return ffc->Output;
}

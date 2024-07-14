#include "main.h"
#include "math.h"

#include "CHASSIS.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "YU_MATH.h"
#include "DBUS.h"
#include "PID.h"
#include "TIM_DEV.h"

#include "Read_Data.h"
#include "superCap.h"
#include "shu_math.h"

// 本代码为全向轮底盘运动基本代码

int16_t CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    int16_t powersum = 0;
    // 运动学解算
    float Vx = 0.0f, Vy = 0.0f, Vr = 0.0f, RATE = 1.0f, COMPONENT[2] = {0.5, 1};
    double ANGLE_Relative = 0.0f, ANGLE_Rad = 0.0f;
    double PRIDICT = 0.0f;    // 底盘预测，前馈
    Vx = (float)DBUS->REMOTE.CH0_int16 * RATE;
    Vy = (float)DBUS->REMOTE.CH1_int16 * RATE;
    // Vr = -(float)DBUS->REMOTE.CH2_int16;
    // Vr = 1000.0f; // spining mode

    // 底盘跟随模式
//    ANGLE_Relative = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW - (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INIT;
//    ANGLE_Rad = ANGLE_Relative * MATH_D_RELATIVE_PARAM;

//    double COS = cos(ANGLE_Rad);
//    double SIN = sin(ANGLE_Rad);
//    Vx = -Vy * SIN + Vx * COS;
//    Vy =  Vy * COS + Vx * SIN;

//    // 各方向分量限幅，TODO：待计算比例因子RATE
//    Vx = MATH_D_LIMIT(3000, -3000, Vx);
//    Vy = MATH_D_LIMIT(3000, -3000, Vy);
//    Vr = MATH_D_LIMIT(1000, -1000, Vr);

//    PRIDICT = MATH_D_LIMIT(200, -200, (DBUS->REMOTE.CH2_int16) * 0.3f);  // @TODO 预测模型待思考

    // 运动学解算
    MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = ( Vx + Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-Vx + Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-Vx - Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = ( Vx - Vy - Vr * COMPONENT[0]) * COMPONENT[1] + PRIDICT;

    // pid 解算
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_1]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_2]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_3]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_4]);
    for(int i = 0;i<4;i++)
    {
        powersum += MATH_D_ABS(MOTOR[i].DATA.CAN_SEND);
    }
    return powersum;
}

// The following code is for reference ZJU Power Control
// Reference Site: https://zju-helloworld.github.io/Wiki/
// fitting function :
// P = \omega * M + P_{loss}
//              M = k_{M} * i_{q}, consultation, k_{M} = 0.3 Nm/A
//                  P_{loss} = R * i^2
//                             R = p_{1} = 0.1249
//                                 p_{2} = -0.007836 -> 0
//                                 p_{3} = 5.952, circuit power
// @TODO using karman filter -> k 
// using function predict

struct CHASSIS_F_PwrLimit
{
    float Pwr;
    float Z;
    float k;
    float R;
    float PwrSum;
    float PwrPred[4];
}CHASSIS_V_PwrLimit;


void CHASSIS_F_PwrLimitInit()
{

}


void CHASSIS_F_PwrLimitCalc(TYPEDEF_MOTOR *MOTOR)
{
    CHASSIS_V_PwrLimit.Pwr = user_data.power_heat_data.chassis_power;
    CHASSIS_V_PwrLimit.Z   = user_data.power_heat_data.buffer_energy;

    CHASSIS_V_PwrLimit.R = 0.3f;
    CHASSIS_V_PwrLimit.k = 0.1249f;  // @TODO k need to calculate by karman filter
    CHASSIS_V_PwrLimit.PwrSum = 0.0f;

    // dimension transform
    float current[4] = {0.0f};
    for (int i = 0; i < 4; i++)
    {
        // dimension transform
        current[i] = MOTOR[i].DATA.CURRENT * 0.001220703125f;
        CHASSIS_V_PwrLimit.PwrPred[i] = 1.99688994e-6f * current[i] * MOTOR[i].DATA.SPEED_NOW //数字是电流力矩常数，从英雄代码里找的
                        + CHASSIS_V_PwrLimit.R * current[i] * current[i];
        if (CHASSIS_V_PwrLimit.PwrPred < 0)
            continue;
        CHASSIS_V_PwrLimit.PwrSum += CHASSIS_V_PwrLimit.PwrPred[i];
    }
    CHASSIS_V_PwrLimit.PwrSum += CHASSIS_V_PwrLimit.Pwr;

    // @TODO no consider the cap
    CHASSIS_V_PwrLimit.PwrSum -= CHASSIS_V_PwrLimit.Z;

    if (CHASSIS_V_PwrLimit.PwrSum > CHASSIS_V_PwrLimit.Pwr)
    {
        float scale = CHASSIS_V_PwrLimit.Pwr / CHASSIS_V_PwrLimit.PwrSum;
        for (int i = 0; i < 4; i++)
        {
            CHASSIS_V_PwrLimit.PwrPred[i] *= scale;
        }
    }

    // @TODO limit the power to motor
    
    
}

/************************************************************万能分隔符**************************************************************
*	@author:			//赵澍
*	@performance:	//将四个电机功率按照输出值按比分配
*	@parameter:		//被限制之后的输出值
*	@time:				//22-03-20
*	@ReadMe:			//
************************************************************************************************************************************/
void AllotPower(int32_t setOut)
{
		//得到未限制之前的总输出
	float allOutPower = abs_float(MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].PID_S.OUT.ALL_OUT + abs_float(MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].PID_S.OUT.ALL_OUT) + \
		abs_float(MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].PID_S.OUT.ALL_OUT) + abs_float(MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].PID_S.OUT.ALL_OUT));
	
	MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CAN_SEND = setOut * (float) (MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].PID_S.OUT.ALL_OUT/allOutPower);
MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CAN_SEND = setOut * (float) (MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].PID_S.OUT.ALL_OUT/allOutPower);
	MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CAN_SEND = setOut * (float) (MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].PID_S.OUT.ALL_OUT/allOutPower);
	MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CAN_SEND = setOut * (float) (MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].PID_S.OUT.ALL_OUT/allOutPower);
}

float rebuffer;
double raid,raidTemp;
int32_t ii;

int32_t PowerLimBuffer_Cap(uint16_t maxPower , float buffer , int32_t setOut)
{
	const float residueBuffer = 15.0f;		//要求的最低缓冲剩余
	static double Raid = 0.0f;
	double raid_temp = 0.0f;
	int32_t reData = 0;

	capData_t.capGetDate.buffer += (maxPower - capData_t.capGetDate.nowPower - 6) * 0.001f;
	capData_t.capGetDate.buffer = SectionLimit_f(60.0f , 0.01f , capData_t.capGetDate.buffer);

	buffer -= residueBuffer;		//减去最少剩余后的缓冲

	buffer = SectionLimit_f(200 , 0 , buffer-residueBuffer);
	//计算比例系数
	Raid = (float) buffer/(60.0f - residueBuffer);
	Raid = SectionLimit_f(1.0f , 0.1f , Raid);
	//平方
	raid_temp = Raid * Raid;

	reData = (float)(raid_temp * setOut);

	AllotPower(reData);

	rebuffer = residueBuffer;
	raid = Raid;
	raidTemp = raid_temp;
	ii = reData;
	
	return reData;

}


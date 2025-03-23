#include "main.h"
#include "math.h"

#include "CHASSIS.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "YU_MATH.h"
#include "DBUS.h"
#include "YU_PID.h"
#include "TIM_DEV.h"

#include "Read_Data.h"
#include "ROOT.h"
#include "YU_MATH.h"
#include "TOP.h"
#include "chassis_power_control.h"

double ANGLE_Rad = 0.0f;
double ANGLE_Relative = 0.0f;

int mod = 0;
		
// 新增宏定义
#define FRONT_WHEEL_THRESHOLD  800.0f    // 前轮离地电流阈值
#define REAR_POWER_BOOST       1.5f      // 后轮功率提升系数

// 电机索引宏定义（需根据实际硬件配置验证）
#define FRONT_LEFT    MOTOR_D_CHASSIS_4  // 电机3
#define FRONT_RIGHT   MOTOR_D_CHASSIS_1  // 电机4
#define REAR_LEFT     MOTOR_D_CHASSIS_3  // 电机1
#define REAR_RIGHT    MOTOR_D_CHASSIS_2  // 电机2


void CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    // 运动学解算
    float VX = 0.0f, VY = 0.0f, VR = 0.0f, COMPONENT[2] = {1, 3.5};
    float ROTATE_VX = 0.0f, ROTATE_VY = 0.0f;  // 旋转矩阵
    double PRIDICT = 0.0f;    // 底盘预测，前馈

    ANGLE_Relative = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW - (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INIT;  // if add 4096
    spinLittleRound(&ANGLE_Relative);
    ANGLE_Rad = ANGLE_Relative * MATH_D_RELATIVE_PARAM;

    VX =  (float)((DBUS->REMOTE.CH0_int16) + (DBUS->KEY_BOARD.D - DBUS->KEY_BOARD.A) * 660.0f) * 10.0f;
    VY =  (float)((DBUS->REMOTE.CH1_int16) + (DBUS->KEY_BOARD.W - DBUS->KEY_BOARD.S) * 660.0f) * 10.0f;
    VR = -(float)((DBUS->REMOTE.DIR_int16) + (DBUS->KEY_BOARD.SHIFT) * 660.0f) * 3.0f;

    if (DBUS->KEY_BOARD.V)
    {
        VR *= 0.4f;
    }
    
    // @TODO 2. VR的负号和-ANGLE_Relative的负号测试是否可以全换成正号
    (!DBUS->REMOTE.DIR_int16)?(PRIDICT = DBUS->REMOTE.CH2_int16 * 2.0f,VR = PID_F_Cal(&FOLLOW_PID, 0, -ANGLE_Relative)):(PRIDICT = 0.0f);     // 分离 滚轮影响小陀螺

    // rotate matrix
    double COS = cos(ANGLE_Rad);
    double SIN = sin(ANGLE_Rad);
    
    ROTATE_VX = -VY * SIN + VX * COS;
    ROTATE_VY =  VY * COS + VX * SIN;
    
    /* 新增飞坡控制逻辑 */
    // 读取前轮电流绝对值（单位：mA）
    float front_left_current = fabsf(MOTOR[FRONT_LEFT].DATA.CURRENT);
    float front_right_current = fabsf(MOTOR[FRONT_RIGHT].DATA.CURRENT);
    float rear_left_current = fabsf(MOTOR[REAR_LEFT].DATA.CURRENT);
    float rear_right_current = fabsf(MOTOR[REAR_RIGHT].DATA.CURRENT);

    // 计算前左和后左电流绝对值之差，前右和后右电流绝对值之差，前左和前右电流绝对值之差
    float front_current_diff = fabsf(front_left_current - front_right_current);
    float rear_current_diff = fabsf(rear_left_current - rear_right_current);
    // float front_avg_current = (front_left_current + front_right_current) * 0.5f;

    // 如果前左和后左电流绝对值之差和前右和后右电流绝对值之差都大于阈值，则认为前轮离地
    DBUS->is_front_lifted = (front_current_diff > FRONT_WHEEL_THRESHOLD) && (rear_current_diff > FRONT_WHEEL_THRESHOLD);

    // 修改后的功率分配逻辑

    // if(DBUS->is_front_lifted && !DBUS->IS_OFF && DBUS->REMOTE.S1_u8 == 2)//暂时注释发射，复用拨杆
    // {
    //     // 前轮离地状态：提升后轮功率
    //     float boosted_rear_left = MOTOR[REAR_LEFT].DATA.AIM * REAR_POWER_BOOST;
    //     float boosted_rear_right = MOTOR[REAR_RIGHT].DATA.AIM * REAR_POWER_BOOST;

    //     // 应用功率调整并关闭前轮
    //     MOTOR[REAR_LEFT].DATA.AIM = boosted_rear_left;
    //     MOTOR[REAR_RIGHT].DATA.AIM = boosted_rear_right;
    //     MOTOR[FRONT_LEFT].DATA.AIM = 0;
    //     MOTOR[FRONT_RIGHT].DATA.AIM = 0;

    // }

    if(0)
    {
        
    }
    else
    {
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = ( ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = ( ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    }
    // 运动学解算
    // (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = ( ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    // (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    // (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    // (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = ( ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    // MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = ( ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    // MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    // MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;
    // MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = ( ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT;

    // pid 解算
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_1]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_2]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_3]);
    PID_F_S(&MOTOR[MOTOR_D_CHASSIS_4]);
//    mod = (((DBUS_V_DATA.REMOTE.S1_u8 - 1) == 0) ? 1 : 0);
	// 		if(DBUS_V_DATA.REMOTE.S1_u8 == 1)
    // chassis_power_control(1);
	// 		else
				chassis_power_control(0);

                //正常使用电容
                // chassis_power_control(DBUS_V_DATA.REMOTE.S1_u8 == 1 && capData_t.capSetData.dataNeaten.power_key ==1 &&capData_t.capSetData.dataNeaten.out_switch == 1) ;
       
    
        
			
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
        CHASSIS_V_PwrLimit.PwrPred[i] = CHASSIS_V_PwrLimit.k * current[i] * MOTOR[i].DATA.SPEED_NOW \
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

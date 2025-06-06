#include "main.h"
#include "math.h"

#include "CHASSIS.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "YU_MATH.h"
#include "DBUS.h"
#include "YU_PID.h"
#include "TIM_DEV.h"
#include "VISION.h"

#include "Read_Data.h"
#include "ROOT.h"
#include "YU_MATH.h"
#include "TOP.h"
#include "chassis_power_control.h"
#include "VOFA.h"

double ANGLE_Rad = 0.0f;
double ANGLE_Relative = 0.0f;

int mod = 0;
		
// 新增宏定义
#define FRONT_POWER_POWER       0.1f     // 前轮功率最高降低系数
#define REAR_POWER_BOOST        1.9f     // 后轮功率最低提升系数

#define FILTER_COEF 0.9f         // 低通滤波系数
#define FRONT_LIFT_THRESHOLD 3000.0f  // 前后轮电流差阈值
#define LIFT_TIME_THRESHOLD 100      // 连续检测次数阈值
#define LANDING_TIME_THRESHOLD 50    // 着地确认次数阈值

// 滤波后的电流值
float filtered_currents[4] = {0.0f};
// 飞坡状态计数器
int16_t lift_counter = 0;
int16_t landing_counter = 0;
uint8_t real_lift_state = 0;    // 0=地面, 1=飞坡中

// 电机索引宏定义 （头超前背向）
#define FRONT_LEFT    MOTOR_D_CHASSIS_4  // 电机4
#define FRONT_RIGHT   MOTOR_D_CHASSIS_1  // 电机1
#define REAR_LEFT     MOTOR_D_CHASSIS_3  // 电机3
#define REAR_RIGHT    MOTOR_D_CHASSIS_2  // 电机2

float watch[10] = {0};


// 添加缓启动相关参数
#define SOFT_START_RATE      0.5f   // 缓启动速率系数 (0.0-1.0)，越小越平滑
#define SOFT_START_THRESHOLD 20.0f   // 小于此阈值时直接到达目标值

// 上一次的电机目标值
static float last_motor_aim[4] = {0.0f, 0.0f, 0.0f, 0.0f};
static uint8_t soft_start_init = 0;  // 是否已初始化

uint8_t cap_mode_ctrl[2] = {0}; // ui 查看 这里的now last 对比用于节省ui时间
uint8_t chassis_control[2][2] = {0}; // ui 查看 0f 1g

void CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    // 运动学解算
    float VX = 0.0f, VY = 0.0f, VR = 0.0f, COMPONENT[2] = {4.0, 3.5};
    float ROTATE_VX = 0.0f, ROTATE_VY = 0.0f;  // 旋转矩阵
    double PRIDICT = 0.0f;    // 底盘预测，前馈

    ANGLE_Relative = (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_NOW - (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INIT;  // if add 4096
    spinLittleRound(&ANGLE_Relative);
    ANGLE_Rad = ANGLE_Relative * MATH_D_RELATIVE_PARAM;

    DBUS->key_flitter.w += 0.05f * (DBUS->KEY_BOARD.W - DBUS->key_flitter.w);
    DBUS->key_flitter.s += 0.05f * (DBUS->KEY_BOARD.S - DBUS->key_flitter.s);
    DBUS->key_flitter.a += 0.05f * (DBUS->KEY_BOARD.A - DBUS->key_flitter.a);
    DBUS->key_flitter.d += 0.05f * (DBUS->KEY_BOARD.D - DBUS->key_flitter.d);
    DBUS->key_flitter.shift += 0.05f * (DBUS->KEY_BOARD.SHIFT - DBUS->key_flitter.shift);

    VX =  (float)((DBUS->REMOTE.CH0_int16) + (DBUS->key_flitter.d - DBUS->key_flitter.a) * 660.0f) * 2.0f;
    VR = -(float)((DBUS->REMOTE.DIR_int16) + (DBUS->key_flitter.shift) * 660.0f) * 1.0f;
    (VR == 0) ? (VY =  (float)((DBUS->REMOTE.CH1_int16) + (DBUS->key_flitter.w - DBUS->key_flitter.s) * 660.0f) * 16.0f) : (VY =  (float)((DBUS->REMOTE.CH1_int16) + (DBUS->key_flitter.w - DBUS->key_flitter.s) * 660.0f) * 4.0f);
    
    if (DBUS->KEY_BOARD.G)
    {
        chassis_control[1][NOW] = 1;
        VR *= 0.2f;  
        VX *= 0.2f;
        VY *= 0.2f;
    } else chassis_control[1][NOW] = 0;
    
    // @TODO 2. VR的负号和-ANGLE_Relative的负号测试是否可以全换成正号
    if (DBUS->REMOTE.S2_u8 != 1)
    {
        (!((DBUS->REMOTE.DIR_int16)||(DBUS->KEY_BOARD.SHIFT)))?(PRIDICT = DBUS->REMOTE.CH2_int16 * 3.0f,VR = PID_F_Cal(&FOLLOW_PID, 0, -ANGLE_Relative)):(PRIDICT = 0.0f);     // 分离 滚轮影响小陀螺
    }
    
    // rotate matrix
    double COS = cos(ANGLE_Rad);
    double SIN = sin(ANGLE_Rad);
    
    ROTATE_VX = -VY * SIN + VX * COS;
    ROTATE_VY =  VY * COS + VX * SIN;

    // 飞坡控制
    
    // 运动学解算
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_1].DATA.AIM = ( ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_2].DATA.AIM = (-ROTATE_VX - ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_3].DATA.AIM = (-ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);
    (DBUS->IS_OFF) ? (MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = 0) : (MOTOR[MOTOR_D_CHASSIS_4].DATA.AIM = ( ROTATE_VX + ROTATE_VY - VR * COMPONENT[0]) * COMPONENT[1] + PRIDICT);

    CHASSIS_F_SoftStart(MOTOR);
    CHASSIS_F_Lifited(MOTOR, DBUS);

    //正常使用电容
    if (DBUS->KEY_BOARD.C && !DBUS->KEY_BOARD.C_PREE_NUMBER) {
        cap_mode_ctrl[NOW] = !cap_mode_ctrl[NOW];  // 切换电容模式
    }

    DBUS->KEY_BOARD.C_PREE_NUMBER = DBUS->KEY_BOARD.C;  
    DBUS->KEY_BOARD.F_PREE_NUMBER = DBUS->KEY_BOARD.F;  
    DBUS->KEY_BOARD.G_PREE_NUMBER = DBUS->KEY_BOARD.G;  

    if (DBUS->KEY_BOARD.F) {
        chassis_control[0][NOW] = 1;
        MOTOR[FRONT_LEFT].DATA.AIM  *= 0.4f;
        MOTOR[FRONT_RIGHT].DATA.AIM *= 0.4f;
        MOTOR[REAR_LEFT].DATA.AIM   *= 1.6f;
        MOTOR[REAR_RIGHT].DATA.AIM  *= 1.6f;
    } else chassis_control[0][NOW] = 0;


    // pid 解算
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_1]);
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_2]);
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_3]);
    PID_F_SC(&MOTOR[MOTOR_D_CHASSIS_4]);
    chassis_power_control(cap_mode_ctrl[NOW], DBUS->is_front_lifted);


}



static void CHASSIS_F_Lifited(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    static float front_power_scale = 1.0f;
    static float rear_power_scale = 1.0f;

    /* 新增飞坡控制逻辑 */
    // 读取前轮电流绝对值（单位：mA）
    float front_left_current  = MATH_D_ABS((float)MOTOR[FRONT_LEFT].DATA.CURRENT);
    float front_right_current = MATH_D_ABS((float)MOTOR[FRONT_RIGHT].DATA.CURRENT);
    float rear_left_current   = MATH_D_ABS((float)MOTOR[REAR_LEFT].DATA.CURRENT);
    float rear_right_current  = MATH_D_ABS((float)MOTOR[REAR_RIGHT].DATA.CURRENT);

    // 计算前左和后左电流绝对值之差，前右和后右电流绝对值之差，前左和前右电流绝对值之差
    float front_current_diff = MATH_D_ABS((front_left_current - front_right_current));
    float rear_current_diff  = MATH_D_ABS((rear_left_current - rear_right_current));
    /* 新增 start*/
    // 1. 低通滤波平滑电流数据
    filtered_currents[0] = FILTER_COEF * filtered_currents[0] + (1.0f - FILTER_COEF) * front_left_current;
    filtered_currents[1] = FILTER_COEF * filtered_currents[1] + (1.0f - FILTER_COEF) * front_right_current;
    filtered_currents[2] = FILTER_COEF * filtered_currents[2] + (1.0f - FILTER_COEF) * rear_left_current;
    filtered_currents[3] = FILTER_COEF * filtered_currents[3] + (1.0f - FILTER_COEF) * rear_right_current;

    // 2. 计算前后轮平均电流
    float front_avg_current = (filtered_currents[0] + filtered_currents[1]) * 0.5f;
    float rear_avg_current  = (filtered_currents[2] + filtered_currents[3]) * 0.5f;
    float current_diff = MATH_D_ABS((front_avg_current - rear_avg_current));

    // 3. 状态机判断，避免抖动
    uint8_t current_state = (current_diff > FRONT_LIFT_THRESHOLD);

    // 4. 状态确认机制
    if (current_state && !real_lift_state) {
        lift_counter++;
        landing_counter = 0;
        if (lift_counter >= LIFT_TIME_THRESHOLD) {
            real_lift_state = 1;  // 确认飞坡状态
            lift_counter = LIFT_TIME_THRESHOLD;  // 防止溢出
        }
    } else if (!current_state && real_lift_state) {
        landing_counter++;
        lift_counter = 0;
        if (landing_counter >= LANDING_TIME_THRESHOLD) {
            real_lift_state = 0;  // 确认着地状态
            landing_counter = LANDING_TIME_THRESHOLD;  // 防止溢出
        }
    }

    // 5. 更新状态标志位
    DBUS->is_front_lifted = real_lift_state;
    /* 新增 end */
    
    if(DBUS->is_front_lifted && !DBUS->IS_OFF)  // 移除了拨杆限制，使算法自动工作
    {
        // 根据离地程度动态调整功率提升系数 (1.5-2.5倍)
        float boost_factor = fminf(2.5f, 1.5f + current_diff / 1000.0f);
        float decrease_factor = 1.0f / boost_factor;

        // 平滑过渡：逐渐减少前轮功率，增加后轮功率
        // 更新功率比例 (平滑过渡)
        front_power_scale = fmaxf(FRONT_POWER_POWER, front_power_scale * decrease_factor); // 限制最小值
        rear_power_scale =  fmaxf(REAR_POWER_BOOST, 1.0f + (boost_factor - 1.0f) * (1.0f - front_power_scale));
        
        // 应用功率调整
        MOTOR[FRONT_LEFT].DATA.AIM  *= front_power_scale;
        MOTOR[FRONT_RIGHT].DATA.AIM *= front_power_scale;
        MOTOR[REAR_LEFT].DATA.AIM   *= rear_power_scale;
        MOTOR[REAR_RIGHT].DATA.AIM  *= rear_power_scale;
        // MOTOR[FRONT_LEFT].DATA.AIM *= FRONT_POWER_POWER;
        // MOTOR[FRONT_RIGHT].DATA.AIM *= FRONT_POWER_POWER;
        // MOTOR[REAR_LEFT].DATA.AIM *= REAR_POWER_BOOST;
        // MOTOR[REAR_RIGHT].DATA.AIM *= REAR_POWER_BOOST;
    }
    else
    {
        // 平地状态：平滑恢复正常功率分配
        // 缓慢恢复
        front_power_scale = fminf(1.0f, front_power_scale + 0.05f);
        rear_power_scale  = fmaxf(1.0f, 1.0f + (rear_power_scale - 1.0f) * 0.9f);
    }

    watch[0] = current_diff;
    watch[1] = real_lift_state;
    watch[2] = landing_counter;
    watch[3] = lift_counter;
    watch[4] = front_power_scale;
    watch[5] = rear_power_scale;
    
}

// 缓启动函数
static void CHASSIS_F_SoftStart(TYPEDEF_MOTOR *MOTOR)
{
    // 4个电机逐步实现目标值
    for(uint8_t i = 0; i < 4; i++)
    {
        // 计算目标值与当前值的差距
        float target_aim = MOTOR[MOTOR_D_CHASSIS_1 + i].DATA.AIM;
        float aim_diff = target_aim - last_motor_aim[i];
        
        // 如果是首次使用或差值很小，直接赋值
        if(!soft_start_init || fabsf(aim_diff) < SOFT_START_THRESHOLD)
        {
            last_motor_aim[i] = target_aim;
        }
        else
        {
            // 限制每次更新的最大变化量，实现平滑过渡
            last_motor_aim[i] += aim_diff * SOFT_START_RATE;
        }
        
        // 应用平滑后的目标值
        MOTOR[MOTOR_D_CHASSIS_1 + i].DATA.AIM = last_motor_aim[i];
    }
    
    // 标记为已初始化
    soft_start_init = 1;
}

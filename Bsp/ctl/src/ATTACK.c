#include "main.h"

#include "ATTACK.h"
#include "DEFINE.h"
#include "MOTOR.h"
#include "DBUS.h"
#include "YU_PID.h"
#include "TIM_DEV.h"
#include "ROOT.h"
#include "YU_MATH.h"
#include "Read_Data.h"
#include "VISION.h"

#include "VOFA.h"
#include "stdlib.h"
#include "bsp_dwt.h"
#include <math.h>

// delete later
#include "CAN_DEV.h"
#include "can.h"

#define ATTACK_D_TIMEOUT 300
#define ATTACK_D_SPEED 30.0f

TYPEDEF_ATTACK_PARAM ATTACK_V_PARAM = {0};

double param[5] = {0};

int IOTA = 0; // test
double M;     // test

float jam_duration_debug, disable_count_debug = 0.0f; // test    
float aim_get_edge = 0.0f;
float initial_speed[2] = {0};
float compensation_hz ;
// @TODO 整合到整个ROOT_init函数中, 记得Init 时间 && 摩擦轮目标值应根据裁判系统拟合
uint8_t ATTACK_F_Init(TYPEDEF_MOTOR *MOTOR)
{
    ATTACK_V_PARAM.TIME = 0;

    // 数据初始化
    ATTACK_V_PARAM.SINGLE_ANGLE = 36864.0f;
    ATTACK_V_PARAM.SPEED = 7200.0f;

    ATTACK_V_PARAM.FLAG = 1;
    ATTACK_V_PARAM.LOCK = 1; // 默认上锁，保证在未收到遥控数据时拨盘不动

    // 电机初始化
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM =  0.0f;
    MOTOR[MOTOR_D_ATTACK_R].DATA.AIM =  0.0f;
    MOTOR[MOTOR_D_ATTACK_G].DATA.AIM =  (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE;

    return ROOT_READY;
}

/**
 * @brief 根据遥控/鼠标输入计算拨弹电机目标值
 * @param MOTOR 拨弹电机指针
 * @param DBUS 遥控器数据指
 * @param autofire 视觉控制开火
 * @return 目标角度值
 */
float ATTACK_F_JAM_Aim(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS, uint8_t autofire)
{
    // 记录上一次的遥控器和鼠标状态
    static uint8_t prev_S1_state = DBUS_D_MOD_SHUT;
    
    // 单发模式处理 (通过状态变化检测)
    if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SINGLE)
    {
        // 检测遥控器模式切换到单发
        if (prev_S1_state != DBUS_D_MOD_SINGLE)
        {
            if (autofire == 0 || ((autofire == 1) && VISION_V_DATA.RECEIVE.fire))
            {
                ATTACK_V_PARAM.COUNT = 1;  // 增加一个弹丸的角度
            }
        }
    }
    // 鼠标单发处理
    else if (DBUS->MOUSE.L_STATE == 1 && ATTACK_V_PARAM.PREV_MOUSE_STATE != 1)
    {
        if (autofire == 0 || ((autofire == 1) && VISION_V_DATA.RECEIVE.fire))
        {
            ATTACK_V_PARAM.COUNT = 1;  // 检测到鼠标左键按下事件，发射一个弹丸
        }
    }
    // 连发模式处理
    else if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_CONSIST || DBUS->MOUSE.L_STATE == 2)
    {
        if (autofire == 0 || ((autofire == 1) && VISION_V_DATA.RECEIVE.fire))
        {
            // 视觉允许开火且为连发模式
            ATTACK_V_PARAM.COUNT = 1;  // 持续小量增加目标角度，形成连续转动
        }
    }
    // 关闭发射处理
    else if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SHUT)
    {
        ATTACK_V_PARAM.COUNT = 0;  // 关闭状态不增加角度
    }
    
    // 计算新的电机目标角度
    if (ATTACK_V_PARAM.COUNT > 0 && ATTACK_V_PARAM.fire_wheel_status && MOTOR->DATA.ENABLE) // @debug  
    {
        MOTOR->DATA.AIM = (float)MOTOR->DATA.ANGLE_INFINITE - ATTACK_V_PARAM.SINGLE_ANGLE * ATTACK_V_PARAM.COUNT;
		// 单发模式下，处理完一次后重置COUNT
        if (DBUS->REMOTE.S1_u8 == DBUS_D_MOD_SINGLE || DBUS->MOUSE.L_STATE == 1)
        {
            ATTACK_V_PARAM.COUNT = 0;
        }
    }
    
    // 保存当前状态用于下一次比较
    ATTACK_V_PARAM.PREV_MOUSE_STATE = DBUS->MOUSE.L_STATE;
    prev_S1_state = DBUS->REMOTE.S1_u8;

    return MOTOR->DATA.AIM;
}

// 判断卡弹 @TODO 将超时判断改为ETC时钟 &&
uint8_t ATTACK_F_JAM_Check(TYPEDEF_MOTOR *MOTOR)
{
    float DIFF = fabsf(MOTOR->DATA.AIM - (float)MOTOR->DATA.ANGLE_INFINITE);
    float EDGE = (float)(ATTACK_V_PARAM.SINGLE_ANGLE / 5.0f);

    if (((DIFF >= EDGE) && ((fabsf((float)MOTOR->DATA.SPEED_NOW)) <= ATTACK_D_SPEED)))
    {
        if (ATTACK_V_PARAM.TIME >= ATTACK_D_TIMEOUT)
        {
            ATTACK_V_PARAM.is_jam = 1;
            return ROOT_ERROR;
        }
        else
        {
            ATTACK_V_PARAM.TIME++;
            return ROOT_READY;
        }
    }
    else // edge到&&速度很小(到达) / edge到&&速度很大(连发) / edge不到&&速度很大(正在转)
    { 
        ATTACK_V_PARAM.TIME = 0;
        ATTACK_V_PARAM.is_jam = 0;
        return ROOT_READY;
    }
}


// 获取摩擦轮目标值  @TODO 应该是一个函数，等会写一个如何获取拟合函数
// @TODO 电机当前转速也值得加入，但这之后再说
float ATTACK_F_FIRE_Aim(TYPEDEF_MOTOR *MOTOR)
{
    static uint8_t fire_mouse_status = 0;
    // @veision 1 拟合
    // float a = 0.0f, b = 0.0f;
    // ATTACK_V_PARAM.SPEED = user_data.shoot_data.initial_speed * a + b;
    // return ATTACK_V_PARAM.SPEED;

    // @version 2, Use this code, this code is get the fire speed by judgement system
    // but the weakness is that the speed is not stable
    
    static float TEMP = 0.0f;
//    initial_speed[NOW] = user_data.shoot_data.initial_speed;
    initial_speed[LAST] = initial_speed[NOW]; // 保证attack速度只在更新时修改摩擦轮转速
    initial_speed[NOW] = user_data.shoot_data.initial_speed; // 更新摩擦轮转速
    if (initial_speed[NOW] != initial_speed[LAST])
    {
        if(user_data.shoot_data.initial_speed <= 23.6f)
        {
            TEMP += 4;
        }
        else if (user_data.shoot_data.initial_speed > 23.6f && user_data.shoot_data.initial_speed <= 23.9f)
        {
            TEMP += 1.5f;
        }
        else if (user_data.shoot_data.initial_speed > 23.9f && user_data.shoot_data.initial_speed <= 24.1f)
        {
            TEMP += 0;
        }
        else if (user_data.shoot_data.initial_speed > 24.1f && user_data.shoot_data.initial_speed <= 24.5f)
        {
            TEMP -= 5;
        }
        else if (user_data.shoot_data.initial_speed > 24.5f)
        {
            TEMP -= 15;
        }
    }
    
    ATTACK_V_PARAM.SPEED = 6500.0f + TEMP;
    ATTACK_V_PARAM.SPEED = MATH_D_LIMIT(6570.0f, 6380.0f, ATTACK_V_PARAM.SPEED);

    // @veision 3, final code, this code is a stable speed
    if (( fire_mouse_status == 1 && DBUS_V_DATA.REMOTE.S2_u8 == 3)|| DBUS_V_DATA.REMOTE.S2_u8 == 2)  // 3 is the fire button
    {
        MOTOR->DATA.AIM = ATTACK_V_PARAM.SPEED;
        ATTACK_V_PARAM.fire_wheel_status = 1;
    } else
    {
        MOTOR->DATA.AIM = 0.0f;
        ATTACK_V_PARAM.fire_wheel_status = 0;
    }
    if (DBUS_V_DATA.KEY_BOARD.CTRL  && !DBUS_V_DATA.KEY_BOARD.CTRL_PREE_NUMBER)
    {
        fire_mouse_status = !fire_mouse_status;
    }
    DBUS_V_DATA.KEY_BOARD.CTRL_PREE_NUMBER = DBUS_V_DATA.KEY_BOARD.CTRL;
    
    return MOTOR->DATA.AIM;
}


// 总控制函数
uint8_t ATTACK_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS)
{
    if (!MOTOR[MOTOR_D_ATTACK_G].is_off[NOW] && MOTOR[MOTOR_D_ATTACK_G].is_off[LAST]) {
        MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE;
    }
    MOTOR[MOTOR_D_ATTACK_G].is_off[LAST] = MOTOR[MOTOR_D_ATTACK_G].is_off[NOW];
    

    aim_get_edge = fabs((MOTOR[MOTOR_D_ATTACK_G].DATA.AIM) - (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE);
    if (aim_get_edge < 5000.0f) // @debug 5000.0f
        ATTACK_V_PARAM.STATUS[NOW] = 0;
    else ATTACK_V_PARAM.STATUS[NOW] = 1;
    
    if (ATTACK_F_HeatControl(&MOTOR[MOTOR_D_ATTACK_G], 0) && ATTACK_V_PARAM.STATUS[NOW] == 0)    // 判断方向优先考虑热量
    {
        MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = ATTACK_F_JAM_Aim(&MOTOR[MOTOR_D_ATTACK_G], DBUS, 0);
    }
    if (!ATTACK_F_JAM_Check(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_G]))
    {
        // 卡弹
        MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE+ ATTACK_V_PARAM.SINGLE_ANGLE * ATTACK_V_PARAM.FLAG; //  @debug  * ATTACK_V_PARAM.FLAG
        ATTACK_V_PARAM.FLAG = -ATTACK_V_PARAM.FLAG;
        ATTACK_V_PARAM.TIME = 0; // 重置时间
    }
    // shooting case when opposite to you l-r
    
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM = -ATTACK_F_FIRE_Aim(&MOTOR[MOTOR_D_ATTACK_L]);
    MOTOR[MOTOR_D_ATTACK_R].DATA.AIM =  ATTACK_F_FIRE_Aim(&MOTOR[MOTOR_D_ATTACK_R]);
    
    ATTACK_F_JAM_Disable(&MOTOR[MOTOR_D_ATTACK_G]);
    // ATTACK_T_FIT(40);
    // pid
    PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_L]);
    PID_F_SC(&MOTOR_V_ATTACK[MOTOR_D_ATTACK_R]);
    // PID_F_S(&MOTOR[MOTOR_D_ATTACK_L]);
    // PID_F_S(&MOTOR[MOTOR_D_ATTACK_R]);
    PID_F_AS(&MOTOR[MOTOR_D_ATTACK_G]);

    return ROOT_READY;
}


/// @brief Prevent segfaults, to be tested
/// @param  
/// @return return (ATTACK_T_Point())
double *ATTACK_T_Point(void)
{
    double* m = (double*)malloc(2 * sizeof(double));
    if (m == NULL) 
    {
        return NULL;
    }
    return m;
}

// int n;
// double x[50], y[50];
// /// @brief test recursive function
// /// @param a 
// /// @param b 
// /// @param k 
// /// @return 
// double recursive(double a, double b, int k)
// {
//     if (k >= n)
//         return a * x[n-1] + b;
//     double p = recursive(a, b, k + 1);
//     double e = y[k] - p;
//     double mu = 0.01;  // learning rate
//     double a_new = a + mu * e * x[k] / (1 + mu * x[k] * x[k]);
//     double b_new = b + mu * e / (1 + mu * x[k] * x[k]);
//     return a_new * x[k] + b_new;
// }


/// @brief fitting using VOFA
/// @param  size buffer length (@TODO + multi param length)
/// @return 
/// @date 2024-06-21
double *ATTACK_T_FIT(int size)
{
    // Param fitting using VOFA
    // in fact, the speed of motor is too fast, so we use rate to lower it.
    // VOFA_T_Send(2,
    //             user_data.shoot_data.initial_speed,
    //             MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW * 0.001f);

    // Simply cauculate the param a,b
    // 使用静态数组代替动态分配
    static double bufferA[50];
    static double bufferB[50];
    static int32_t count = 0;  // help IOTA replace

    // 限制size不超过静态数组大小
    if (size > 50) size = 50;
    
    bufferA[IOTA]   = user_data.shoot_data.initial_speed;
    bufferB[IOTA++] = MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW;
    count++;

    // choose special line by Least Squares Method
    double array[2] = {0};
    if (count > (size - 1))
    {
        double sumX = 0.0f, sumY = 0.0f, sumXY = 0.0f, sumXX = 0.0f;
        for (int i = 0; i < size; i++)
        {
            sumX  += bufferA[i];
            sumY  += bufferB[i];
            sumXY += bufferA[i] * bufferB[i];
            sumXX += bufferA[i] * bufferA[i];
        }
        if (size * sumXX - sumX * sumX != 0.0f && size > 0)
        { 
            array[0] = (size * sumXY - sumX * sumY) / (size * sumXX - sumX * sumX);
            array[1] = (sumY - array[0] * sumX) / size;
        }
        else
        {
            // @TODO return error pointer  -- type cauculate error
        }
        
        if(count % 50 == 0 || IOTA == size)
            IOTA = 0;  // replace the head of array
        double error = array[0] * user_data.shoot_data.initial_speed + array[1] - MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW;

        VOFA_T_Send(0, 10, array[0], array[1], user_data.shoot_data.initial_speed, MOTOR_V_ATTACK[2].DATA.SPEED_NOW,
                            error, 0, 0, 0, 0, 0);  // the place is not sure

        param[0] = array[0];  // a
        param[1] = array[1];  // b
        param[2] = error;    // error
        param[3] = user_data.shoot_data.initial_speed;  // speed
        param[4] = MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW;  // speed now
    }

    return &M;
}

/**
 * @brief 卡弹处理函数 - 累计3秒后失能拨弹电机，重新启动需要手动拨到目标位置
 * @param MOTOR 拨弹电机指针
 * @return 返回电机状态 1表示已失能 0表示未失能
 * @note 部分参数名称别看，瞎写的
 */
uint8_t ATTACK_F_JAM_Disable(TYPEDEF_MOTOR *MOTOR) 
{
    // 定义卡弹持续时间阈值(3秒)
    const float JAM_DISABLE_THRESHOLD = 10.0f;  // 单位：秒
    static uint8_t jam_disable_flag = 0;       // 记录是否已经失能
    static float jam_start_time = 0.0f;        // 记录卡弹开始时间
    static uint8_t last_jam_status = {0};        // 记录上一次卡弹状态
    float current_time = DWT_GetTimeline_s();
    
    // V键手动强制恢复电机
    if (DBUS_V_DATA.KEY_BOARD.V && !DBUS_V_DATA.KEY_BOARD.V_PREE_NUMBER) {
        jam_disable_flag = 0;
        jam_start_time   = 0.0f;
        MOTOR->DATA.ENABLE = 1;      // 重新使能
    }
    DBUS_V_DATA.KEY_BOARD.V_PREE_NUMBER = DBUS_V_DATA.KEY_BOARD.V;

    // 如果检测到卡弹
    if (ATTACK_V_PARAM.STATUS[NOW] == 1) {
        // 卡弹状态开始（从无卡弹到有卡弹的转变）
        if (last_jam_status == 0) {
            jam_start_time = current_time;  // 记录卡弹开始时间
            last_jam_status = 1;
        }
        
        // 计算卡弹持续时间
        float jam_duration = current_time - jam_start_time;
        jam_duration_debug = jam_duration;  // 调试用，记录卡弹持续时间
        // 如果卡弹持续超过阈值
        if (jam_duration >= JAM_DISABLE_THRESHOLD && !jam_disable_flag) {
            // 失能拨弹电机
            MOTOR->DATA.ENABLE = 0;
            jam_disable_flag = 1;  // 标记已失能
            disable_count_debug++;
            return ROOT_READY;  // 返回成功状态
        }
        
    } 
    else {
        // 卡弹解除
        if (last_jam_status == 1) {
            // 状态从有卡弹变为无卡弹
            last_jam_status = 0;
        }
            MOTOR->DATA.ENABLE = 1;
            jam_disable_flag = 0;
    }
    // 失能后 5s+10s*n 重新启动拨弹电机
    
    // VOFA_T_Send(0, 10, (float)ATTACK_V_PARAM.STATUS[NOW],
    //         (float)jam_start_time,
    //         (float)current_time,
    //         (float)jam_duration_debug,
    //         (float)MOTOR->DATA.ENABLE, 
    //         (float)MOTOR->DATA.AIM, 
    //         (float)MOTOR->DATA.ANGLE_INFINITE, 
    //         (float)ATTACK_V_PARAM.TIME, 
    //         (float)(1.0f/jam_duration_debug),
    //         (float)user_data.shoot_data.launching_frequency
    //     );
    return ROOT_ERROR;  // 返回未失能状态
}

/**
 * @brief 弹频拟合函数
 * @param type 1:线性拟合 2:二次拟合 3:考虑卡弹 4:不考虑卡弹
 */
float ATTACK_F_FireRate_Control(TYPEDEF_MOTOR *motor, float hz, uint8_t type)
{
    switch (type)
    {
    case 1: {// 线性拟合
        // 线性拟合 (y = ax + b):      x = np.array([3000.0, 4500.0, 5000.0])
        //                            y = np.array([13.3, 16.99, 23.3])
        //                            方程: y = 0.0044x + -0.5277
        //                            R² = 0.8253
        float a = 0.0044f, b = -0.5277f;
        float y = a * hz + b;
        motor->PID_A.IN.ALL_LIT = y;
        break;
    }
       
    case 2: {// 二次拟合
        float a = 0.0f, b = 0.0f, c = 0.0f;
        float y = a * hz * hz + b * hz + c;
        motor->PID_A.IN.ALL_LIT = y;
        break;
    }
    case 3: { // 考虑退弹卡弹
        const float TASK_RUN_TIME = 1.1f, JAM_COUNT = 10.0f; // 任务运行时间ms，JAM_COUNTs内卡弹1次数
        float a = ATTACK_D_TIMEOUT * TASK_RUN_TIME / 1000.0f, P = 1.0f / (hz * JAM_COUNT); // 假设卡弹一次0.5s 卡弹概率为2s内一发
        float fact_hz = 1.0f / ((1.0f - P) * 1.0f / hz + P * 2 * a);      // 考虑卡弹实际频率

        compensation_hz = ATTACK_Calc_Hz_From_FactHz(hz, a, P);
        compensation_hz = MATH_D_LIMIT(25.0f, 12.0f, compensation_hz); // 限制最大值
        motor->PID_A.IN.ALL_LIT = compensation_hz * 60.0f * 4.5f;
        break;
    }
    case 4: {
        motor->PID_A.IN.ALL_LIT = hz * 60.0f * 4.5f;
    }
    default:
        break;
    }
    return motor->PID_A.IN.ALL_LIT;
}

TYPEDEF_MOTOR aaa = {0};

enum {
    overhigh = 0,
    high = 1,
    medium = 2,
    low = 3,
    overlow = 4,
}heat;
uint8_t heatstatus = overlow;

/**
 * @brief 
 * 
 * @param motor 
 * @param type 类型选择 0停转 1减频
 * @return uint8_t 
 * @note type为0时，代码调用在获取拨弹目标值上，保证热量不足时不获取拨弹目标值
 * @note type为1时，代码直接调用，根据当前等级和热量控制弹频
 */
uint8_t ATTACK_F_HeatControl(TYPEDEF_MOTOR *motor, uint8_t type) 
{
    float d = 10.0f, shoot_time = 0.0f, shoot_speed = 0.0f;           
    float a = (float)(user_data.robot_status.shooter_barrel_cooling_value); // 冷却值 /s
    float m = fabsf((float)(user_data.robot_status.shooter_barrel_heat_limit - user_data.power_heat_data.shooter_17mm_1_barrel_heat)); // 剩余可发热量 10*n
    uint16_t leastbullet = (uint16_t)(m) / 10;
    float rate = (m+a * 0.1f)/(float)user_data.robot_status.shooter_barrel_heat_limit;
    if (a == 0) rate = 2.0f;  // 收不到裁判系统数据，设置为错误数据
    
    ATTACK_F_FireRate_Control(&aaa, 18.0f, 3);

    VOFA_T_Send(0, 10, (float)a, m, 
                       (float)user_data.robot_status.shooter_barrel_heat_limit, 
                       (float)user_data.power_heat_data.shooter_17mm_1_barrel_heat, 
                       shoot_time, (float)leastbullet, (float)ATTACK_V_PARAM.SPEED, 
                       (float)aaa.PID_S.IN.ALL_LIT, compensation_hz, 0); // @TODO 发送数据到VOFA
    if (type == 0)
    {
        switch (user_data.robot_status.robot_level)
        {
            case 1:
            {
                if (leastbullet >=5) return 1;
                else return 0;
            }
                break;
            case 2:
            {
                if (leastbullet >= 5) return 1;
                else return 0;
            }
                break;
            case 3:case 4:
            {
                if (leastbullet >= 5) return 1;
                else return 0;
            }
                break;
            case 5:case 6:
            {
                if (leastbullet >= 5) return 1;
                else return 0;
            }
                break;
            case 7:case 8:
            {
                if (leastbullet >= 3) return 1;
                else return 0;
            }
                break;
            case 9:case 10:
            {
                if (leastbullet >= 3) return 1;
                else return 0;
            }
                break;
            default:
            {
                return 1;
            }
                break;
            }
        }
    else if (type == 1)
    {
        static float fq = 15.0f;
        if (rate <= 0) // 超热量
        {
            fq = 0.0f;
        } else if (rate >0 && rate <= 1.0f) {
            // rate 0->1 0->18
            fq = -0.0586 * rate - 0.0431f;
        } else {
            fq = 15.0f; // 如果收不到裁判系统数据，定值
        }
        if (fq <= 0.0f) {
            fq = 0.0f;
        }
        ATTACK_F_FireRate_Control(motor, fq, 3);
    }
    return 1;
}

/**
 * @brief 反算设置频率 hz，使得考虑卡弹后实际频率为 fact_hz
 *        原公式：fact_hz = 1 / ( (1-P)/hz + 2 * a * P )
 * @param fact_hz    目标实际频率
 * @param a          卡弹时间系数 = ATTACK_D_TIMEOUT * TASK_RUN_TIME / 1000.0f
 * @param P          卡弹概率系数 = 1.0f / (hz * JAM_COUNT)
 * @return 反算得到的 hz 值 (>0)，否则返回 0
 */
float ATTACK_Calc_Hz_From_FactHz(float fact_hz, float a, float P)
{
    if (fact_hz <= 0.0f || P >= 1.0f) {
        return 0.0f;
    }
    // 1/fact_hz = (1-P)/hz + 2*a*P
    // (1-P)/hz = 1/fact_hz - 2*a*P
    // hz = (1-P) / (1/fact_hz - 2*a*P)
    float denom = 1.0f / fact_hz - 2.0f * a * P;
    if (denom <= 0.0f) {
        return 0.0f;
    }
    return (1.0f - P) / denom;
}
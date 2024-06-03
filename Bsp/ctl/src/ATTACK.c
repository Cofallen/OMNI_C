#include "main.h"
#include <time.h>
#include <stdio.h>

#include "ATTACK.h"
#include "DEFINE.h"
#include "YU_MATH.h"
#include "PID.h"
#include "JUDGEMENT.h"

/************************************************************万能分隔符**************************************************************
 * 	@author:			//瑞
 *	@performance:	    //
 *	@parameter:		    //
 *	@time:				//24-2-25 下午2:51
 *	@ReadMe:			//发射机构初始化
 ************************************************************万能分隔符**************************************************************/
struct
{
    int WIPE_MAX_SPEED;
    int WIPE_MIN_SPEED;
    int WIPE_MID_SPEED;
    int SINGLE_ANGLE;
} RUI_TYPEDEF_ATTACK;

uint8_t RUI_F_ATTACK_INIT()
{
    int param[4] = {0, 0, 0, 0};
    printf("发射机构初始化");
    RUI_TYPEDEF_ATTACK.WIPE_MAX_SPEED = param[0];
    RUI_TYPEDEF_ATTACK.WIPE_MIN_SPEED = param[1];
    RUI_TYPEDEF_ATTACK.WIPE_MID_SPEED = param[2];
    RUI_TYPEDEF_ATTACK.SINGLE_ANGLE = param[3];
    printf("成功-发射机构初始化");
    return ROOT_READY;
}
/************************************************************万能分隔符**************************************************************
 * 	@author:			//瑞
 *	@performance:	    //
 *	@parameter:		    //
 *	@time:				//2024/1/11 14:53
 *	@ReadMe:			//卡弹检测
 ************************************************************万能分隔符**************************************************************/
uint8_t RUI_F_JAM(TYPEDEF_MOTOR *MOTOR)
{
    int64_t ERROR_ANGLE = (int64_t)MOTOR->DATA.AIM - MOTOR->DATA.ANGLE_INFINITE;
    // 一定误差速度
    if (MATH_D_ABS(ERROR_ANGLE) > 200 && MATH_D_ABS(MOTOR->DATA.SPEED_NOW) < 100)
    {
        // 防止是正在拨弹过程中判断为卡弹，加入了时间，在一定的时间内还没有拨弹成功才判断为卡弹
        if (clock() - MOTOR->DATA.STUCK_TIME > 200000)
        {
            MOTOR->DATA.AIM = (float)(MOTOR->DATA.ANGLE_INFINITE - RUI_TYPEDEF_ATTACK.SINGLE_ANGLE);
            return ROOT_ERROR;
        }
    }
    else
    {
        // 正常拨弹时，可能角度会有误差，但速度不会低于某个值，所以判断为正常拨弹，更新卡弹时间
        MOTOR->DATA.STUCK_TIME = clock();
        return ROOT_READY;
    }
    return ROOT_READY;
}
/************************************************************万能分隔符**************************************************************
 * 	@author:			//瑞
 *	@performance:	    //
 *	@parameter:		    //
 *	@time:				//24-2-25 下午1:24
 *	@ReadMe:			//获取摩擦轮的目标值
 ************************************************************万能分隔符**************************************************************/
float RUI_F_GET_FIRE_WIPE_SPEED(TYPEDEF_DBUS *RUI_V_DBUS, RUI_TYPEDEF_JUDGE *RUI_V_JUDGE)
{
    static uint8_t LOCK = 0, MOD = 0, KEYBOARD_LOCK = 0, JUDGE_LOCK = 0;
    static float AIM = 0.0f, TEMP = 0.0f;
    MOD = RUI_V_DBUS->L_FLAG | RUI_V_DBUS->MOUSE.L_STATE | LOCK;
    // 停止
    // if (MOD == 0 || RUI_V_DBUS->L_FLAG == 1 || RUI_V_MONITOR.STATUS[NOW][RUI_D_STATUS_ID_DBUS] == RUI_D_STATUS_OFF)
    if (MOD == 0 || RUI_V_DBUS->L_FLAG == 1)
    {
        AIM = 0.0f; // 0.0
        LOCK = 0;
    }
    else
    {
        LOCK = 1;
        if (RUI_V_DBUS->KEY_BOARD.CTRL)
        {
            if (RUI_V_DBUS->KEY_BOARD.V && KEYBOARD_LOCK == 0)
            {
                TEMP += 10;
                KEYBOARD_LOCK = 1;
            }
            if (RUI_V_DBUS->KEY_BOARD.B && KEYBOARD_LOCK == 0)
            {
                TEMP -= 10;
                KEYBOARD_LOCK = 1;
            }
            if (RUI_V_DBUS->KEY_BOARD.V == 0 && RUI_V_DBUS->KEY_BOARD.B == 0)
            {
                KEYBOARD_LOCK = 0;
            }
        }
        else
        {
            KEYBOARD_LOCK = 0;
        }

        if (RUI_V_JUDGE->SHOOT_DATA.INITIAL_SPEED > 29.5f && JUDGE_LOCK == 0)
        {
            JUDGE_LOCK = 1;
            TEMP -= 50;
        }
        if (RUI_V_JUDGE->SHOOT_DATA.INITIAL_SPEED < 29.5f)
        {
            JUDGE_LOCK = 0;
        }
        AIM = 8000.0f + TEMP;
    }
    return AIM;
}
/************************************************************万能分隔符**************************************************************
 * 	@author:			//瑞
 *	@performance:	    //
 *	@parameter:		    //
 *	@time:				//24-2-25 下午1:46
 *	@ReadMe:			//获取拨弹电机的目标值
 ************************************************************万能分隔符**************************************************************/
int64_t RUI_F_GET_FIRE_AIM(TYPEDEF_DBUS *RUI_V_DBUS, TYPEDEF_MOTOR *MOTOR, RUI_TYPEDEF_JUDGE *RUI_V_JUDGE)
{
    static uint8_t SINGLE_LOCK = 0;
    static int64_t AIM = 0;
    uint8_t MOD = RUI_V_DBUS->R_FLAG | RUI_V_DBUS->MOUSE.L_STATE;
    // 停止
    if (MOD == 0)
    {
        // 单发解锁
        SINGLE_LOCK = 0;
        AIM = MOTOR->DATA.ANGLE_INFINITE;
    }
    else
    {
        if (MOD == 1 && SINGLE_LOCK == 0)
        {
            // 单发上锁
            SINGLE_LOCK = 1;
            AIM = MOTOR->DATA.ANGLE_INFINITE + (int)RUI_TYPEDEF_ATTACK.SINGLE_ANGLE;
        }
        if (MOD == 2)
        {
            // 这里应该乘根据裁判系统热量算出的发弹量
            AIM = MOTOR->DATA.ANGLE_INFINITE + (int)RUI_TYPEDEF_ATTACK.SINGLE_ANGLE;
        }
    }
    return AIM;
}
/************************************************************万能分隔符**************************************************************
 * 	@author:			//瑞
 *	@performance:	    //
 *	@parameter:		    //
 *	@time:				//2024/1/11 14:53
 *	@ReadMe:			//发射总控制函数
 ************************************************************万能分隔符**************************************************************/
uint8_t RUI_F_ATTACK(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *RUI_V_DBUS, RUI_TYPEDEF_JUDGE *RUI_V_JUDGE)
{

    static int INIT_FLAG = 0;
    if (!INIT_FLAG)
    {
        INIT_FLAG = RUI_F_ATTACK_INIT();
    }

    float RUI_V_FIRE_WIPE_SPEED = RUI_F_GET_FIRE_WIPE_SPEED(RUI_V_DBUS, RUI_V_JUDGE);
    MOTOR[MOTOR_D_ATTACK_L].DATA.AIM = -RUI_V_FIRE_WIPE_SPEED;
    MOTOR[MOTOR_D_ATTACK_R].DATA.AIM = RUI_V_FIRE_WIPE_SPEED;
    // 确保摩擦轮到达指定目标速度，在拨弹，防止堵弹
    if (MATH_D_ABS(MOTOR[MOTOR_D_ATTACK_L].DATA.SPEED_NOW - MOTOR[MOTOR_D_ATTACK_L].DATA.AIM) > 2500 || MOTOR[MOTOR_D_ATTACK_L].DATA.AIM == 0)
    {
        //        printf("确保摩擦轮到达指定目标速度SPEED_NOW : %d AIM %f", MOTOR[ MOTOR_D_ATTACK_L ].DATA.SPEED_NOW, MOTOR[ MOTOR_D_ATTACK_L ].DATA.AIM);
        MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = (float)MOTOR[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE;
    }
    else
    {
        static int16_t LOCK = 0, RUI_V_TEMP_TIME = 0;
        if (RUI_F_JAM(&MOTOR[MOTOR_D_ATTACK_G]) == ROOT_ERROR && LOCK == 0)
        {
            PID_F_AS(&MOTOR[MOTOR_D_ATTACK_G]);
            LOCK = 1;
            return ROOT_ERROR;
        }
        RUI_V_TEMP_TIME++;
        if (RUI_V_DBUS->R_FLAG == 1 || RUI_V_TEMP_TIME > 1000)
        {
            RUI_V_TEMP_TIME = 0;
            LOCK = 0;
        }
        if (LOCK != 1)
        {
            MOTOR[MOTOR_D_ATTACK_G].DATA.AIM = (float)RUI_F_GET_FIRE_AIM(RUI_V_DBUS, &MOTOR[MOTOR_D_ATTACK_G], RUI_V_JUDGE);
        }
    }

    PID_F_SC(&MOTOR[MOTOR_D_ATTACK_L]);
    PID_F_SC(&MOTOR[MOTOR_D_ATTACK_R]);
    PID_F_AS(&MOTOR[MOTOR_D_ATTACK_G]);

    return ROOT_READY;
}
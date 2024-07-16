#include "power_control.h"
#include "superCap.h"
#include "TIM_DEV.h"
#include "main.h"
#include "MOTOR.h"
#include "math.h"
#include "PID.h"
#include "shu_math.h"
TYPEDEF_MOTOR_PID PID_Buffer;
	
fp32 scaled_give_power[4];

fp32 toque_coefficient = 1.99688994e-6f; // (20/16384)*(0.3)*(187/3591)/9.55 力矩电流系数
fp32 a = 1.23e-07;						 // k1
fp32 k2 = 1.453e-07;					 // k2
fp32 constant = 4.081f;                  // a 增大这个系数可以减小功率，反之增加

/**
  * @author: 楠
  * @performance: 功率控制总函数
  * @parameter: 电容标志位（是否开启）	
  * @time: 24-4-1			
  * @ReadMe: 放在底盘PID解算后即可
*/ 
void chassis_power_control(uint8_t cap_state)
{
    //*可编辑部分*begin*//
    const uint16_t PowerCompensation = 10;  //正常模式下的功率补偿
    const uint16_t SuperMaxPower = 150;	    //超级电容下的功率补偿
    const uint16_t capValt = 140;	        //强制退出的电压阈值
    //*可编辑部分*end*//
	
	uint16_t max_power_limit = 50;  //最大功率限制
	fp32 chassis_max_power = 0;
	fp32 input_power = 0;		    // 输入功率（裁判系统）
	fp32 initial_give_power[4];    // 初始功率由PID计算以及电机数据得到
	fp32 initial_total_power = 0;
		
	fp32 chassis_power = 0.0f;
	fp32 chassis_power_buffer = 0.0f;
	
    chassis_power = user_data.power_heat_data.chassis_power;		// 得到底盘功率
    chassis_power_buffer = user_data.power_heat_data.buffer_energy;	// 得到缓冲能量
    max_power_limit = user_data.robot_status.chassis_power_limit;    // 得到最大功率限制

    PID_buffer_init(&PID_Buffer);
    PID_buffer(&PID_Buffer, chassis_power_buffer, 30);  // 缓冲能量闭环

    chassis_max_power = input_power;

    input_power = max_power_limit - PID_Buffer.OUT.ALL_OUT;  // 加入缓冲能量

    if(capData_t.capGetDate.capVolt > capValt)
	{	
        if(cap_state == 0)
        {
            chassis_max_power = input_power + PowerCompensation;    // 功率设置略大于最大输入功率，提高电容能量利用率
        }else{
            chassis_max_power = input_power + SuperMaxPower;        // 开启电容
        }
    }else{
        chassis_max_power = input_power;    // 电容电量低或电容离线时无补偿
    }

    //得到初始电机功率
    initial_give_power[0] = get_initial_power(&MOTOR_V_CHASSIS[0]);
    initial_give_power[1] = get_initial_power(&MOTOR_V_CHASSIS[1]);
    initial_give_power[2] = get_initial_power(&MOTOR_V_CHASSIS[2]);
    initial_give_power[3] = get_initial_power(&MOTOR_V_CHASSIS[3]);

    for(uint8_t i = 0; i < 4; i++)
    {
        if (initial_give_power[i] < 0) // 不考虑负功(反向电动势)
			continue;
		initial_total_power += initial_give_power[i]; // 获得底盘总功率
    }
    
	if (initial_total_power > chassis_max_power) // 确定是否大于最大功率
	{
		fp32 power_scale = chassis_max_power / initial_total_power;

		for(uint8_t i = 0; i < 4; i++)
		{
			scaled_give_power[i] = initial_give_power[i] * power_scale; // 获得缩放功率
		}
		
		//对每个电机分别进行功率限制
		chassis_power_limit(&MOTOR_V_CHASSIS[0]);
		chassis_power_limit(&MOTOR_V_CHASSIS[1]);
		chassis_power_limit(&MOTOR_V_CHASSIS[2]);
		chassis_power_limit(&MOTOR_V_CHASSIS[3]);
	}
}

/**
  * @author: 楠
  * @performance: 电机功率计算函数
  * @parameter: 电机结构体	
  * @time: 24-4-1			
  * @ReadMe: 依靠电机功率模型计算电机功率
*/ 
float get_initial_power(TYPEDEF_MOTOR *chassis_power_control)
{
    float initial_power;

    int32_t speed_rpm = (int32_t)chassis_power_control->DATA.SPEED_NOW;
	
   initial_power = chassis_power_control ->PID_S.OUT.ALL_OUT * toque_coefficient * speed_rpm + \
                   k2 * speed_rpm * speed_rpm + \
                   a * chassis_power_control -> PID_S.OUT.ALL_OUT * chassis_power_control -> PID_S.OUT.ALL_OUT + constant;    

    return initial_power;
}

/**
  * @author: 楠
  * @performance: 电机功率限制函数
  * @parameter: 电机结构体	
  * @time: 24-4-1			
  * @ReadMe: 对电机功率进行缩放进行功率再分配
*/ 
void chassis_power_limit(TYPEDEF_MOTOR *chassis_power_limit)
{
    uint8_t p = 0;

    if (chassis_power_limit == &MOTOR_V_CHASSIS[0]){
        p = 0;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[1]){
        p = 1;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[2]){
        p = 2;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[3]){
        p = 3;
    }

    if(scaled_give_power[p] < 0)
    {
        return;
    }

    fp32 speed_rpm = (int32_t)chassis_power_limit->DATA.SPEED_NOW;
    fp32 b = toque_coefficient * speed_rpm;
    fp32 c = k2 * speed_rpm * speed_rpm - scaled_give_power[p] + constant;

	
    if(chassis_power_limit->PID_S.OUT.ALL_OUT > 0)  // 根据原力矩方向选择计算公式
    {
        fp32 temp = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        if(temp > 16000)
        {
            chassis_power_limit->PID_S.OUT.ALL_OUT = 16000;
        }else{
             chassis_power_limit -> PID_S.OUT.ALL_OUT = temp;
        }
    }else {
        fp32 temp = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        if (temp < -16000)
		{
             chassis_power_limit -> PID_S.OUT.ALL_OUT = -16000;
        }else{
            chassis_power_limit -> PID_S.OUT.ALL_OUT = temp;
        }
    }
}

/**
  * @author: 楠
  * @performance: 缓冲能量PID计算
  * @parameter: PID 缓冲能量 要求剩余的最低缓冲能量	
  * @time: 24-4-1			
  * @ReadMe: 
*/ 
void PID_buffer(TYPEDEF_MOTOR_PID *PID_buffer, fp32 power_buffer, fp32 temp)
{
    PID_buffer->OUT.ERROR[0] = temp - power_buffer;
    /*比例输出*/
    PID_buffer->OUT.P_OUT = ( PID_buffer->OUT.ERROR[0] * PID_buffer->IN.KP);
    /*积分输出*/
    PID_buffer->OUT.I_OUT += (PID_buffer->OUT.ERROR[0] * PID_buffer->IN.KI);
    /*积分限幅*/
    PID_buffer->OUT.I_OUT = SectionLimit_f(PID_buffer->IN.I_LIT, -PID_buffer->IN.I_LIT, PID_buffer->OUT.I_OUT);
    /*微分输出*/
    PID_buffer->OUT.D_OUT = -(PID_buffer->OUT.ERROR[0] - PID_buffer->OUT.ERROR[1]) * PID_buffer->IN.KD;
    /*数据迭代*/
    PID_buffer->OUT.ERROR[1] = PID_buffer->OUT.ERROR[0];
    /*角度环总输出*/
    PID_buffer->OUT.ALL_OUT = (PID_buffer->OUT.P_OUT + PID_buffer->OUT.I_OUT + PID_buffer->OUT.D_OUT);
    /*总输出限幅*/
    PID_buffer->OUT.ALL_OUT = SectionLimit_f(PID_buffer->IN.ALL_LIT, -PID_buffer->IN.ALL_LIT, PID_buffer->OUT.ALL_OUT);
}

/**
  * @author: 楠
  * @performance: PID初始化
  * @parameter: PID	
  * @time: 24-4-1			
  * @ReadMe: 
*/ 
void PID_buffer_init(TYPEDEF_MOTOR_PID *PID_buffer)
{
    PID_buffer->IN.KP = 2;
    PID_buffer->IN.KI = 0;
    PID_buffer->IN.KD = 0;
    PID_buffer->IN.I_LIT = 0;
    PID_buffer->IN.ALL_LIT = 100;
}

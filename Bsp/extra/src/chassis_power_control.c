#include "chassis_power_control.h"
#include "TIM_DEV.h"
#include "CAP.h"
#include "DEFINE.h"
#include "math.h"
#include "YU_MATH.h"

PID_buffer_t PID_Buffer;
	
fp32 scaled_give_power[4];

fp32 toque_coefficient = 1.99688994e-6f; // (20/16384)*(0.3)*(187/3591)/9.55 ??????????
fp32 a = 1.23e-07;						 // k1
fp32 k2 = 1.453e-07;					 // k2
fp32 constant = 4.081f;                  // a ???????????????��????????????

/**
  * @author: ?
  * @performance: ????????????
  * @parameter: ??????��?????????
  * @time: 24-4-1
  * @ReadMe: ???????PID??????
*/
void chassis_power_control(uint8_t cap_state)
{
    //*???????*begin*//
    const uint16_t PowerCompensation = 10;  //???????????????
    const uint16_t SuperMaxPower = 150;	    //?????????????????
    const uint16_t capValt = 140;	        //?????????????
    //*???????*end*//

	uint16_t max_power_limit = 50;  //?????????
	fp32 chassis_max_power = 0;
	fp32 input_power = 0;		    // ???????????????
	fp32 initial_give_power[4];    // ?????????PID????????????????
	fp32 initial_total_power = 0;

	fp32 chassis_power = 0.0f;
	fp32 chassis_power_buffer = 0.0f;

    chassis_power = user_data.power_heat_data.chassis_power;		// ??????????
    chassis_power_buffer = user_data.power_heat_data.buffer_energy;	// ???????????
    // max_power_limit = user_data.power_heat_data.max_power_limit;    // ????????????
    max_power_limit = 150;    // @TODO 有时间测试一下是否能够限制，移植的代码 user.data 没有这个参数

    PID_buffer_init(&PID_Buffer);
    PID_buffer(&PID_Buffer, chassis_power_buffer, 30);  // ???????????

    chassis_max_power = input_power;

    input_power = max_power_limit - PID_Buffer.All_out;  // ??????????

    if(capData_t.capGetDate.capVolt > capValt)
	{
        if(cap_state == 0)
        {
            chassis_max_power = input_power + PowerCompensation;    // ???????????????????????????????????????
        }else{
            chassis_max_power = input_power + SuperMaxPower;        // ????????
        }
    }else{
        chassis_max_power = input_power;    // ???????????????????????
    }

    //?????????????
    initial_give_power[0] = get_initial_power(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1]);
    initial_give_power[1] = get_initial_power(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2]);
    initial_give_power[2] = get_initial_power(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3]);
    initial_give_power[3] = get_initial_power(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4]);

    for(uint8_t i = 0; i < 4; i++)
    {
        if (initial_give_power[i] < 0) // ?????????(????�T??)
			continue;
		initial_total_power += initial_give_power[i]; // ???????????
    }

	if (initial_total_power > chassis_max_power) // ??????????????
	{
		fp32 power_scale = chassis_max_power / initial_total_power;

		for(uint8_t i = 0; i < 4; i++)
		{
			scaled_give_power[i] = initial_give_power[i] * power_scale; // ??????????
		}

		//?????????????��???????
		chassis_power_limit(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1]);
		chassis_power_limit(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2]);
		chassis_power_limit(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3]);
		chassis_power_limit(&MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4]);
	}
}

/**
  * @author: ?
  * @performance: ????????????
  * @parameter: ???????
  * @time: 24-4-1
  * @ReadMe: ???????????????????????
*/
float get_initial_power(TYPEDEF_MOTOR *chassis_power_control)
{
    float initial_power;
    int32_t speed_rpm = (int32_t)chassis_power_control->DATA.SPEED_NOW;
    initial_power = chassis_power_control->PID_S.OUT.ALL_OUT * toque_coefficient * speed_rpm + \
                k2 * speed_rpm * speed_rpm + \
                a * chassis_power_control -> PID_S.OUT.ALL_OUT * chassis_power_control -> PID_S.OUT.ALL_OUT + constant;
    return initial_power;
}

/**
  * @author: ?
  * @performance: ??????????????
  * @parameter: ???????
  * @time: 24-4-1
  * @ReadMe: ?????????????????��????????
*/
void chassis_power_limit(TYPEDEF_MOTOR *chassis_power_limit)
{
    uint8_t p = 0;

    if (chassis_power_limit == &MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1]){
        p = 0;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2]){
        p = 1;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3]){
        p = 2;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4]){
        p = 3;
    }

    if(scaled_give_power[p] < 0)
    {
        return;
    }

    fp32 speed_rpm = (int32_t)chassis_power_limit->DATA.SPEED_NOW;
    fp32 b = toque_coefficient * speed_rpm;
    fp32 c = k2 * speed_rpm * speed_rpm - scaled_give_power[p] + constant;


    if(chassis_power_limit->PID_S.OUT.ALL_OUT > 0)  // ???????????????????
    {
        fp32 temp = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        if(temp > 16000)
        {
            chassis_power_limit -> PID_S.OUT.ALL_OUT = 16000;
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
  * @author: ?
  * @performance: ????????PID????
  * @parameter: PID ???????? ?????????????????
  * @time: 24-4-1
  * @ReadMe:
*/
void PID_buffer(PID_buffer_t *PID_buffer, fp32 power_buffer, fp32 temp)
{
    PID_buffer->Error[NOW] = temp - power_buffer;
    /*???????*/
    PID_buffer->P_out = (PID_buffer->Error[NOW] * PID_buffer->Kp);
    /*???????*/
    PID_buffer->I_out += (PID_buffer->Error[NOW] * PID_buffer->Ki);
    /*???????*/
    PID_buffer->I_out = MATH_D_LIMIT(PID_buffer->ILt, -PID_buffer->ILt, PID_buffer->I_out);
    /*??????*/
    PID_buffer->D_out = -(PID_buffer->Error[NOW] - PID_buffer->Error[LAST]) * PID_buffer->Kd;
    /*???????*/
    PID_buffer->Error[LAST] = PID_buffer->Error[NOW];
    /*?????????*/
    PID_buffer->All_out = (PID_buffer->P_out + PID_buffer->I_out + PID_buffer->D_out);
    /*????????*/
    PID_buffer->All_out = MATH_D_LIMIT(PID_buffer->AlLt, -PID_buffer->AlLt, PID_buffer->All_out);
}

/**
  * @author: ?
  * @performance: PID?????
  * @parameter: PID	
  * @time: 24-4-1			
  * @ReadMe: 
*/ 
void PID_buffer_init(PID_buffer_t *PID_buffer)
{
    PID_buffer->Kp = 2;
    PID_buffer->Ki = 0;
    PID_buffer->Kd = 0;
    PID_buffer->ILt = 0;
    PID_buffer->AlLt = 100;
}

#ifndef __POWERCOL_H
#define __POWERCOL_H

#include "MOTOR.h"
#include "CAP.h"
#include "yu_math.h"
#include "Read_Data.h"

typedef float fp32;

typedef struct  
{
	float Kp;
	float Ki;
	float Kd;
	float ILt;
	float AlLt;
	float Error[2];
	float P_out;
	float I_out;
	float D_out;
	float All_out;
}PID_buffer_t;


void chassis_power_control(uint8_t cap_state, uint8_t is_flying);
float get_initial_power(TYPEDEF_MOTOR *chassis_power_control);
void chassis_power_limit(TYPEDEF_MOTOR *chassis_power_limit);
void PID_buffer(PID_buffer_t *PID_buffer, fp32 power_buffer, fp32 temp);
void PID_buffer_init(PID_buffer_t *PID_buffer);

#endif


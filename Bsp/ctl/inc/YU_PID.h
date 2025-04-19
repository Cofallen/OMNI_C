#ifndef __PID_H__
#define __PID_H__

#include "MOTOR.h"
#include "user_lib.h"
#include "feedforward.h"

void PID_F_Init(TYPEDEF_MOTOR_PID *PID, const float *PID_V);
float PID_F_Cal(TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL);
// extern void PID_F_Clear(TYPEDEF_MOTOR *MOTOR);
uint8_t PID_F_AS(TYPEDEF_MOTOR *MOTOR);
uint8_t PID_F_SC(TYPEDEF_MOTOR *MOTOR);

uint8_t PID_F_S(TYPEDEF_MOTOR *MOTOR);
float PID_T_Cal(TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL, int8_t mode);
uint8_t PID_F_G(TYPEDEF_MOTOR *MOTOR);
uint8_t PID_F_P(TYPEDEF_MOTOR *MOTOR);
uint8_t PID_F_VISION_YAW(TYPEDEF_MOTOR *MOTOR);
uint8_t PID_F_VISION_PIT(TYPEDEF_MOTOR *MOTOR);
uint8_t PID_F_P_T(TYPEDEF_MOTOR *MOTOR);
void Feedforward_Init(Feedforward_t *ffc,float max_out,float *c,float lpf_rc,uint16_t ref_dot_ols_order,uint16_t ref_ddot_ols_order);
float Feedforward_Calculate(Feedforward_t *ffc, float ref);

extern float vision_aim;

#endif // !__PID_H__

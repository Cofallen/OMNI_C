#ifndef __PID_H__
#define __PID_H__

#include "MOTOR.h"

extern void PID_F_Init(TYPEDEF_MOTOR_PID *PID, const float *PID_V);
extern float PID_F_Cal(TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL);
// extern void PID_F_Clear(TYPEDEF_MOTOR *MOTOR);
extern uint8_t PID_F_AS(TYPEDEF_MOTOR *MOTOR);
extern uint8_t PID_F_SC(TYPEDEF_MOTOR *MOTOR);

extern uint8_t PID_F_S(TYPEDEF_MOTOR *MOTOR);
float PID_T_Cal(TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL, int8_t mode);
uint8_t PID_F_G(TYPEDEF_MOTOR *MOTOR);

#endif // !__PID_H__

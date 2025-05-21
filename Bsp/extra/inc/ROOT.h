#ifndef __ROOT_H__
#define __ROOT_H__

#include "DBUS.h"
#include "MOTOR.h"
#include "VT13.h"

extern uint8_t ROOT_V_MONITOR_DBUS;
extern TYPEDEF_MOTOR_PID FOLLOW_PID;
extern TYPEDEF_MOTOR_PID TOP_OFF_S, TOP_OFF_A;
extern TYPEDEF_MOTOR_PID VISION_PID_YAW_SPEED;
extern TYPEDEF_MOTOR_PID VISION_PID_YAW_ANGLE;
extern TYPEDEF_MOTOR_PID VISION_PID_PIT_SPEED;
extern TYPEDEF_MOTOR_PID VISION_PID_PIT_ANGLE;

extern void ROOT_F_MONITOR_DBUS(TYPEDEF_DBUS *DBUS);
extern uint8_t ROOT_F_PIDinit();
extern void ROOT_F_Init();
void ROOT_F_MONITOR_VT(VT13_Typedef *vt);

#endif // !__ROOT_H__

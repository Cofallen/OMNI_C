#ifndef __ROOT_H__
#define __ROOT_H__

#include "DBUS.h"
#include "MOTOR.h"

extern uint8_t ROOT_V_MONITOR_DBUS;
extern TYPEDEF_MOTOR_PID FOLLOW_PID;

extern void ROOT_F_MONITOR_DBUS(TYPEDEF_DBUS *DBUS);
extern uint8_t ROOT_F_PIDinit();
extern void ROOT_F_Init();

#endif // !__ROOT_H__

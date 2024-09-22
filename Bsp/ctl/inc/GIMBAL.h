#ifndef __GIMBAL_H__
#define __GIMBAL_H__

#include "MOTOR.h"
#include "DBUS.h"

extern void GIMBAL_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);

extern float yaw;
extern float yawInfinit;

#endif // !__GIMBAL_H__
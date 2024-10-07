#ifndef __GIMBAL_H__
#define __GIMBAL_H__

#include "MOTOR.h"
#include "DBUS.h"

extern void GIMBAL_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);
extern float DBUS_V_CH2[5];

#endif // !__GIMBAL_H__
#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "DBUS.h"
#include "MOTOR.h"

extern void CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);

extern float xx, yy;
extern double ANGLE_Rad, ANGLE_Relative;

#endif // !__CHASSIS_H__

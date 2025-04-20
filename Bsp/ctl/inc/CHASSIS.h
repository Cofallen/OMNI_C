#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "DBUS.h"
#include "MOTOR.h"

extern void CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);
static void CHASSIS_F_Lifited(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);

extern float xx, yy;
extern double ANGLE_Rad, ANGLE_Relative;

extern float watch[10];

static void CHASSIS_F_SoftStart(TYPEDEF_MOTOR *MOTOR);

#endif // !__CHASSIS_H__

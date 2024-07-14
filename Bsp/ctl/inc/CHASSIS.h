#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "DBUS.h"
#include "MOTOR.h"

extern int16_t CHASSIS_F_Ctl(TYPEDEF_MOTOR *MOTOR, TYPEDEF_DBUS *DBUS);
extern int32_t PowerLimBuffer_Cap(uint16_t maxPower , float buffer , int32_t setOut);
extern void AllotPower(int32_t setOut);
#endif // !__CHASSIS_H__

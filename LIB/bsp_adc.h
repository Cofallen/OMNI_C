#ifndef BSP_ADC_H
#define BSP_ADC_H
#include "main.h"

extern void init_vrefint_reciprocal(void);
extern float get_temprate(void);
extern float get_battery_voltage(void);
extern uint8_t get_hardware_version(void);
#endif

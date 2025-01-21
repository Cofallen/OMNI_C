#ifndef __CAP_H__
#define __CAP_H__

#include "main.h"

typedef struct TYPEDEF_CAP
{
    struct
    {
        uint8_t status;
        uint16_t capVolt;
    } capGetDate;
    
    struct
    {
        uint8_t status;
    } send;
    
}TYPEDEF_CAP;

extern TYPEDEF_CAP capData_t;

#endif // !__CAP_H__

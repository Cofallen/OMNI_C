#ifndef _ANONYMITY_H_
#define _ANONYMITY_H_

#include "stdint.h"
#include "dma.h"
#include "usart.h"
#include "Read_Data.h"
//#include "nan_superCap.h"

#define df_AnonymityLong 70
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

extern uint8_t anonymity_au8[df_AnonymityLong];												//匿名用数组
//匿名发送函数一共十个第十个不能用
void Anonymity_TX(int16_t a_x , int16_t a_y , int16_t a_z , int16_t g_x , int16_t g_y , int16_t g_z , int16_t m_x , int16_t m_y , int16_t m_z , int32_t bar);

void powerTX(void);

#endif

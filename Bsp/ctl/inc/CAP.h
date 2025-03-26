#ifndef __CAP_H__
#define __CAP_H__
#define df_SuperResidue capData_t.capGetDate.capVolt

#include "main.h"
struct capGetData_typdef	//接收的电容的数据
{
	float buffer; 		//电容缓冲
	float capVolt;		//电容电压
	float nowPower;		//当前总功率
	int16_t outBoll;	//当前电容是否在输出
	float   cap_realy_out;    //电容的净输出
};

union capSetData_typdef		//发给电容的数据//使用共用体整合数据
{
	struct
	{
		uint32_t residueBuff : 8;		//当前缓冲
		uint32_t maxPower : 8;		//最大功率
		uint32_t volt : 8;			//当前电池电压，因为最大电压大于25.5V，处理时加一个50
		uint32_t outPower : 8;			//当前总功率
		uint8_t  out_switch : 8;
		uint8_t  power_key : 8;
	}dataNeaten;
	//CAN发送的数据
	uint16_t sendData[3];
};

struct capData_typdef	//使用到的超级电容数据
{
	uint32_t outPutState : 1;		//放电状态
	uint32_t residueBuff : 8;		//当前缓冲	
};

struct capDate_typdef
{
	union capSetData_typdef capSetData;
	struct capGetData_typdef capGetDate;
};
extern struct capDate_typdef capData_t;

extern int16_t shu_temp72;
extern int16_t shu_temp2;	//电容开启关闭标志位

extern void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt);
extern void CanManage_cap(uint8_t* can_data , struct capGetData_typdef* data);
extern void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time);
#endif // !__CAP_H__


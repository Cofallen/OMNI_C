#include "CAP.h"
#include "main.h"
#include "can.h"

#include "CAP.h"
#include "CAN_DEV.h"
#include "read_data.h"
#include "define.h"
float  Inter=0;
int  temp_cap_time=0;
struct capDate_typdef capData_t ={ 0 };
/************************************************************万能分隔符**************************************************************
*	@author:			//赵澍
*	@performance:	//电容发送初始化
*	@parameter:		//最大功率//放电标志位//当前缓冲//当前功率//电池电压
*	@ReadMe:			//每次执行电容发送之前都需要调用一次
************************************************************************************************************************************/
float shu_temp75 = 0.0f;
void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt)
{
		//减三是因为裁判系统自己耗电3W左右
	shu_temp75 = 250 * ((float) powerBuffer/60.0f);
	shu_temp75 = SectionLimit_f(200.0f , 45.0f , shu_temp75);

//	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) user_data.robot_status.chassis_power_limit; //		//最大功率
	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) 60; //		//最大功率
	capData_t.capSetData.dataNeaten.residueBuff = powerBuffer;	//	当前缓冲
	capData_t.capSetData.dataNeaten.outPower = nowPower;	//当前功率
	capData_t.capSetData.dataNeaten.volt = volt - 50;		//电池电压
	capData_t.capSetData.dataNeaten.power_key = 1;
	capData_t.capSetData.dataNeaten.out_switch =1;//C键作为电容强制开关，上锁模式。默认离线，按一下打开，
	//超级电容发送函数
	CAN_F_Send(&hcan1 , 0x1ff , capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , \
													capData_t.capSetData.sendData[2] , capData_t.capSetData.sendData[1] );
}

/************************************************************万能分隔符**************************************************************
*	@author:			//赵澍
*	@performance:	//电容CAN接收处理函数
*	@parameter:		///can_data:CAN接收数组。data：超级电容接收结构体
*	@time:				//22-03-06
*	@ReadMe:			//
************************************************************************************************************************************/
void CanManage_cap(uint8_t* can_data , struct capGetData_typdef* data)
{
	int16_t nowpower=0;
	//root_t.superCapRoot.time = 0;	//电容离线重置
	capData_t.capGetDate.capVolt = can_data[0]<<8 | can_data[1];
	capData_t.capGetDate.capVolt=capData_t.capGetDate.capVolt*0.1f;
	nowpower=can_data[2]<<8 | can_data[3];
	capData_t.capGetDate.nowPower = nowpower;
	capData_t.capGetDate.nowPower *= 0.01f;
	capData_t.capGetDate.outBoll=can_data[6]<<8 | can_data[7];
	capData_t.capGetDate.cap_realy_out=capData_t.capGetDate.nowPower-user_data.power_heat_data.chassis_power;

}
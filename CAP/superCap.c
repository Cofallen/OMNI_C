#include "superCap.h"
#include "user_define.h"		//用于操作手自定义数据
#include "main.h"
#include "CAN_DEV.h"
#include "can.h"
#include "shu_math.h"
#include "Read_Data.h"
#include "CHASSIS.h"
struct capDate_typdef capData_t ={ 0 };

#ifdef RUI_CAP_FUHANG
float  Inter=0;
int  temp_cap_time=0;
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
//	if (CanCommunit_t.gmTOch.dataNeaten.chMod == CHMODLock)
//	{
//		shu_temp75 = 1;
//	}
	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) user_data.robot_status.chassis_power_limit;		//最大功率
	capData_t.capSetData.dataNeaten.residueBuff = powerBuffer;	//	当前缓冲
	capData_t.capSetData.dataNeaten.outPower = nowPower;	//当前功率
	capData_t.capSetData.dataNeaten.volt = volt - 50;		//电池电压
	capData_t.capSetData.dataNeaten.power_key = 1;
	capData_t.capSetData.dataNeaten.out_switch =DBUS_V_UNION.DATA_NEATEN.KEY_C;//C键作为电容强制开关，上锁模式。默认离线，按一下打开，
	//超级电容发送函数
	CAN_F_Send(&hcan2 , 0x204 , capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , \
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
	capData_t.capGetDate.cap_realy_out=capData_t.capGetDate.nowPower-user_data.robot_status.chassis_power_limit;

}
/************************************************************万能分隔符**************************************************************
*	@author:			//赵澍
*	@performance:	//电容进/出调整函数
*	@parameter:		//电容开关模式//总输出//时间轴
*	@time:				//22-04-07
*	@ReadMe:			//内部调用功率限制函数
								//对电容的退出加了一个斜坡
************************************************************************************************************************************/
int16_t shu_temp72 = 0;
int16_t shu_temp2 = 0;	//电容开启关闭标志位

void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time)
{

	//  //*可编辑部分*begin*//
	const uint16_t SuperMaxPower = user_SuperCapPower;  //超级电容下的功率

//	int i_time;

	const uint64_t outTime_1 = 600;  //超级电容进入时的斜坡长度//单位毫秒
	const uint64_t outTime_2= 300;  //超级电容退出时的斜坡长度//单位毫秒
	const uint16_t capValt = 7;   //制退出的电压阈值
//*	const uint16_t capValt_2 = 19;   //制退出的电压阈值
//*	const uint16_t capValt_3 = 13;   //制退出的电压阈值
  //*可编辑部分*end*//

	static uint64_t lastTime_1 = 0;
	static uint64_t lastTime_2 = 0;
	float raid_1 = 0.0f;//超级电容进入时的比例系数
	float raid_2 = 0.0f;//超级电容退出时的比例系数
//*	float raid_3=0.1;
	uint16_t slope_out_1 = 0;  //电容进入斜坡当前值
	uint16_t slope_out_2 = 0;  //电容退出斜坡当前值
	uint16_t slope_err = SuperMaxPower - user_data.robot_status.chassis_power_limit;		//斜坡功率总值//
//	mod=remote_t.key_board.ctrl;

	if (capData_t.capGetDate.capVolt <= capValt)
	{
		mod = 0;	//电压过低强制退出
	}


	if (mod==1&&capData_t.capSetData.dataNeaten.power_key==1&&capData_t.capSetData.dataNeaten.out_switch==1)
	{  //超级电容模式开启   

		temp_cap_time=lastTime_2;

		raid_1=  SectionLimit_f(1.0f , 0.0f , ((float) (time - lastTime_2)/(float) outTime_1)); //斜坡比例

		if (capData_t.capGetDate.capVolt>21)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*5.8f+user_data.robot_status.chassis_power_limit);
		if (capData_t.capGetDate.capVolt<=21&&capData_t.capGetDate.capVolt>18)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*5.3f+user_data.robot_status.chassis_power_limit);
		if (capData_t.capGetDate.capVolt<=18&&capData_t.capGetDate.capVolt>=12)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*5+user_data.robot_status.chassis_power_limit);
		if (capData_t.capGetDate.capVolt<12)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*4.4f+user_data.robot_status.chassis_power_limit);

		PowerLimBuffer_Cap(Inter , capData_t.capGetDate.buffer , setOut);
		lastTime_1 = time; //更新时间轴


	}
	if (mod == 0||capData_t.capSetData.dataNeaten.out_switch==0)
	{  //超级电容模式退出

		raid_2 =  SectionLimit_f(1.0f , 0.0f , (1-(float) (time - lastTime_1)/(float) outTime_2)); //斜坡比例
		slope_out_2 = raid_1 * slope_err;  //得到斜坡功率当前值
		shu_temp72 = slope_out_1;
		PowerLimBuffer_Cap(user_data.robot_status.chassis_power_limit , capData_t.capGetDate.buffer , setOut);

		lastTime_2 = time; //更新时间轴
	}

}

#endif


/************************************************************万能分隔符**************************************************************
*	@author:			//赵澍
*	@performance:	//电容发送初始化
*	@parameter:		//最大功率//放电标志位//当前缓冲//当前功率//电池电压
*	@time:				//22-03-06
*	@ReadMe:			//每次执行电容发送之前都需要调用一次
************************************************************************************************************************************/
#ifdef RUI_CAP_YUNXIANG
float shu_temp75 = 0.0f;
void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt)
{
		//减三是因为裁判系统自己耗电3W左右
	shu_temp75 = 250 * ((float) powerBuffer/60.0f);
	shu_temp75 = SectionLimit_f(200.0f , 45.0f , shu_temp75);
	if (CanCommunit_t.gmTOch.dataNeaten.chMod == CHMODLock)
	{
		shu_temp75 = 1;
	}
	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) shu_temp75;		//最大功率
	capData_t.capSetData.dataNeaten.residueBuff = powerBuffer;	//	当前缓冲
	capData_t.capSetData.dataNeaten.outPower = nowPower;	//当前功率
	capData_t.capSetData.dataNeaten.volt = volt - 50;		//电池电压
	//超级电容发送函数
	my_CAN_Send(CAN2 , 0x1ff , capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , \
													capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , 8);
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
	root_t.superCapRoot.time = 0;	//电容离线重置
	capData_t.capGetDate.capVolt = can_data[0]<<8 | can_data[1];
	capData_t.capGetDate.nowPower = can_data[2]<<8 | can_data[3];
	capData_t.capGetDate.nowPower *= 0.01f;
}
/************************************************************万能分隔符**************************************************************
*	@author:			//赵澍
*	@performance:	//电容进/出调整函数
*	@parameter:		//电容开关模式//总输出//时间轴
*	@time:				//22-04-07
*	@ReadMe:			//内部调用功率限制函数
								//对电容的退出加了一个斜坡
************************************************************************************************************************************/
int16_t shu_temp72 = 0;
int16_t shu_temp2 = 0;	//电容开启关闭标志位
void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time)
{
		//*可编辑部分*begin*//
	const uint16_t SuperMaxPower = user_SuperCapPower;		//超级电容下的功率
	const uint64_t outTime = 1000;		//超级电容退出时的斜坡长度//单位毫秒
	const uint16_t capValt = 140;			//强制退出的电压阈值
	//*可编辑部分*end*//

	static uint64_t lastTime = 0;
	float raid = 0.0f;
	uint16_t slope_out = 0;		//斜坡当前值
	uint16_t slope_err = SuperMaxPower - twoJudgeData.power.maxPower;		//斜坡功率总值

	//是否允许进入电容模式
	if (capData_t.capGetDate.capVolt <= capValt)
	{
		mod = 0;	//电压过低强制退出
	}

	if (mod)
	{		//超级电容模式开启
		PowerLimIntegral(SuperMaxPower , capData_t.capGetDate.nowPower , setOut);
		lastTime = time;	//更新时间轴
	}
	else if (mod == 0)
	{		//超级电容模式退出
		raid = 	SectionLimit_f(1.0f , 0.0f , (1-(float) (time - lastTime)/(float) outTime));	//斜坡比例
		slope_out = raid * slope_err;		//得到斜坡功率当前值
		shu_temp72 = slope_out;
		PowerLimBuffer_Cap(twoJudgeData.power.maxPower+slope_out , capData_t.capGetDate.buffer , setOut);
	}
}


#endif




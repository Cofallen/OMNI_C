#include "CAP.h"
#include "main.h"
#include "can.h"

#include "CAP.h"
#include "CAN_DEV.h"
#include "read_data.h"
#include "define.h"
#include "YU_MATH.h"
#include "chassis_power_control.h"
#include "DBUS.h"

float  Inter=0;
int  temp_cap_time=0;
struct capDate_typdef capData_t ={ 0 };
struct capData_JHB capData_JHB = { 0 };
/************************************************************���ָܷ���**************************************************************
*	@author:			//����
*	@performance:	//���ݷ��ͳ�ʼ��
*	@parameter:		//�����//�ŵ��־λ//��ǰ����//��ǰ����//��ص�ѹ
*	@ReadMe:			//ÿ��ִ�е��ݷ���֮ǰ����Ҫ����һ��
************************************************************************************************************************************/
float shu_temp75 = 0.0f;
void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt)
{
		//��������Ϊ����ϵͳ�Լ��ĵ�3W����
	shu_temp75 = 250 * ((float) powerBuffer/60.0f);
	shu_temp75 = SectionLimit_f(200.0f , 45.0f , shu_temp75);

//	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) user_data.robot_status.chassis_power_limit; //		//�����
	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) 60; //		//�����
	capData_t.capSetData.dataNeaten.residueBuff = powerBuffer;	//	��ǰ����
	capData_t.capSetData.dataNeaten.outPower = nowPower;	//��ǰ����
	capData_t.capSetData.dataNeaten.volt = volt - 50;		//��ص�ѹ
	capData_t.capSetData.dataNeaten.power_key = 1;
	capData_t.capSetData.dataNeaten.out_switch =1;//C����Ϊ����ǿ�ƿ��أ�����ģʽ��Ĭ�����ߣ���һ�´򿪣�
	//�������ݷ��ͺ���
	CAN_F_Send(&hcan1 , 0x1ff , capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , \
													capData_t.capSetData.sendData[2] , capData_t.capSetData.sendData[1] );
}

/************************************************************���ָܷ���**************************************************************
*	@author:			//����
*	@performance:	//����CAN���մ�������
*	@parameter:		///can_data:CAN�������顣data���������ݽ��սṹ��
*	@time:				//22-03-06
*	@ReadMe:			//
************************************************************************************************************************************/
void CanManage_cap(uint8_t* can_data , struct capGetData_typdef* data)
{
	int16_t nowpower=0;
	//root_t.superCapRoot.time = 0;	//������������
	capData_t.capGetDate.capVolt = can_data[0]<<8 | can_data[1];
	capData_t.capGetDate.capVolt=capData_t.capGetDate.capVolt*0.1f;
	nowpower=can_data[2]<<8 | can_data[3];
	capData_t.capGetDate.nowPower = nowpower;
	capData_t.capGetDate.nowPower *= 0.01f;
	capData_t.capGetDate.outBoll=can_data[6]<<8 | can_data[7];
	capData_t.capGetDate.cap_realy_out=capData_t.capGetDate.nowPower-user_data.power_heat_data.chassis_power;

}

void CanManage_cap_new(uint8_t* can_data , struct capData_JHB_Receive* data)
{
	int16_t nowpower=0;
	//root_t.superCapRoot.time = 0;	//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	capData_JHB.Receive_data_typedef.switchStatus = can_data[0];
	capData_JHB.Receive_data_typedef.capStatus = can_data[1];
	capData_JHB.Receive_data_typedef.wireVolt = (uint16_t)(can_data[2]<<8 | can_data[3])/100.0f;
	capData_JHB.Receive_data_typedef.Power = (uint16_t)(can_data[4]<<8 | can_data[5])/100.0f;
	capData_JHB.Receive_data_typedef.capVolt = (uint16_t)(can_data[6]<<8 | can_data[7])/100.0f;
	
}
void CapSend_new(uint8_t powerBuffer , uint16_t volt)
{
	static uint8_t cap_open = 1;
	if (DBUS_V_DATA.KEY_BOARD.X && !DBUS_V_DATA.KEY_BOARD.X_PREE_NUMBER) {
		cap_open = !cap_open;  
	}
	capData_JHB.Send_data_typedef.Send_data.powerLimit = (uint32_t)user_data.robot_status.chassis_power_limit;	//锟斤拷锟斤拷锟?
		// capData_JHB.Send_data_typedef.Send_data.powerLimit = 55;
	capData_JHB.Send_data_typedef.Send_data.robotStatus = (user_data.robot_status.current_HP > 0) ? 1 : 0;
	capData_JHB.Send_data_typedef.Send_data.switchControl = (DBUS_V_DATA.REMOTE.S2_u8 == 3) ? ( user_data.robot_status.current_HP > 0 && cap_open): 0;
	capData_JHB.Send_data_typedef.Send_data.bufferEnergy = (uint32_t) powerBuffer;		
	capData_JHB.Send_data_typedef.Send_data.Verify = 0xAA;//标志位仅供人参考，不做校验
	CAN_F_Send(&hcan1 , 0x252 , capData_JHB.Send_data_typedef.Data[0] , capData_JHB.Send_data_typedef.Data[1], \
		capData_JHB.Send_data_typedef.Data[2], 0 );	
	DBUS_V_DATA.KEY_BOARD.X_PREE_NUMBER = DBUS_V_DATA.KEY_BOARD.X;
}

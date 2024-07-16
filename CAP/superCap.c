#include "superCap.h"
#include "user_define.h"		//���ڲ������Զ�������
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
//	if (CanCommunit_t.gmTOch.dataNeaten.chMod == CHMODLock)
//	{
//		shu_temp75 = 1;
//	}
	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) user_data.robot_status.chassis_power_limit;		//�����
	capData_t.capSetData.dataNeaten.residueBuff = powerBuffer;	//	��ǰ����
	capData_t.capSetData.dataNeaten.outPower = nowPower;	//��ǰ����
	capData_t.capSetData.dataNeaten.volt = volt - 50;		//��ص�ѹ
	capData_t.capSetData.dataNeaten.power_key = 1;
	capData_t.capSetData.dataNeaten.out_switch =DBUS_V_UNION.DATA_NEATEN.KEY_C;//C����Ϊ����ǿ�ƿ��أ�����ģʽ��Ĭ�����ߣ���һ�´򿪣�
	//�������ݷ��ͺ���
	CAN_F_Send(&hcan2 , 0x204 , capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , \
													capData_t.capSetData.sendData[2] , capData_t.capSetData.sendData[1] );
}

/************************************************************���ָܷ���**************************************************************
*	@author:			//����
*	@performance:	//����CAN���մ�����
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
	capData_t.capGetDate.cap_realy_out=capData_t.capGetDate.nowPower-user_data.robot_status.chassis_power_limit;

}
/************************************************************���ָܷ���**************************************************************
*	@author:			//����
*	@performance:	//���ݽ�/����������
*	@parameter:		//���ݿ���ģʽ//�����//ʱ����
*	@time:				//22-04-07
*	@ReadMe:			//�ڲ����ù������ƺ���
								//�Ե��ݵ��˳�����һ��б��
************************************************************************************************************************************/
int16_t shu_temp72 = 0;
int16_t shu_temp2 = 0;	//���ݿ����رձ�־λ

void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time)
{

	//  //*�ɱ༭����*begin*//
	const uint16_t SuperMaxPower = user_SuperCapPower;  //���������µĹ���

//	int i_time;

	const uint64_t outTime_1 = 600;  //�������ݽ���ʱ��б�³���//��λ����
	const uint64_t outTime_2= 300;  //���������˳�ʱ��б�³���//��λ����
	const uint16_t capValt = 7;   //���˳��ĵ�ѹ��ֵ
//*	const uint16_t capValt_2 = 19;   //���˳��ĵ�ѹ��ֵ
//*	const uint16_t capValt_3 = 13;   //���˳��ĵ�ѹ��ֵ
  //*�ɱ༭����*end*//

	static uint64_t lastTime_1 = 0;
	static uint64_t lastTime_2 = 0;
	float raid_1 = 0.0f;//�������ݽ���ʱ�ı���ϵ��
	float raid_2 = 0.0f;//���������˳�ʱ�ı���ϵ��
//*	float raid_3=0.1;
	uint16_t slope_out_1 = 0;  //���ݽ���б�µ�ǰֵ
	uint16_t slope_out_2 = 0;  //�����˳�б�µ�ǰֵ
	uint16_t slope_err = SuperMaxPower - user_data.robot_status.chassis_power_limit;		//б�¹�����ֵ//
//	mod=remote_t.key_board.ctrl;

	if (capData_t.capGetDate.capVolt <= capValt)
	{
		mod = 0;	//��ѹ����ǿ���˳�
	}


	if (mod==1&&capData_t.capSetData.dataNeaten.power_key==1&&capData_t.capSetData.dataNeaten.out_switch==1)
	{  //��������ģʽ����   

		temp_cap_time=lastTime_2;

		raid_1=  SectionLimit_f(1.0f , 0.0f , ((float) (time - lastTime_2)/(float) outTime_1)); //б�±���

		if (capData_t.capGetDate.capVolt>21)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*5.8f+user_data.robot_status.chassis_power_limit);
		if (capData_t.capGetDate.capVolt<=21&&capData_t.capGetDate.capVolt>18)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*5.3f+user_data.robot_status.chassis_power_limit);
		if (capData_t.capGetDate.capVolt<=18&&capData_t.capGetDate.capVolt>=12)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*5+user_data.robot_status.chassis_power_limit);
		if (capData_t.capGetDate.capVolt<12)
			Inter=raid_1*((capData_t.capGetDate.capVolt)*4.4f+user_data.robot_status.chassis_power_limit);

		PowerLimBuffer_Cap(Inter , capData_t.capGetDate.buffer , setOut);
		lastTime_1 = time; //����ʱ����


	}
	if (mod == 0||capData_t.capSetData.dataNeaten.out_switch==0)
	{  //��������ģʽ�˳�

		raid_2 =  SectionLimit_f(1.0f , 0.0f , (1-(float) (time - lastTime_1)/(float) outTime_2)); //б�±���
		slope_out_2 = raid_1 * slope_err;  //�õ�б�¹��ʵ�ǰֵ
		shu_temp72 = slope_out_1;
		PowerLimBuffer_Cap(user_data.robot_status.chassis_power_limit , capData_t.capGetDate.buffer , setOut);

		lastTime_2 = time; //����ʱ����
	}

}

#endif


/************************************************************���ָܷ���**************************************************************
*	@author:			//����
*	@performance:	//���ݷ��ͳ�ʼ��
*	@parameter:		//�����//�ŵ��־λ//��ǰ����//��ǰ����//��ص�ѹ
*	@time:				//22-03-06
*	@ReadMe:			//ÿ��ִ�е��ݷ���֮ǰ����Ҫ����һ��
************************************************************************************************************************************/
#ifdef RUI_CAP_YUNXIANG
float shu_temp75 = 0.0f;
void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt)
{
		//��������Ϊ����ϵͳ�Լ��ĵ�3W����
	shu_temp75 = 250 * ((float) powerBuffer/60.0f);
	shu_temp75 = SectionLimit_f(200.0f , 45.0f , shu_temp75);
	if (CanCommunit_t.gmTOch.dataNeaten.chMod == CHMODLock)
	{
		shu_temp75 = 1;
	}
	capData_t.capSetData.dataNeaten.maxPower = (uint32_t) shu_temp75;		//�����
	capData_t.capSetData.dataNeaten.residueBuff = powerBuffer;	//	��ǰ����
	capData_t.capSetData.dataNeaten.outPower = nowPower;	//��ǰ����
	capData_t.capSetData.dataNeaten.volt = volt - 50;		//��ص�ѹ
	//�������ݷ��ͺ���
	my_CAN_Send(CAN2 , 0x1ff , capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , \
													capData_t.capSetData.sendData[0] , capData_t.capSetData.sendData[1] , 8);
}

/************************************************************���ָܷ���**************************************************************
*	@author:			//����
*	@performance:	//����CAN���մ�����
*	@parameter:		///can_data:CAN�������顣data���������ݽ��սṹ��
*	@time:				//22-03-06
*	@ReadMe:			//
************************************************************************************************************************************/
void CanManage_cap(uint8_t* can_data , struct capGetData_typdef* data)
{
	root_t.superCapRoot.time = 0;	//������������
	capData_t.capGetDate.capVolt = can_data[0]<<8 | can_data[1];
	capData_t.capGetDate.nowPower = can_data[2]<<8 | can_data[3];
	capData_t.capGetDate.nowPower *= 0.01f;
}
/************************************************************���ָܷ���**************************************************************
*	@author:			//����
*	@performance:	//���ݽ�/����������
*	@parameter:		//���ݿ���ģʽ//�����//ʱ����
*	@time:				//22-04-07
*	@ReadMe:			//�ڲ����ù������ƺ���
								//�Ե��ݵ��˳�����һ��б��
************************************************************************************************************************************/
int16_t shu_temp72 = 0;
int16_t shu_temp2 = 0;	//���ݿ����رձ�־λ
void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time)
{
		//*�ɱ༭����*begin*//
	const uint16_t SuperMaxPower = user_SuperCapPower;		//���������µĹ���
	const uint64_t outTime = 1000;		//���������˳�ʱ��б�³���//��λ����
	const uint16_t capValt = 140;			//ǿ���˳��ĵ�ѹ��ֵ
	//*�ɱ༭����*end*//

	static uint64_t lastTime = 0;
	float raid = 0.0f;
	uint16_t slope_out = 0;		//б�µ�ǰֵ
	uint16_t slope_err = SuperMaxPower - twoJudgeData.power.maxPower;		//б�¹�����ֵ

	//�Ƿ�����������ģʽ
	if (capData_t.capGetDate.capVolt <= capValt)
	{
		mod = 0;	//��ѹ����ǿ���˳�
	}

	if (mod)
	{		//��������ģʽ����
		PowerLimIntegral(SuperMaxPower , capData_t.capGetDate.nowPower , setOut);
		lastTime = time;	//����ʱ����
	}
	else if (mod == 0)
	{		//��������ģʽ�˳�
		raid = 	SectionLimit_f(1.0f , 0.0f , (1-(float) (time - lastTime)/(float) outTime));	//б�±���
		slope_out = raid * slope_err;		//�õ�б�¹��ʵ�ǰֵ
		shu_temp72 = slope_out;
		PowerLimBuffer_Cap(twoJudgeData.power.maxPower+slope_out , capData_t.capGetDate.buffer , setOut);
	}
}


#endif




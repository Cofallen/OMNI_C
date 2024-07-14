#ifndef SUPERCAP__H
#define SUPERCAP__H
#include "chassis.h"
#include "stdint.h"
#include "judgeSystem.h"
#include "CAN_receive.h"
#include "switch.h"
#define df_SuperResidue capData_t.capGetDate.capVolt

struct capGetData_typdef	//���յĵ��ݵ�����
{
	float buffer; 		//���ݻ���
	float capVolt;		//���ݵ�ѹ
	float nowPower;		//��ǰ�ܹ���
	int16_t outBoll;	//��ǰ�����Ƿ������
	float   cap_realy_out;    //���ݵľ����
};

union capSetData_typdef		//�������ݵ�����//ʹ�ù�������������
{
	struct
	{
		uint32_t residueBuff : 8;		//��ǰ����
		uint32_t maxPower : 8;		//�����
		uint32_t volt : 8;			//��ǰ��ص�ѹ����Ϊ����ѹ����25.5V������ʱ��һ��50
		uint32_t outPower : 8;			//��ǰ�ܹ���
		uint8_t  out_switch : 8;
		uint8_t  power_key : 8;
	}dataNeaten;
	//CAN���͵�����
	uint16_t sendData[3];
};

struct capData_typdef	//ʹ�õ��ĳ�����������
{
	uint32_t outPutState : 1;		//�ŵ�״̬
	uint32_t residueBuff : 8;		//��ǰ����	
};

struct capDate_typdef
{
	union capSetData_typdef capSetData;
	struct capGetData_typdef capGetDate;
};
extern struct capDate_typdef capData_t;

extern int16_t shu_temp72;
extern int16_t shu_temp2;	//���ݿ����رձ�־λ
extern struct capDate_typdef capData_t;
#ifdef  RUI_CAP_YUNXIANG
//�������ݷ��ͳ�ʼ��
void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt);
//��������CAN���մ�����d:\download\TIM\superCap.c
void CanManage_cap(uint8_t* can_data , struct capGetData_typdef* data);
//������/�����
void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time);
#endif

#ifdef  RUI_CAP_FUHANG

void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt);
void CanManage_cap(uint8_t* can_data , struct capGetData_typdef* data);//��������CAN���մ�����
void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time);   //������/�����
int32_t PowerLimIntegral_Cap(uint16_t maxPower , float nowPower , int32_t setOut);
int32_t PowerLimBuffer_Cap_Cap(uint16_t maxPower , float buffer , int32_t setOut);
#endif
#endif



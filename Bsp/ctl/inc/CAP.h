#ifndef __CAP_H__
#define __CAP_H__
#define df_SuperResidue capData_t.capGetDate.capVolt

#include "main.h"
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

extern void CapSendInit(uint8_t powerBuffer , uint8_t nowPower , uint16_t volt);
extern void CanManage_cap(uint8_t* can_data , struct capGetData_typdef* data);
extern void SuperCapTransation(uint8_t mod , int32_t setOut , uint64_t time);
#endif // !__CAP_H__


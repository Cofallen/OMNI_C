#ifndef JUDGESYSTEM__H
#define JUDGESYSTEM__H

#include "stdint.h"
#include "usart.h"

extern uint8_t data_au8[255];

#define X_CENTER 960
#define Y_CENTER 540

//������
#define BLUE 0
#define RED  1

#define JudgeBufferLength       255
#define JudgeFrameHeader        0xA5        //֡ͷ 
#define FrameHeader_Len         5		//֡ͷУ�鳤��
#define client_custom_Length    28
extern uint8_t JudgeDataBuffer[JudgeBufferLength];
extern uint8_t JudgeDataBuffer_Send[JudgeBufferLength];
extern uint8_t client_custom_Send[client_custom_Length];
extern uint8_t robot_custom_Send[client_custom_Length];
/*__packed  c���Թؼ��֣��������ṹ���е����ݾͲ����׵�ַ����
����˵���������ݻᱻѹ��
1.ͨ�������������ɴ����ʱ�򶼻���нṹ�����䣬��֤���ṹ���ڲ���Ա��������ܵĶ��뷽ʽ
2.�������Զ���������ṹ����ڴ棨���綨��Ϊȫ�ֱ�����ֲ��������϶��Ƕ����
*/
typedef enum
{
  RobotStatus_ID                   =0x0201 ,	//������״̬  �ȼ�
  PowerHeat_ID   	          			=0x0202 ,	//ǹ������ ���̹���
  RobotHurt_ID                    =0x0206 ,	//�˺�����
  ShootState_ID	      					  =0x0207 ,	//��Ƶ����
} Judege_Cmd_ID;
//֡ͷ
typedef __packed struct
{
  uint8_t   SOF;				//0xA5
  uint16_t  DataLenth;	//����λ����
  uint8_t   Seq;				//�����
  uint8_t   CRC8;				//crc8λУ��
}tFrameHeader;
/*�˺�����*/
typedef __packed struct
{
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
} ext_robot_hurt_t;
/*����������״̬*/
typedef __packed struct
{
  uint8_t robot_id;			//������ID
  uint8_t robot_level;	//�����˵ȼ�
  uint16_t remain_HP;		//������ʣ��HP
  uint16_t max_HP;			//����������Ѫ��
  uint16_t shooter_id1_17mm_cooling_rate;		//1��17������ȴ
  uint16_t shooter_id1_17mm_cooling_limit;	//1��17������������
  uint16_t shooter_id1_17mm_speed_limit;		//1��17����ǹ����������

  uint16_t shooter_id2_17mm_cooling_rate;		//2��17������ȴ
  uint16_t shooter_id2_17mm_cooling_limit;	//2��17������������		
  uint16_t shooter_id2_17mm_speed_limit;		//2��17����ǹ����������

  uint16_t shooter_id1_42mm_cooling_rate;		//42����ǹ��û����ȴֵ
  uint16_t shooter_id1_42mm_cooling_limit;	//42����ǹ����������
  uint16_t shooter_id1_42mm_speed_limit;		//42������������

  uint16_t chassis_power_limit;							//�����˵��̹�������
  uint8_t mains_power_gimbal_output : 1;
  uint8_t mains_power_chassis_output : 1;
  uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;
/*���̹��ʼ�ǹ������*/
typedef __packed struct
{
  uint16_t chassis_volt;					//���������ѹ  ��λ ����
  uint16_t chassis_current;				//�����������  ��λ����
  float chassis_power;						//�����������  ��λ W
  uint16_t chassis_power_buffer;	//���̹��ʻ���  ��λJ
  uint16_t shooter_id1_17mm_cooling_heat;		//1��17����ǹ������
  uint16_t shooter_id2_17mm_cooling_heat;		//2��17����ǹ������
  uint16_t shooter_id1_42mm_cooling_heat;		//42����ǹ������
} ext_power_heat_data_t;
/*�ӵ���Ϣ*/
typedef __packed struct
{
  uint8_t bullet_type;		//�ӵ�����//1С//2��
  uint8_t shooter_id;			//����ID 1��2��Ӧ1��2�������.3��Ӧ42���׻���
  uint8_t bullet_freq;		//��Ƶ
  float bullet_speed;			//����
} ext_shoot_data_t;


//////////����ϵͳ��������//////////
typedef  struct
{
  tFrameHeader          frameHeader;//֡ͷ
  uint16_t              rxCmdId;//������
  ext_power_heat_data_t power_heat_data_t;//���̹���ǹ����������
  ext_shoot_data_t		  shoot_data_t;//�����Ϣ
  ext_robot_hurt_t      robot_hurt_t;//�˺�����
  ext_game_robot_state_t game_robot_state_t;//�����˻�����Ϣ
}JudgementDataTypedef;
extern JudgementDataTypedef JudgementData;

//��Ҫ�ĺ�������
uint8_t Get_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength , uint8_t ucCRC8);
uint32_t Verify_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength);
void Append_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength , uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength);
void judgeCalculate(uint8_t* JudgeDataBuffer);

//���δ���ĺ�������
#define RedColor 	1		//��ɫ��
#define BuleColor	2		//��ɫ��
#define HoreRobot 1				//Ӣ�ۻ�����	
#define RescueRobot 2			//����
#define InfantryRobot 3		//����
#define AirRobot 6				//����
#define SentryRobot 7			//�ڱ�
#define DartsRobot 8			//����	
#define RadarRobot 9			//�״�

void Manage_RobotHurt(void);
void Manage_PowerHeat(void);
void Manage_ShootState(void);
void Manage_RobotStatus(void);

//���δ���Ĳ���ϵͳ����
struct twoJudgeData_typedef
{
  //�ȼ� Ѫ�� ����״̬
  struct
  {
    uint8_t mod : 6; 	//��������
    uint8_t color : 2;//��ɫ
    int8_t level;		//�ȼ�	
    int16_t hp;			//��ǰѪ��
    int16_t maxHp;	//Ѫ������
  }RoboSate;
  //�˺���Դ
  struct
  {
    uint8_t ArmorFront : 1;		//ǰװ�װ�
    uint8_t ArmorRight : 1;		//��װ�װ�
    uint8_t ArmorBack : 1;		//��װ�װ�
    uint8_t ArmorLeft : 1;		//��װ�װ�		
    uint8_t ExceedPower : 1;	//�����ʿ�Ѫ
    uint8_t ExceedHead : 1;		//��������Ѫ
    uint8_t ExceedSpeed : 1;	//�����ٿ�Ѫ
    uint8_t Mods_OffLine : 1;	//ģ�����߿�Ѫ	
  }HurtSource;
  //���� ���� ���
  struct
  {
    float nowPower;				//��ǰ����
    uint16_t maxPower;		//�����
    uint16_t buffer;			//ʣ�໺��
    uint16_t maxBuffer;		//��󻺳�
    uint16_t volt;				//��ص�ѹ
    float	bufferProportion;		// ʣ�໺��/��󻺳�
  }power;

  //������ǹ����Ϣ
  struct
  {
    float shootSpeed;				//����
    uint16_t maxShootSpeed;	//�������
    uint16_t maxCooling;		//���ǹ������
    uint16_t nowCooling;		//��ǰǹ������
    int8_t coolingRate;			//ǹ����ȴ
    int8_t residueNumber;		//ʣ����Ի���ĵ�������	
  }muzzle;
};
extern struct twoJudgeData_typedef twoJudgeData;

/**********************************����ϵͳ����end******************************/



/*********************************����ϵͳ����begin*****************************/

typedef __packed struct
{
  uint16_t data_id;		//�����ʶ��
  uint16_t send_ID; //������
  uint16_t receive_ID;//������
}tdata;

//��ر����Ľṹ��
typedef __packed struct
{
  uint8_t graphic_name[3];//����
  uint32_t operate_tpye : 3;//
  uint32_t graphic_tpye : 3;
  uint32_t layer : 4;//ͼ��
  uint32_t color : 4;//��ɫ
  uint32_t start_angle : 9;//��Բ
  uint32_t end_angle : 9;
  uint32_t width : 10;//�������
  uint32_t start_x : 11;
  uint32_t start_y : 11;
  uint32_t radius : 10;
  uint32_t end_x : 11;
  uint32_t end_y : 11;
}ext_graphic_data_struct_t;

typedef  __packed struct
{
  uint8_t operate_tpye;
  uint8_t layer;
}ext_client_custom_graphic_delete_t;

typedef __packed struct
{
  ext_graphic_data_struct_t		graphic_data_struct_t[7];
}ext_uixianduan;

typedef __packed struct
{
  ext_graphic_data_struct_t		fu[7];
}ext_dianrong;

typedef __packed struct
{
  ext_graphic_data_struct_t		graphic_data_struct_t;
  char   shuju[30];
}ext_client_custom_character_t;

typedef __packed struct
{
  ext_graphic_data_struct_t		graphic_data_struct_t;
  char   shuju[30];
}ext_client_custom_shuzi_t;

//////////����ϵͳ������������//////////
typedef __packed struct
{
  tFrameHeader          frameHeader;//֡ͷ
  uint16_t              CmdId;//������
  tdata 								data;//���͹̶�����
  ext_uixianduan				uixianduan;
  uint16_t              CRC16;
}hypTypedef;
extern hypTypedef hyp;//�ߵĻ���

typedef __packed struct
{
  tFrameHeader          					frameHeader;//֡ͷ
  uint16_t              					CmdId;//������
  tdata 													data;
  ext_client_custom_character_t 	zifu;
  uint16_t              					CRC16;
}ziTypedef;
extern ziTypedef zi;//����

typedef __packed struct
{
  tFrameHeader         					  frameHeader;//֡ͷ
  uint16_t              				  CmdId;//������
  tdata 													data;//�������ݹ̶�
  ext_dianrong 										dong;
  uint16_t                        CRC16;
}dianrongTypedef;
extern dianrongTypedef dianrong;//���ݵ���

typedef __packed struct
{
  tFrameHeader         					  frameHeader;//֡ͷ
  uint16_t              				  CmdId;//������
  tdata 													data;//�������ݹ̶�
  ext_client_custom_character_t 	zifu;
  uint16_t                        CRC16;
}tuoluoTypedef;
extern tuoluoTypedef tuoluo;//������

typedef __packed struct
{
  tFrameHeader         					  frameHeader;//֡ͷ
  uint16_t              				  CmdId;//������
  tdata 													data;//�������ݹ̶�
  ext_client_custom_shuzi_t 			shuzi;
  uint16_t                        CRC16;
}tupTypedef;
extern tupTypedef tup;//Ӣ��P��ĽǶ�
/*****end********/

//��Ҫ�ĺ�������
uint8_t Get_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength , uint8_t ucCRC8);
uint32_t Verify_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength);
void Append_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength , uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength);
void judgeCalculate(uint8_t* JudgeDataBuffer);
void int_into_char(char* Data , int16_t i);
void determine_ID(void);
_Bool is_red_or_blue(void);




void Usart4_TX_staticUI(void);		//������
void Usart4_TX_Letter(void);			//��ĸ
void Usart4_TX_Number(int32_t residueCap , int32_t pitchAngle , float angle);		//����//�Ƕȴ�С//���ݵ���
void Usart4_TX_Top(uint8_t state);		//������
void Usart4_TX_ChassisFriction(float angle);		//������״̬
void AllPrintfControl(uint64_t time);		//��ͼ���ܺ���

#endif

/***********************************������ID*************************************/
/*

ID: 0x0001          Byte: 11      ����״̬����                   ����Ƶ�� 1Hz
ID: 0x0002          Byte: 1       �����������                   ������������
ID: 0x0003          Byte: 32      ����������Ѫ������             ����Ƶ�� 1Hz
ID: 0x0004          Byte: 3       ���ڷ���״̬                   ���ڷ������
ID: 0x0005          Byte: 11      �˹�������ս���ӳ���ͷ�       ����Ƶ�� 1Hz
ID: 0x0101          Byte: 4       �����¼�����                   �¼��ı����
ID: 0x0102          Byte: 4       ���ز���վ������ʶ����         �����ı����
//ID: 0X0103          Byte: 2       ���ز���վԤԼ�ӵ�����         �����ӷ��ͣ�����10Hz��RM�Կ���δ���ţ�
ID: 0X0104          Byte: 2       ���о�������                   ���淢������
ID: 0x0105          Byte: 1       ���ڷ���ڵ���ʱ               ����Ƶ�� 1Hz
ID: 0X0201          Byte: 27      ������״̬����                 ����Ƶ�� 10Hz
ID: 0X0202          Byte: 16      ʵʱ������������               ����Ƶ�� 50Hz
ID: 0X0203          Byte: 16      ������λ������                 ����Ƶ�� 10Hz
ID: 0X0204          Byte: 1       ��������������                 ����״̬�ı����
ID: 0X0205          Byte: 2       ���л���������״̬����         ����Ƶ�� 10Hz��ֻ�п��л��������ط���
ID: 0X0206          Byte: 1       �˺�״̬����                   �˺���������
ID: 0X0207          Byte: 7       ʵʱ�������                   �ӵ��������
ID: 0X0208          Byte: 6       �ӵ�ʣ�෢����                 ���˻����ڱ�����  ����Ƶ�� 1Hz
ID: 0X0209          Byte: 4       ������RFID״̬                 ����Ƶ�� 1Hz
ID: 0x020A          Byte: 12      ���ڻ����˿ͻ���ָ����         ����Ƶ�� 10Hz
ID: 0X0301          Byte: n       �����˼佻������               ���ͷ���������    ����Ƶ�� 10Hz
ID: 0x0302          Byte: n       �Զ���������������ݽӿ�       �ͻ��˴�������    ����Ƶ�� 30Hz
ID: 0x0303          Byte: 15      �ͻ���С��ͼ��������           ��������
ID: 0x0304          Byte: 12      ���̡������Ϣ                 ͨ��ͼ�����ڷ��� */

/*******************************************************************************
/    ����ID          ���ȣ�ͷ�ṹ+�������ݶΣ�            ����˵��
/ 0x0200~0x02FF             6+n                         ���������˼�ͨ��
/    0x0100                 6+2                         �ͻ���ɾ��ͼ��
/    0x0101                 6+15                        �ͻ��˻���һ��ͼ��
/    0x0102                 6+30                        �ͻ��˻��ƶ���ͼ��
/    0x0103                 6+75                        �ͻ��˻������ͼ��
/    0x0104                 6+105                       �ͻ��˻����߸�ͼ��
/    0x0110                 6+45                        �ͻ��˻����ַ�ͼ��
********************************************************************************/
/*
  uint8_t graphic_name[3]; //ͼ����,��ɾ�����޸ĵȲ����У���Ϊ�ͻ��˵�������
  uint32_t operate_tpye:3; //ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
  uint32_t graphic_tpye:3; //ͼ�����ͣ�0��ֱ�ߣ�1�����Σ�2����Բ��3����Բ��4��Բ����5:������ 6�������� 7���ַ�
  uint32_t layer:4; //ͼ������0~9
  uint32_t color:4; //��ɫ��0��������ɫ��1����ɫ��2����ɫ��3����ɫ��4���Ϻ�ɫ��5����ɫ��6����ɫ��7����ɫ��8����ɫ��
  uint32_t start_angle:9; //����ʼ�Ƕȣ���λ���㣬��Χ[0,360]��
  uint32_t end_angle:9; //��ֹ�Ƕȣ���λ���㣬��Χ[0,360]��
  uint32_t width:10; //�߿�0-9
  uint32_t start_x:11; //���x���ꣻ
  uint32_t start_y:11; //���y���ꡣ
  uint32_t radius:10; //�����С���߰뾶��
  uint32_t end_x:11; //�յ�x���ꣻ
  uint32_t end_y:11; //�յ�y���ꡣ
*/

















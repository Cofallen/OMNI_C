#ifndef USER_DEFINE__H
#define USER_DEFINE__H
#include "remote_control.h"
/*
�����й������ȵĸ�ֵ  ��С�����Գ��� 1  ��СΪ0.01
											��С�����Գ��� 1  ��СΪ0.01
											��С�����Գ��� 1  ��СΪ0.01
*/
//���������
#define user_Sensitivity_yaw 0.6f		//yaw�����������
#define user_Sensitivity_pitch 0.8f		//pitch�����������//��Ȼ����ע�͸���p�ᵫʵ�ʿ��Ƶ�yaw

//���̸���
#define user_FollowSpeed_chassis 1000.0f		//���̸����ٶ�	
#define user_twoBuffer_keyboard	100		//����������ٶ�

//��������
//����ʽ����//����ʽ����//����ʽ����//��Ҫ������˵����
#define user_SuperCapPower	300			//�������ݷŵ繦��//�����Գ���350W
#define user_SuperCapLospTime	1000	//���������˳�ʱ�Ļ���ʱ��//��λms//1s = 1000ms

//�����Լ��������ٶ�
//����ʽ����//����ʽ����//����ʽ����//��Ҫ������˵����
#define user_SilentSpeed 50		//�����ٶ�//һ�����ڶҵ��Լ�΢��
#define user_UpSlopSpeed 100	//�����ٶ�//һ������ڵ͹�������ʹ��//�߹��ʿ���ֱ�ӳ���ȥ

//�������Զ��尴������
#define user_WindmillKey KeyBoard_qLock				//�糵ģʽ	
#define user_SupercapKey KeyBoard_cLock		//��������
#define user_CarSpinKey KeyBoard_shift		//С����
#define user_OpenMagazineKey (KeyBoard_bLock)		//������
#define user_OffMagazineKey (!KeyBoard_bLock)		//�ص���
#define user_ResetTop (KeyBoard_ctrl & KeyBoard_z)		//����������




#ifdef Infantry_Number_Four
#define user_AttackFuMod 0      //���ģʽ����       �����Ƿ����
#endif




#ifdef Infantry_Number_Three
#define user_AttackFuMod 0x00      //���ģʽ����       �����Ƿ����
#endif





//�������Զ��� 
#define user_NumberUpData (KeyBoard_c & KeyBoard_ctrl)		//��ʼ��������
#define user_SuperCap (KeyBoard_ctrl)		//���ݿ���
#endif

////����ʽ����
//#define KeyBoard_ctrl
//#define KeyBoard_a
//#define KeyBoard_s
//#define KeyBoard_d
//#define KeyBoard_w
//#define KeyBoard_q
//#define KeyBoard_e
//#define KeyBoard_r
//#define KeyBoard_t
//#define KeyBoard_g
//#define KeyBoard_z
//#define KeyBoard_x
//#define KeyBoard_c
//#define KeyBoard_v
//#define KeyBoard_b
//#define KeyBoard_shift
////����ʽ����
//#define KeyBoard_qLock
//#define KeyBoard_eLock
//#define KeyBoard_rLock
//#define KeyBoard_fLock
//#define KeyBoard_gLock
//#define KeyBoard_zLock
//#define KeyBoard_xLock
//#define KeyBoard_cLock
//#define KeyBoard_vLock
//#define KeyBoard_bLock
//#define KeyBoard_shiftLock
//#define KeyBoard_ctrlLock








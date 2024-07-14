#ifndef USER_DEFINE__H
#define USER_DEFINE__H
#include "remote_control.h"
/*
所有有关灵敏度的赋值  大小不可以超过 1  最小为0.01
											大小不可以超过 1  最小为0.01
											大小不可以超过 1  最小为0.01
*/
//鼠标灵敏度
#define user_Sensitivity_yaw 0.6f		//yaw轴基础灵敏度
#define user_Sensitivity_pitch 0.8f		//pitch轴基础灵敏度//虽然澍哥注释给的p轴但实际控制的yaw

//底盘跟随
#define user_FollowSpeed_chassis 1000.0f		//底盘跟随速度	
#define user_twoBuffer_keyboard	100		//按键缓冲的速度

//超级电容
//按键式触发//按键式触发//按键式触发//重要的事情说三遍
#define user_SuperCapPower	300			//超级电容放电功率//不可以超过350W
#define user_SuperCapLospTime	1000	//超级电容退出时的缓冲时间//单位ms//1s = 1000ms

//上坡以及静步的速度
//开关式触发//开关式触发//开关式触发//重要的事情说三遍
#define user_SilentSpeed 50		//静步速度//一般用于兑弹以及微调
#define user_UpSlopSpeed 100	//上坡速度//一般仅用于低功率上坡使用//高功率可以直接冲上去

//操作手自定义按键区域
#define user_WindmillKey KeyBoard_qLock				//风车模式	
#define user_SupercapKey KeyBoard_cLock		//超级电容
#define user_CarSpinKey KeyBoard_shift		//小陀螺
#define user_OpenMagazineKey (KeyBoard_bLock)		//开弹仓
#define user_OffMagazineKey (!KeyBoard_bLock)		//关弹仓
#define user_ResetTop (KeyBoard_ctrl & KeyBoard_z)		//重启陀螺仪




#ifdef Infantry_Number_Four
#define user_AttackFuMod 0      //打符模式开关       地盘是否跟随
#endif




#ifdef Infantry_Number_Three
#define user_AttackFuMod 0x00      //打符模式开关       地盘是否跟随
#endif





//操作手自定义 
#define user_NumberUpData (KeyBoard_c & KeyBoard_ctrl)		//开始更新数字
#define user_SuperCap (KeyBoard_ctrl)		//电容开关
#endif

////按键式触发
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
////开关式触发
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








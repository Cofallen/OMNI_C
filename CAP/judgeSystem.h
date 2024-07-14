#ifndef JUDGESYSTEM__H
#define JUDGESYSTEM__H

#include "stdint.h"
#include "usart.h"

extern uint8_t data_au8[255];

#define X_CENTER 960
#define Y_CENTER 540

//红蓝方
#define BLUE 0
#define RED  1

#define JudgeBufferLength       255
#define JudgeFrameHeader        0xA5        //帧头 
#define FrameHeader_Len         5		//帧头校验长度
#define client_custom_Length    28
extern uint8_t JudgeDataBuffer[JudgeBufferLength];
extern uint8_t JudgeDataBuffer_Send[JudgeBufferLength];
extern uint8_t client_custom_Send[client_custom_Length];
extern uint8_t robot_custom_Send[client_custom_Length];
/*__packed  c语言关键字，有了它结构体中的数据就不会首地址对齐
进而说有了它数据会被压缩
1.通常编译器在生成代码的时候都会进行结构体的填充，保证（结构体内部成员）最高性能的对齐方式
2.编译器自动分配出来结构体的内存（比如定义为全局变量或局部变量）肯定是对齐的
*/
typedef enum
{
  RobotStatus_ID                   =0x0201 ,	//机器人状态  等级
  PowerHeat_ID   	          			=0x0202 ,	//枪口热量 底盘功率
  RobotHurt_ID                    =0x0206 ,	//伤害类型
  ShootState_ID	      					  =0x0207 ,	//射频射速
} Judege_Cmd_ID;
//帧头
typedef __packed struct
{
  uint8_t   SOF;				//0xA5
  uint16_t  DataLenth;	//数据位长度
  uint8_t   Seq;				//包序号
  uint8_t   CRC8;				//crc8位校验
}tFrameHeader;
/*伤害类型*/
typedef __packed struct
{
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
} ext_robot_hurt_t;
/*比赛机器人状态*/
typedef __packed struct
{
  uint8_t robot_id;			//机器人ID
  uint8_t robot_level;	//机器人等级
  uint16_t remain_HP;		//机器人剩余HP
  uint16_t max_HP;			//机器人上限血量
  uint16_t shooter_id1_17mm_cooling_rate;		//1号17毫米冷却
  uint16_t shooter_id1_17mm_cooling_limit;	//1号17毫米热量上限
  uint16_t shooter_id1_17mm_speed_limit;		//1号17毫米枪口热量上限

  uint16_t shooter_id2_17mm_cooling_rate;		//2号17毫米冷却
  uint16_t shooter_id2_17mm_cooling_limit;	//2号17毫米热量上限		
  uint16_t shooter_id2_17mm_speed_limit;		//2号17毫米枪口射速上限

  uint16_t shooter_id1_42mm_cooling_rate;		//42毫米枪口没秒冷却值
  uint16_t shooter_id1_42mm_cooling_limit;	//42毫米枪口热量上限
  uint16_t shooter_id1_42mm_speed_limit;		//42毫米射速上限

  uint16_t chassis_power_limit;							//机器人底盘功率上限
  uint8_t mains_power_gimbal_output : 1;
  uint8_t mains_power_chassis_output : 1;
  uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;
/*底盘功率及枪口热量*/
typedef __packed struct
{
  uint16_t chassis_volt;					//底盘输出电压  单位 毫伏
  uint16_t chassis_current;				//底盘输出电流  单位毫安
  float chassis_power;						//底盘输出功率  单位 W
  uint16_t chassis_power_buffer;	//底盘功率缓冲  单位J
  uint16_t shooter_id1_17mm_cooling_heat;		//1号17毫米枪口热量
  uint16_t shooter_id2_17mm_cooling_heat;		//2号17毫米枪口热量
  uint16_t shooter_id1_42mm_cooling_heat;		//42毫米枪口热量
} ext_power_heat_data_t;
/*子弹信息*/
typedef __packed struct
{
  uint8_t bullet_type;		//子弹类型//1小//2大
  uint8_t shooter_id;			//发射ID 1、2对应1、2发射机构.3对应42毫米机构
  uint8_t bullet_freq;		//射频
  float bullet_speed;			//射速
} ext_shoot_data_t;


//////////裁判系统整体数据//////////
typedef  struct
{
  tFrameHeader          frameHeader;//帧头
  uint16_t              rxCmdId;//命令码
  ext_power_heat_data_t power_heat_data_t;//底盘功率枪口热量数据
  ext_shoot_data_t		  shoot_data_t;//射击信息
  ext_robot_hurt_t      robot_hurt_t;//伤害类型
  ext_game_robot_state_t game_robot_state_t;//机器人基本信息
}JudgementDataTypedef;
extern JudgementDataTypedef JudgementData;

//必要的函数声明
uint8_t Get_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength , uint8_t ucCRC8);
uint32_t Verify_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength);
void Append_CRC8_Check_Sum(uint8_t* pchMessage , uint16_t dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength , uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t* pchMessage , uint32_t dwLength);
void judgeCalculate(uint8_t* JudgeDataBuffer);

//二次处理的函数声明
#define RedColor 	1		//红色方
#define BuleColor	2		//蓝色方
#define HoreRobot 1				//英雄机器人	
#define RescueRobot 2			//工程
#define InfantryRobot 3		//步兵
#define AirRobot 6				//空中
#define SentryRobot 7			//哨兵
#define DartsRobot 8			//飞镖	
#define RadarRobot 9			//雷达

void Manage_RobotHurt(void);
void Manage_PowerHeat(void);
void Manage_ShootState(void);
void Manage_RobotStatus(void);

//二次处理的裁判系统数据
struct twoJudgeData_typedef
{
  //等级 血量 增益状态
  struct
  {
    uint8_t mod : 6; 	//车辆兵种
    uint8_t color : 2;//颜色
    int8_t level;		//等级	
    int16_t hp;			//当前血量
    int16_t maxHp;	//血量上限
  }RoboSate;
  //伤害来源
  struct
  {
    uint8_t ArmorFront : 1;		//前装甲板
    uint8_t ArmorRight : 1;		//右装甲板
    uint8_t ArmorBack : 1;		//后装甲板
    uint8_t ArmorLeft : 1;		//左装甲板		
    uint8_t ExceedPower : 1;	//超功率扣血
    uint8_t ExceedHead : 1;		//超热量扣血
    uint8_t ExceedSpeed : 1;	//超射速扣血
    uint8_t Mods_OffLine : 1;	//模块离线扣血	
  }HurtSource;
  //功率 缓冲 电池
  struct
  {
    float nowPower;				//当前功率
    uint16_t maxPower;		//最大功率
    uint16_t buffer;			//剩余缓冲
    uint16_t maxBuffer;		//最大缓冲
    uint16_t volt;				//电池电压
    float	bufferProportion;		// 剩余缓冲/最大缓冲
  }power;

  //机器人枪口信息
  struct
  {
    float shootSpeed;				//弹速
    uint16_t maxShootSpeed;	//最大射速
    uint16_t maxCooling;		//最大枪口热量
    uint16_t nowCooling;		//当前枪口热量
    int8_t coolingRate;			//枪口冷却
    int8_t residueNumber;		//剩余可以击打的弹丸数量	
  }muzzle;
};
extern struct twoJudgeData_typedef twoJudgeData;

/**********************************裁判系统接收end******************************/



/*********************************裁判系统发送begin*****************************/

typedef __packed struct
{
  uint16_t data_id;		//命令标识符
  uint16_t send_ID; //发送者
  uint16_t receive_ID;//接收者
}tdata;

//相关变量的结构体
typedef __packed struct
{
  uint8_t graphic_name[3];//名字
  uint32_t operate_tpye : 3;//
  uint32_t graphic_tpye : 3;
  uint32_t layer : 4;//图层
  uint32_t color : 4;//颜色
  uint32_t start_angle : 9;//椭圆
  uint32_t end_angle : 9;
  uint32_t width : 10;//线条宽度
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

//////////裁判系统发送整体数据//////////
typedef __packed struct
{
  tFrameHeader          frameHeader;//帧头
  uint16_t              CmdId;//命令码
  tdata 								data;//发送固定内容
  ext_uixianduan				uixianduan;
  uint16_t              CRC16;
}hypTypedef;
extern hypTypedef hyp;//线的绘制

typedef __packed struct
{
  tFrameHeader          					frameHeader;//帧头
  uint16_t              					CmdId;//命令码
  tdata 													data;
  ext_client_custom_character_t 	zifu;
  uint16_t              					CRC16;
}ziTypedef;
extern ziTypedef zi;//电容

typedef __packed struct
{
  tFrameHeader         					  frameHeader;//帧头
  uint16_t              				  CmdId;//命令码
  tdata 													data;//发送内容固定
  ext_dianrong 										dong;
  uint16_t                        CRC16;
}dianrongTypedef;
extern dianrongTypedef dianrong;//电容电量

typedef __packed struct
{
  tFrameHeader         					  frameHeader;//帧头
  uint16_t              				  CmdId;//命令码
  tdata 													data;//发送内容固定
  ext_client_custom_character_t 	zifu;
  uint16_t                        CRC16;
}tuoluoTypedef;
extern tuoluoTypedef tuoluo;//陀螺仪

typedef __packed struct
{
  tFrameHeader         					  frameHeader;//帧头
  uint16_t              				  CmdId;//命令码
  tdata 													data;//发送内容固定
  ext_client_custom_shuzi_t 			shuzi;
  uint16_t                        CRC16;
}tupTypedef;
extern tupTypedef tup;//英雄P轴的角度
/*****end********/

//必要的函数声明
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




void Usart4_TX_staticUI(void);		//辅助线
void Usart4_TX_Letter(void);			//字母
void Usart4_TX_Number(int32_t residueCap , int32_t pitchAngle , float angle);		//数字//角度大小//电容电量
void Usart4_TX_Top(uint8_t state);		//陀螺仪
void Usart4_TX_ChassisFriction(float angle);		//画底盘状态
void AllPrintfControl(uint64_t time);		//画图形总函数

#endif

/***********************************命令码ID*************************************/
/*

ID: 0x0001          Byte: 11      比赛状态数据                   发送频率 1Hz
ID: 0x0002          Byte: 1       比赛结果数据                   比赛结束后发送
ID: 0x0003          Byte: 32      比赛机器人血量数据             发送频率 1Hz
ID: 0x0004          Byte: 3       飞镖发射状态                   飞镖发射后发送
ID: 0x0005          Byte: 11      人工智能挑战赛加成与惩罚       发送频率 1Hz
ID: 0x0101          Byte: 4       场地事件数据                   事件改变后发送
ID: 0x0102          Byte: 4       场地补给站动作标识数据         动作改变后发送
//ID: 0X0103          Byte: 2       场地补给站预约子弹数据         参赛队发送，上限10Hz（RM对抗赛未开放）
ID: 0X0104          Byte: 2       裁判警告数据                   警告发生后发送
ID: 0x0105          Byte: 1       飞镖发射口倒计时               发送频率 1Hz
ID: 0X0201          Byte: 27      机器人状态数据                 发送频率 10Hz
ID: 0X0202          Byte: 16      实时功率热量数据               发送频率 50Hz
ID: 0X0203          Byte: 16      机器人位置数据                 发送频率 10Hz
ID: 0X0204          Byte: 1       机器人增益数据                 增益状态改变后发送
ID: 0X0205          Byte: 2       空中机器人能量状态数据         发送频率 10Hz，只有空中机器人主控发送
ID: 0X0206          Byte: 1       伤害状态数据                   伤害发生后发送
ID: 0X0207          Byte: 7       实时射击数据                   子弹发射后发送
ID: 0X0208          Byte: 6       子弹剩余发送数                 无人机与哨兵发送  发送频率 1Hz
ID: 0X0209          Byte: 4       机器人RFID状态                 发送频率 1Hz
ID: 0x020A          Byte: 12      飞镖机器人客户端指令书         发送频率 10Hz
ID: 0X0301          Byte: n       机器人间交互数据               发送方触发发送    上限频率 10Hz
ID: 0x0302          Byte: n       自定义控制器交互数据接口       客户端触发发送    上限频率 30Hz
ID: 0x0303          Byte: 15      客户端小地图交互数据           触发发送
ID: 0x0304          Byte: 12      键盘、鼠标信息                 通过图传串口发送 */

/*******************************************************************************
/    内容ID          长度（头结构+内容数据段）            功能说明
/ 0x0200~0x02FF             6+n                         己方机器人间通信
/    0x0100                 6+2                         客户端删除图形
/    0x0101                 6+15                        客户端绘制一个图形
/    0x0102                 6+30                        客户端绘制二个图形
/    0x0103                 6+75                        客户端绘制五个图形
/    0x0104                 6+105                       客户端绘制七个图形
/    0x0110                 6+45                        客户端绘制字符图形
********************************************************************************/
/*
  uint8_t graphic_name[3]; //图形名,在删除，修改等操作中，作为客户端的索引。
  uint32_t operate_tpye:3; //图形操作：0：空操作；1：增加；2：修改；3：删除；
  uint32_t graphic_tpye:3; //图形类型：0：直线；1：矩形；2：整圆；3：椭圆；4：圆弧；5:浮点数 6：整型数 7：字符
  uint32_t layer:4; //图层数，0~9
  uint32_t color:4; //颜色：0：红蓝主色；1：黄色；2：绿色；3：橙色；4：紫红色；5：粉色；6：青色；7：黑色；8：白色；
  uint32_t start_angle:9; //：起始角度，单位：°，范围[0,360]；
  uint32_t end_angle:9; //终止角度，单位：°，范围[0,360]。
  uint32_t width:10; //线宽；0-9
  uint32_t start_x:11; //起点x坐标；
  uint32_t start_y:11; //起点y坐标。
  uint32_t radius:10; //字体大小或者半径；
  uint32_t end_x:11; //终点x坐标；
  uint32_t end_y:11; //终点y坐标。
*/

















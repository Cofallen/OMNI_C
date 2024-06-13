/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 * 
 * 
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *            佛祖保佑     永不宕机     永无BUG
 * 
 *        佛曰:  
 *                写字楼里写字间，写字间里程序员；  
 *                程序人员写程序，又拿程序换酒钱。  
 *                酒醒只在网上坐，酒醉还来网下眠；  
 *                酒醉酒醒日复日，网上网下年复年。  
 *                但愿老死电脑间，不愿鞠躬老板前；  
 *                奔驰宝马贵者趣，公交自行程序员。  
 *                别人笑我忒疯癫，我笑自己命太贱；  
 *                不见满街漂亮妹，哪个归得程序员？
 */

/*
 * @Descripttion: 
 * @version: 
 * @Author: Eugene
 * @Date: 2024-01-23 11:39:25
 * @LastEditors: Andy
 * @LastEditTime: 2024-01-24 13:29:17
 */

#ifndef __READ_DATA_H
#define __READ_DATA_H

#include "usart.h" 


enum Read_Cmd_ID_Typdef
{
    game_state      = 0x0001 ,  //比赛状态数据，固定 1Hz 频率发送
    Match_results   = 0x0002 ,  //比赛结果数据，比赛结束触发发送
    Robot_HP        = 0x0003 ,  //机器人血量数据，固定 3Hz 频率
    Venue_Events    = 0x0101 ,  //场地事件数据，固定 1Hz 频率
    Supply_action   = 0x0102 ,  //补给站动作标识数据，补给站弹丸释放时触发发送
    Referee_warning = 0x0104 ,  //裁判警告数据，己方判罚/判负时触发发送，其余时间以 1Hz 频率发送
    Dart_fire       = 0x0105 ,  //飞镖发射相关数据，固定 1Hz 频率发送
    Robot_performan = 0x0201 ,  //机器人性能体系数据，固定10Hz 频率发送  
    time_power      = 0x0202 ,  //实时功率热量数据，固定 50Hz频率发送
    Robot_location  = 0x0203 ,  //机器人位置数据，哨兵机器人的发送频率为 10Hz，其余机器人的发送频率为 1Hz
    Robot_buff      = 0x0204 ,  //机器人增益数据，固定 3Hz 频率发送
    Air_support     = 0x0205 ,  //空中支援时间数据，固定 1Hz 频率发送
    Damage_status   = 0x0206 ,  //伤害状态数据，伤害发生后发送
    time_shooting   = 0x0207 ,  //实时射击数据，弹丸发射后发送
    Allowable_ammo  = 0x0208 ,  //允许发弹量，固定 10Hz 频率发送
    RFID            = 0x0209 ,  //机器人 RFID 状态，固定 3Hz 频率发送
    Dart_directives = 0x020A ,  //飞镖选手端指令数据，飞镖闸门上线后固定 3Hz 频率发送
    Ground_location = 0x020B ,  //地面机器人位置数据，固定 1Hz频率发送
    Radar_Marking   = 0x020C ,  //雷达标记进度数据，固定 1Hz 频率发送
    Route_Informat  = 0x020D ,  //哨兵自主决策相关信息同步，固定 1Hz 频率发送
    Radar_Informat  = 0x020E ,  //雷达自主决策信息同步，固定1Hz 频率发送
    Minimap         = 0x0303 ,  //选手端小地图交互数据，选手端触发发送
    remote          = 0x0304 ,  //键鼠遥控数据，固定 30Hz 频率发送
};

/*0x0001*/
typedef struct __packed
{
    uint8_t game_type : 4;      //比赛类型
    uint8_t game_progress : 4;  //当前比赛阶段
    uint16_t stage_remain_time; //当前阶段剩余时间，单位：秒
    uint64_t SyncTimeStamp;     //UNIX 时间，当机器人正确连接到裁判系统的 NTP 服务器后生效

}game_status_t;

/*0x0002*/
typedef struct __packed
{
    uint8_t winner;     //获胜方

}game_result_t;

/*0x0003*/
typedef struct __packed
{
    uint16_t red_1_robot_HP;    //红 1 英雄机器人血量。若该机器人未上场或者被罚下，则血量为 0
    uint16_t red_2_robot_HP;    //红 2 工程机器人血量
    uint16_t red_3_robot_HP;    //红 3 步兵机器人血量 
    uint16_t red_4_robot_HP;    //红 4 步兵机器人血量 
    uint16_t red_5_robot_HP;    //红 5 步兵机器人血量 
    uint16_t red_7_robot_HP;    //红 7 哨兵机器人血量
    uint16_t red_outpost_HP;    //红方前哨站血量 
    uint16_t red_base_HP;       //红方基地血量 
    uint16_t blue_1_robot_HP;   //蓝 1 英雄机器人血量 
    uint16_t blue_2_robot_HP;   //蓝 2 工程机器人血量 
    uint16_t blue_3_robot_HP;   //蓝 3 步兵机器人血量
    uint16_t blue_4_robot_HP;   //蓝 4 步兵机器人血量 
    uint16_t blue_5_robot_HP;   //蓝 5 步兵机器人血量 
    uint16_t blue_7_robot_HP;   //蓝 7 哨兵机器人血量 
    uint16_t blue_outpost_HP;   //蓝方前哨站血量 
    uint16_t blue_base_HP;      //蓝方基地血量 

}game_robot_HP_t;

/*0x0101*/
typedef struct __packed
{
    uint32_t refueling_Blood : 1;           //己方补给站前补血点的占领状态，1 为已占领 
    uint32_t supply_Blood : 1;              //己方补给站内部补血点的占领状态，1 为已占领 
    uint32_t Reserved_bits_1 : 1;           //己方补给区的占领状态，1 为已占领（仅 RMUL 适用）
    uint32_t buff_activated : 1;            //己方能量机关激活点的占领状态，1 为已占领 
    uint32_t buff_small : 1;                //己方小能量机关的激活状态，1 为已激活
    uint32_t buff_big : 1;                  //己方大能量机关的激活状态，1 为已激活 
    uint32_t Circular_Heights : 2;          //己方环形高地的占领状态，1 为被己方占领，2 为被对方占领 
    uint32_t Trapezoidal_heights_3 : 2;     //己方梯形高地的占领状态，1 为被己方占领，2 为被对方占领 
    uint32_t Trapezoidal_heights_4 : 2;     //己方梯形高地的占领状态，1 为被己方占领，2 为被对方占领 
    uint32_t Virtual_Shield : 7;            //己方基地虚拟护盾的剩余值百分比（四舍五入，保留整数）
    uint32_t Dart_hit_time : 9;             //飞镖最后一次击中己方前哨站或基地的时间（0-420，开局默认为0
    uint32_t Dart_hit_aim : 2;              //飞镖最后一次击中己方前哨站或基地的具体目标，开局默认为 0，1 为击中前哨站，2 为击中基地固定目标，3 为击中基地随机目标 
    uint32_t Reserved_bits_2 : 2;           //中心增益点的占领情况，0 为未被占领，1 为被己方占领，2 为被对方占领，3 为被双方占领。（仅 RMUL 适用）

}event_data_t;

/*0x0102*/
typedef struct __packed
{
    uint8_t reserved;                   //保留
    uint8_t supply_robot_id;            //补弹机器人 ID：
    uint8_t supply_projectile_step;     //出弹口开闭状态： 
    uint8_t supply_projectile_num;      //补弹数量：

} ext_supply_projectile_action_t;

/*0x0104*/
typedef struct __packed
{
    uint8_t level;                  //己方最后一次受到判罚的等级
    uint8_t offending_robot_id;     //己方最后一次受到判罚的违规机器人 ID
    uint8_t count;                  //己方最后一次受到判罚的违规机器人对应判罚等级的违规次数

}referee_warning_t;

/*0x0105*/
typedef struct __packed
{
    uint8_t dart_remaining_time;    //己方飞镖发射剩余时间，单位：秒
    uint8_t Dart_aimed : 2;         //最近一次己方飞镖击中的目标，开局默认为 0，1 为击中前哨站，2 为击中基地固定目标，3 为击中基地随机目标 
    uint8_t Dart_success : 3;       //对方最近被击中的目标累计被击中计数，开局默认为 0，至多为 4 
    uint8_t Dart_aiming : 2;        //飞镖此时选定的击打目标，开局默认或未选定/选定前哨站时为 0，选中基地固定目标为 1，选中基地随机目标为 2 
    uint8_t Reserved_bits : 1;      //保留

}dart_info_t;

/*0x0201*/
typedef struct __packed
{
    uint8_t robot_id;                               //本机器人 ID
    uint8_t robot_level;                            //机器人等级
    uint16_t current_HP;                            //机器人当前血量
    uint16_t maximum_HP;                            //机器人血量上限
    uint16_t shooter_barrel_cooling_value;          //机器人枪口热量每秒冷却值
    uint16_t shooter_barrel_heat_limit;             //机器人枪口热量上限
    uint16_t chassis_power_limit;                   //机器人底盘功率上限
    uint8_t power_management_gimbal_output : 1;     //gimbal 口输出：0 为无输出，1 为 24V 输出 
    uint8_t power_management_chassis_output : 1;    //chassis 口输出：0 为无输出，1 为 24V 输出 
    uint8_t power_management_shooter_output : 1;    //shooter 口输出：0 为无输出，1 为 24V 输出

}robot_status_t;

/*0x0202*/
typedef struct __packed
{
    uint16_t chassis_voltage;               //电源管理模块的 chassis 口输出电压（单位：mV）
    uint16_t chassis_current;               //电源管理模块的 chassis 口输出电流（单位：mA） 
    float chassis_power;                    //底盘功率（单位：W） 
    uint16_t buffer_energy;                 //缓冲能量（单位：J）
    uint16_t shooter_17mm_1_barrel_heat;    //第 1 个 17mm 发射机构的枪口热量
    uint16_t shooter_17mm_2_barrel_heat;    //第 2 个 17mm 发射机构的枪口热量 
    uint16_t shooter_42mm_barrel_heat;      //42mm 发射机构的枪口热量 

}power_heat_data_t;

/*0x0203*/
typedef struct __packed
{
    float x;        //本机器人位置 x 坐标，单位：m 
    float y;        //本机器人位置 y 坐标，单位：m 
    float angle;    //本机器人测速模块的朝向，单位：度。正北为 0 度 

}robot_pos_t;

/*0x0204*/
typedef struct __packed
{
    uint8_t recovery_buff;      //机器人回血增益（百分比，值为 10 表示每秒恢复血量上限的 10%） 
    uint8_t cooling_buff;       //机器人枪口冷却倍率（直接值，值为 5 表示 5 倍冷却）
    uint8_t defence_buff;       //机器人防御增益（百分比，值为 50 表示 50%防御增益） 
    uint8_t vulnerability_buff; //机器人负防御增益（百分比，值为 30 表示-30%防御增益）
    uint16_t attack_buff;       //机器人攻击增益（百分比，值为 50 表示 50%攻击增益） 

}buff_t;

/*0x0205*/
typedef struct __packed
{
    uint8_t airforce_status;    //空中机器人状态（0 为正在冷却，1 为冷却完毕，2 为正在空中支援） 
    uint8_t time_remain;        //此状态的剩余时间

}air_support_data_t;

/*0x0206*/
typedef struct __packed
{
    uint8_t armor_id : 4;               //当扣血原因为装甲模块被弹丸攻击、受撞击、离线或测速模块离线时，该 4 bit 组成的数值为装甲模块或测速模块的 ID 编号；当其他原因导致扣血时，该数值为 0 
    uint8_t HP_deduction_reason : 4;    //血量变化类型 

}hurt_data_t;

/*0x0207*/
typedef struct __packed
{
    uint8_t bullet_type;            //弹丸类型
    uint8_t shooter_number;         //发射机构 ID
    uint8_t launching_frequency;    //弹丸射速（单位：Hz） 
    float initial_speed;            //弹丸初速度（单位：m/s）

}shoot_data_t;

/*0x0208*/
typedef struct __packed
{
    uint16_t projectile_allowance_17mm;     //17mm 弹丸允许发弹量
    uint16_t projectile_allowance_42mm;     //42mm 弹丸允许发弹量
    uint16_t remaining_gold_coin;           //剩余金币数量

}projectile_allowance_t;

/*0x0209*/
typedef struct __packed
{
    uint32_t bit0 : 1;      //己方基地增益点 
    uint32_t bit1 : 1;      //己方环形高地增益点 
    uint32_t bit2 : 1;      //对方环形高地增益点
    uint32_t bit3 : 1;      //己方 R3/B3 梯形高地增益点 
    uint32_t bit4 : 1;      //对方 R3/B3 梯形高地增益点
    uint32_t bit5 : 1;      //己方 R4/B4 梯形高地增益点 
    uint32_t bit6 : 1;      //对方 R4/B4 梯形高地增益点 
    uint32_t bit7 : 1;      //己方能量机关激活点 
    uint32_t bit8 : 1;      //己方飞坡增益点（靠近己方一侧飞坡前）
    uint32_t bit9 : 1;      //己方飞坡增益点（靠近己方一侧飞坡后）
    uint32_t bit10 : 1;     //对方飞坡增益点（靠近对方一侧飞坡前）
    uint32_t bit11 : 1;     //对方飞坡增益点（靠近对方一侧飞坡后）
    uint32_t bit12 : 1;     //己方前哨站增益点 
    uint32_t bit13 : 1;     //己方补血点（检测到任一均视为激活）
    uint32_t bit14 : 1;     //己方哨兵巡逻区
    uint32_t bit15 : 1;     //对方哨兵巡逻区 
    uint32_t bit16 : 1;     //己方大资源岛增益点 
    uint32_t bit17 : 1;     //对方大资源岛增益点 
    uint32_t bit18 : 1;     //己方兑换区 
    uint32_t bit19 : 1;     //中心增益点（仅 RMUL 适用）
    uint32_t bit20_31 : 13; //保留

}rfid_status_t;

/*0x020A*/
typedef struct __packed
{
    uint8_t dart_launch_opening_status;     //当前飞镖发射站的状态：1：关闭 2：正在开启或者关闭中 0：已经开启 
    uint8_t reserved;                       //保留位
    uint16_t target_change_time;            //切换击打目标时的比赛剩余时间，单位：秒，无/未切换动作，默认为 0。 
    uint16_t latest_launch_cmd_time;        //最后一次操作手确定发射指令时的比赛剩余时间，单位：秒，初始值为 0。 

}dart_client_cmd_t;

/*0x020B*/
typedef struct __packed
{
    float hero_x;           //己方英雄机器人位置 x 轴坐标，单位：m 
    float hero_y;           //己方英雄机器人位置 y 轴坐标，单位：m
    float engineer_x;       //己方工程机器人位置 x 轴坐标，单位：m 
    float engineer_y;       //己方工程机器人位置 y 轴坐标，单位：m
    float standard_3_x;     //己方 3 号步兵机器人位置 x 轴坐标，单位：m 
    float standard_3_y;     //己方 3 号步兵机器人位置 y 轴坐标，单位：m 
    float standard_4_x;     //己方 4 号步兵机器人位置 x 轴坐标，单位：m 
    float standard_4_y;     //己方 4 号步兵机器人位置 y 轴坐标，单位：m 
    float standard_5_x;     //己方 5 号步兵机器人位置 x 轴坐标，单位：m 
    float standard_5_y;     //己方 5 号步兵机器人位置 y 轴坐标，单位：m 

}ground_robot_position_t;

/*0x020C*/
typedef struct __packed
{
    uint8_t mark_hero_progress;         //对方英雄机器人被标记进度：0-120 
    uint8_t mark_engineer_progress;     //对方工程机器人被标记进度：0-120
    uint8_t mark_standard_3_progress;   //对方 3 号步兵机器人被标记进度：0-120 
    uint8_t mark_standard_4_progress;   //对方 4 号步兵机器人被标记进度：0-120 
    uint8_t mark_standard_5_progress;   //对方 5 号步兵机器人被标记进度：0-120 
    uint8_t mark_sentry_progress;       //对方哨兵机器人被标记进度：0-120 

}radar_mark_data_t;

/*0x020D*/
typedef struct __packed
{
    uint32_t successfully_redeemed : 11;        //除远程兑换外，哨兵成功兑换的发弹量，开局为 0，在哨兵成功兑换一定发弹量后，该值将变为哨兵成功兑换的发弹量值。 
    uint32_t remote_successfully_redeemed : 4;  //哨兵成功远程兑换发弹量的次数，开局为 0，在哨兵成功远程兑换发弹量后，该值将变为哨兵成功远程兑换发弹量的次数。 
    uint32_t remote_HP : 4;                     //哨兵成功远程兑换血量的次数，开局为 0，在哨兵成功远程兑换血量后，该值将变为哨兵成功远程兑换血量的次数。 
    uint32_t Reserved_bits : 13;                //保留

}sentry_info_t;

/*0x020E*/
typedef struct __packed
{
    uint8_t vulnerable_begin : 2;   //雷达是否拥有触发双倍易伤的机会，开局为 0，数值为雷达拥有触发双倍易伤的机会，至多为 2 
    uint8_t vulnerable_now : 1;     //对方是否正在被触发双倍易伤
	uint8_t vulnerable_receive : 5;
}radar_info_t;

/*0x0303*/
typedef struct __packed
{
    float target_position_x;    //目标位置 x 轴坐标，单位 m  当发送目标机器人 ID 时，该值为 0 
    float target_position_y;    //目标位置 y 轴坐标，单位 m  当发送目标机器人 ID 时，该值为 0 
    uint8_t cmd_keyboard;       //云台手按下的键盘按键通用键值
    uint8_t target_robot_id;    //对方机器人 ID 
    uint8_t cmd_source;         //信息来源 ID 

}map_command_t;

/*0x0304*/
typedef struct __packed
{
    int16_t mouse_x;                    //鼠标 x 轴移动速度，负值标识向左移动
    int16_t mouse_y;                    //鼠标 y 轴移动速度，负值标识向下移动
    int16_t mouse_z;                    //鼠标滚轮移动速度，负值标识向后滚动
    int8_t left_button_down;            //鼠标左键是否按下：0 为未按下；1 为按下
    int8_t right_button_down;           //鼠标右键是否按下：0 为未按下，1 为按下
    uint16_t keyboard_value_W : 1;      //W 键
    uint16_t keyboard_value_S : 1;      //S 键 
    uint16_t keyboard_value_A : 1;      //A 键 
    uint16_t keyboard_value_D : 1;      //D 键 
    uint16_t keyboard_value_Shift : 1;  //Shift 键 
    uint16_t keyboard_value_Ctrl : 1;   //Ctrl 键 
    uint16_t keyboard_value_Q : 1;      //Q 键 
    uint16_t keyboard_value_E : 1;      //E 键 
    uint16_t keyboard_value_R : 1;      //R 键
    uint16_t keyboard_value_F : 1;      //F 键
    uint16_t keyboard_value_G : 1;      //G 键 
    uint16_t keyboard_value_Z : 1;      //Z 键 
    uint16_t keyboard_value_X : 1;      //X 键 
    uint16_t keyboard_value_C : 1;      //C 键 
    uint16_t keyboard_value_V : 1;      //V 键 
    uint16_t keyboard_value_B : 1;      //B 键 
    uint16_t reserved;                  //保留位 

}remote_control_t;



typedef struct __packed
{
    /* data */
    game_status_t game_status;
    game_result_t game_result;
    game_robot_HP_t game_robot_HP;
    event_data_t event_data;
    ext_supply_projectile_action_t ext_supply_projectile_action;
    referee_warning_t referee_warning;
    dart_info_t dart_info;
    robot_status_t robot_status;
    power_heat_data_t power_heat_data;
    robot_pos_t robot_pos;
    buff_t buff;
    air_support_data_t air_support_data;
    hurt_data_t hurt_data;
    shoot_data_t shoot_data;
    projectile_allowance_t projectile_allowance;
    rfid_status_t rfid_status;
    dart_client_cmd_t dart_client_cmd;
    ground_robot_position_t ground_robot_position;
    radar_mark_data_t radar_mark_data;
    sentry_info_t sentry_info;
    radar_info_t radar_info;
    map_command_t map_command;
    remote_control_t remote_control;

}User_Data_T;



typedef struct __packed 
{
    uint8_t   SOF;				//0xA5
    uint16_t  DataLenth;	    //数据位长度
	uint8_t   Seq;				//包序号
    uint8_t   CRC8;				//crc8位校验
}frame_header_R_Typdef;

typedef union 
{
	struct __packed
    {
        /* data head*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
    }RX_Data_head;
	
    struct __packed
    {
        /* data game_status*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        game_status_t game_status;
        uint16_t frame_tail;    
    }RX_Data_game_status;
	
	struct __packed
    {
        /* data game_result*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        game_result_t game_result;
        uint16_t frame_tail;    
    }RX_Data_game_result;
	
	struct __packed
    {
        /* data game_robot_HP*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        game_robot_HP_t game_robot_HP;
        uint16_t frame_tail;    
    }RX_Data_game_robot_HP;
	
	struct __packed
    {
        /* data event_data*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        event_data_t event_data;
        uint16_t frame_tail;    
    }RX_Data_event_data;
	
	struct __packed
    {
        /* data ext_supply_projectile_action*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        ext_supply_projectile_action_t ext_supply_projectile_action;
        uint16_t frame_tail;    
    }RX_Data_ext_supply_projectile_action;
	
	struct __packed
    {
        /* data referee_warning*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        referee_warning_t referee_warning;
        uint16_t frame_tail;    
    }RX_Data_referee_warning;
	
	struct __packed
    {
        /* data dart_info*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        dart_info_t dart_info;
        uint16_t frame_tail;    
    }RX_Data_dart_info;
	
	struct __packed
    {
        /* data robot_status*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        robot_status_t robot_status;
        uint16_t frame_tail;    
    }RX_Data_robot_status;
	
	struct __packed
    {
        /* data power_heat_data*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        power_heat_data_t power_heat_data;
        uint16_t frame_tail;    
    }RX_Data_power_heat_data;
	
	struct __packed
    {
        /* data robot_pos*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        robot_pos_t robot_pos;
        uint16_t frame_tail;    
    }RX_Data_robot_pos;
	
	struct __packed
    {
        /* data buff*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        buff_t buff;
        uint16_t frame_tail;    
    }RX_Data_buff;
	
	struct __packed
    {
        /* data air_support_data*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        air_support_data_t air_support_data;
        uint16_t frame_tail;    
    }RX_Data_air_support_data;
	
	struct __packed
    {
        /* data hurt_data*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        hurt_data_t hurt_data;
        uint16_t frame_tail;    
    }RX_Data_hurt_data;
	
	struct __packed
    {
        /* data shoot_data*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        shoot_data_t shoot_data;
        uint16_t frame_tail;    
    }RX_Data_shoot_data;
	
	struct __packed
    {
        /* data projectile_allowance*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        projectile_allowance_t projectile_allowance;
        uint16_t frame_tail;    
    }RX_Data_projectile_allowance;
	
	struct __packed
    {
        /* data rfid_status*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        rfid_status_t rfid_status;
        uint16_t frame_tail;    
    }RX_Data_rfid_status;
	
	struct __packed
    {
        /* data dart_client_cmd*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        dart_client_cmd_t dart_client_cmd;
        uint16_t frame_tail;    
    }RX_Data_dart_client_cmd;
	
	struct __packed
    {
        /* data ground_robot_position*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        ground_robot_position_t ground_robot_position;
        uint16_t frame_tail;    
    }RX_Data_ground_robot_position;
	
	struct __packed
    {
        /* data radar_mark_data*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        radar_mark_data_t radar_mark_data;
        uint16_t frame_tail;    
    }RX_Data_radar_mark_data;
	
	struct __packed
    {
        /* data sentry_info*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        sentry_info_t sentry_info;
        uint16_t frame_tail;    
    }RX_Data_sentry_info;
	
	struct __packed
    {
        /* data radar_info*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        radar_info_t radar_info;
        uint16_t frame_tail;    
    }RX_Data_radar_info;
	
	struct __packed
    {
        /* data map_command*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        map_command_t map_command;
        uint16_t frame_tail;    
    }RX_Data_map_command;
	
	struct __packed
    {
        /* data remote_control*/
        frame_header_R_Typdef frame_header;
        uint16_t read_cmd_id;
        remote_control_t remote_control;
        uint16_t frame_tail;    
    }RX_Data_remote_control;

    uint8_t Data[255];
    
}ALL_RX_Data_T;

void Read_Data_first(ALL_RX_Data_T *ALL_RX_Data , User_Data_T *user_data , uint16_t length);
uint16_t Read_Data_system(ALL_RX_Data_T *ALL_RX_Data , User_Data_T *user_data);

extern ALL_RX_Data_T ALL_RX;
extern User_Data_T user_data;

#endif

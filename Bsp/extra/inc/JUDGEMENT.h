#ifndef __JUDGEMENT_H__
#define __JUDGEMENT_H__

#include <cmsis_armclang.h>
#include "DEFINE.h"

/***************************** ↓ 帧头 ↓ **********************************/
typedef struct
{
    uint8_t SOF;         // 0xA5
    uint16_t DATA_LENTH; // 数据位长度
    uint8_t SEQ;         // 包序号
    uint8_t CRC8;        // CRC8位校验
} __attribute__((__packed__)) RUI_TYPEDEF_FRAME_HEADER_TF;
/***************************** ↑ 帧头 ↑ **********************************/

enum RUI_TYPEDEF_READ_CMD_ID_TYPDEF
{
    GAME_STATE = 0x0001,      // 比赛状态数据，固定 1Hz 频率发送
    MATCH_RESULTS = 0x0002,   // 比赛结果数据，比赛结束触发发送
    ROBOT_HP = 0x0003,        // 机器人血量数据，固定 3Hz 频率
    VENUE_EVENTS = 0x0101,    // 场地事件数据，固定 1Hz 频率
    SUPPLY_ACTION = 0x0102,   // 补给站动作标识数据，补给站弹丸释放时触发发送
    REFEREE_WARNING = 0x0104, // 裁判警告数据，己方判罚/判负时触发发送，其余时间以 1Hz 频率发送
    DART_FIRE = 0x0105,       // 飞镖发射相关数据，固定 1Hz 频率发送
    ROBOT_PERFORMAN = 0x0201, // 机器人性能体系数据，固定10Hz 频率发送
    TIME_POWER = 0x0202,      // 实时功率热量数据，固定 50Hz频率发送
    ROBOT_LOCATION = 0x0203,  // 机器人位置数据，哨兵机器人的发送频率为 10Hz，其余机器人的发送频率为 1Hz
    ROBOT_BUFF = 0x0204,      // 机器人增益数据，固定 3Hz 频率发送
    AIR_SUPPORT = 0x0205,     // 空中支援时间数据，固定 1Hz 频率发送
    DAMAGE_STATUS = 0x0206,   // 伤害状态数据，伤害发生后发送
    TIME_SHOOTING = 0x0207,   // 实时射击数据，弹丸发射后发送
    ALLOWABLE_AMMO = 0x0208,  // 允许发弹量，固定 10Hz 频率发送
    RFID = 0x0209,            // 机器人 RFID 状态，固定 3Hz 频率发送
    DART_DIRECTIVES = 0x020A, // 飞镖选手端指令数据，飞镖闸门上线后固定 3Hz 频率发送
    GROUND_LOCATION = 0x020B, // 地面机器人位置数据，固定 1Hz频率发送
    RADAR_MARKING = 0x020C,   // 雷达标记进度数据，固定 1Hz 频率发送
    ROUTE_INFORMAT = 0x020D,  // 哨兵自主决策相关信息同步，固定 1Hz 频率发送
    RADAR_INFORMAT = 0x020E,  // 雷达自主决策信息同步，固定1Hz 频率发送
    MINIMAP = 0x0303,         // 选手端小地图交互数据，选手端触发发送
    REMOTE = 0x0304,          // 键鼠遥控数据，固定 30Hz 频率发送
};

/*0x0001*/
typedef struct
{
    uint8_t GAME_TYPE : 4;
    uint8_t GAME_PROGRESS : 4;
    uint16_t STAGE_REMAIN_TIME;
    uint64_t SYNC_TIME_STAMP;

} __attribute__((__packed__)) RUI_TYPEDEF_GAME_STATUS_T;

/*0x0002*/
typedef struct
{
    uint8_t WINNER;

} __attribute__((__packed__)) RUI_TYPEDEF_GAME_RESULT_T;

/*0x0003*/
typedef struct
{
    uint16_t RED_1_ROBOT_HP;
    uint16_t RED_2_ROBOT_HP;
    uint16_t RED_3_ROBOT_HP;
    uint16_t RED_4_ROBOT_HP;
    uint16_t RED_5_ROBOT_HP;
    uint16_t RED_7_ROBOT_HP;
    uint16_t RED_OUTPOST_HP;
    uint16_t RED_BASE_HP;
    uint16_t BLUE_1_ROBOT_HP;
    uint16_t BLUE_2_ROBOT_HP;
    uint16_t BLUE_3_ROBOT_HP;
    uint16_t BLUE_4_ROBOT_HP;
    uint16_t BLUE_5_ROBOT_HP;
    uint16_t BLUE_7_ROBOT_HP;
    uint16_t BLUE_OUTPOST_HP;
    uint16_t BLUE_BASE_HP;

} __attribute__((__packed__)) RUI_TYPEDEF_GAME_ROBOT_HP_T;

/*0x0101*/
typedef struct
{
    uint32_t REFUELING_BLOOD : 1;
    uint32_t SUPPLY_BLOOD : 1;
    uint32_t RESERVED_BITS_1 : 1;
    uint32_t BUFF_ACTIVATED : 1;
    uint32_t BUFF_SMALL : 1;
    uint32_t BUFF_BIG : 1;
    uint32_t CIRCULAR_HEIGHTS : 2;
    uint32_t TRAPEZOIDAL_HEIGHTS_3 : 2;
    uint32_t TRAPEZOIDAL_HEIGHTS_4 : 2;
    uint32_t VIRTUAL_SHIELD : 7;
    uint32_t DART_HIT_TIME : 9;
    uint32_t DART_HIT_AIM : 2;
    uint32_t RESERVED_BITS_2 : 2;

} __attribute__((__packed__)) RUI_TYPEDEF_EVENT_DATA_T;

/*0x0102*/
typedef struct
{
    uint8_t RESERVED;
    uint8_t SUPPLY_ROBOT_ID;
    uint8_t SUPPLY_PROJECTILE_STEP;
    uint8_t SUPPLY_PROJECTILE_NUM;

} __attribute__((__packed__)) RUI_TYPEDEF_EXT_SUPPLY_PROJECTILE_ACTION_T;

/*0x0104*/
typedef struct
{
    uint8_t LEVEL;
    uint8_t OFFENDING_ROBOT_ID;
    uint8_t COUNT;

} __attribute__((__packed__)) RUI_TYPEDEF_REFEREE_WARNING_T;

/*0x0105*/
typedef struct
{
    uint8_t DART_REMAINING_TIME;
    uint8_t DART_AIMED : 2;
    uint8_t DART_SUCCESS : 3;
    uint8_t DART_AIMING : 2;
    uint8_t RESERVED_BITS : 1;

} __attribute__((__packed__)) RUI_TYPEDEF_DART_INFO_T;

/*0x0201*/
typedef struct
{
    uint8_t ROBOT_ID;
    uint8_t ROBOT_LEVEL;
    uint16_t CURRENT_HP;
    uint16_t MAXIMUM_HP;
    uint16_t SHOOTER_BARREL_COOLING_VALUE;
    uint16_t SHOOTER_BARREL_HEAT_LIMIT;
    uint16_t CHASSIS_POWER_LIMIT;
    uint8_t POWER_MANAGEMENT_GIMBAL_OUTPUT : 1;
    uint8_t POWER_MANAGEMENT_CHASSIS_OUTPUT : 1;
    uint8_t POWER_MANAGEMENT_SHOOTER_OUTPUT : 1;

} __attribute__((__packed__)) RUI_TYPEDEF_ROBOT_STATUS_T;

/*0x0202*/
typedef struct
{
    uint16_t CHASSIS_VOLTAGE;
    uint16_t CHASSIS_CURRENT;
    float CHASSIS_POWER;
    uint16_t BUFFER_ENERGY;
    uint16_t SHOOTER_17_MM_1_BARREL_HEAT;
    uint16_t SHOOTER_17_MM_2_BARREL_HEAT;
    uint16_t SHOOTER_42_MM_BARREL_HEAT;

} __attribute__((__packed__)) RUI_TYPEDEF_POWER_HEAT_DATA_T;

/*0x0203*/
typedef struct
{
    float X;
    float Y;
    float ANGLE;

} __attribute__((__packed__)) RUI_TYPEDEF_ROBOT_POS_T;

/*0x0204*/
typedef struct
{
    uint8_t RECOVERY_BUFF;
    uint8_t COOLING_BUFF;
    uint8_t DEFENCE_BUFF;
    uint16_t ATTACK_BUFF;

} __attribute__((__packed__)) RUI_TYPEDEF_BUFF_T;

/*0x0205*/
typedef struct
{
    uint8_t AIRFORCE_STATUS;
    uint8_t TIME_REMAIN;

} __attribute__((__packed__)) RUI_TYPEDEF_AIR_SUPPORT_DATA_T;

/*0x0206*/
typedef struct
{
    uint8_t ARMOR_ID : 4;
    uint8_t HP_DEDUCTION_REASON : 4;

} __attribute__((__packed__)) RUI_TYPEDEF_HURT_DATA_T;

/*0x0207*/
typedef struct
{
    uint8_t BULLET_TYPE;
    uint8_t SHOOTER_NUMBER;
    uint8_t LAUNCHING_FREQUENCY;
    float INITIAL_SPEED;

} __attribute__((__packed__)) RUI_TYPEDEF_SHOOT_DATA_T;

/*0x0208*/
typedef struct
{
    uint16_t PROJECTILE_ALLOWANCE_17_MM;
    uint16_t PROJECTILE_ALLOWANCE_42_MM;
    uint16_t REMAINING_GOLD_COIN;

} __attribute__((__packed__)) RUI_TYPEDEF_PROJECTILE_ALLOWANCE_T;

/*0x0209*/
typedef struct
{
    uint32_t BIT_0 : 1;
    uint32_t BIT_1 : 1;
    uint32_t BIT_2 : 1;
    uint32_t BIT_3 : 1;
    uint32_t BIT_4 : 1;
    uint32_t BIT_5 : 1;
    uint32_t BIT_6 : 1;
    uint32_t BIT_7 : 1;
    uint32_t BIT_8 : 1;
    uint32_t BIT_9 : 1;
    uint32_t BIT_10 : 1;
    uint32_t BIT_11 : 1;
    uint32_t BIT_12 : 1;
    uint32_t BIT_13 : 1;
    uint32_t BIT_14 : 1;
    uint32_t BIT_15 : 1;
    uint32_t BIT_16 : 1;
    uint32_t BIT_17 : 1;
    uint32_t BIT_18 : 1;
    uint32_t BIT_19 : 1;
    uint32_t BIT_20_31 : 13;

} __attribute__((__packed__)) RUI_TYPEDEF_RFID_STATUS_T;

/*0x020A*/
typedef struct
{
    uint8_t DART_LAUNCH_OPENING_STATUS;
    uint8_t RESERVED;
    uint16_t TARGET_CHANGE_TIME;
    uint16_t LATEST_LAUNCH_CMD_TIME;

} __attribute__((__packed__)) RUI_TYPEDEF_DART_CLIENT_CMD_T;

/*0x020B*/
typedef struct
{
    float HERO_X;
    float HERO_Y;
    float ENGINEER_X;
    float ENGINEER_Y;
    float STANDARD_3_X;
    float STANDARD_3_Y;
    float STANDARD_4_X;
    float STANDARD_4_Y;
    float STANDARD_5_X;
    float STANDARD_5_Y;

} __attribute__((__packed__)) RUI_TYPEDEF_GROUND_ROBOT_POSITION_T;

/*0x020C*/
typedef struct
{
    uint8_t MARK_HERO_PROGRESS;
    uint8_t MARK_ENGINEER_PROGRESS;
    uint8_t MARK_STANDARD_3_PROGRESS;
    uint8_t MARK_STANDARD_4_PROGRESS;
    uint8_t MARK_STANDARD_5_PROGRESS;
    uint8_t MARK_SENTRY_PROGRESS;

} __attribute__((__packed__)) RUI_TYPEDEF_RADAR_MARK_DATA_T;

/*0x020D*/
typedef struct
{
    uint32_t SUCCESSFULLY_REDEEMED : 11;
    uint32_t REMOTE_SUCCESSFULLY_REDEEMED : 4;
    uint32_t REMOTE_HP : 4;
    uint32_t RESERVED_BITS : 13;

} __attribute__((__packed__)) RUI_TYPEDEF_SENTRY_INFO_T;

/*0x020E*/
typedef struct
{
    uint8_t VULNERABLE_BEGIN : 2;
    uint8_t VULNERABLE_NOW : 1;

} RUI_TYPEDEF_RADAR_INFO_T;

/*0x0303*/
typedef struct
{
    float TARGET_POSITION_X;
    float TARGET_POSITION_Y;
    uint8_t CMD_KEYBOARD;
    uint8_t TARGET_ROBOT_ID;
    uint8_t CMD_SOURCE;

} __attribute__((__packed__)) RUI_TYPEDEF_MAP_COMMAND_T;

/*0x0304*/
typedef struct
{
    int16_t MOUSE_X;
    int16_t MOUSE_Y;
    int16_t MOUSE_Z;
    int8_t LEFT_BUTTON_DOWN;
    int8_t RIGHT_BUTTON_DOWN;
    uint16_t KEYBOARD_VALUE_W : 1;
    uint16_t KEYBOARD_VALUE_S : 1;
    uint16_t KEYBOARD_VALUE_A : 1;
    uint16_t KEYBOARD_VALUE_D : 1;
    uint16_t KEYBOARD_VALUE_SHIFT : 1;
    uint16_t KEYBOARD_VALUE_CTRL : 1;
    uint16_t KEYBOARD_VALUE_Q : 1;
    uint16_t KEYBOARD_VALUE_E : 1;
    uint16_t KEYBOARD_VALUE_R : 1;
    uint16_t KEYBOARD_VALUE_F : 1;
    uint16_t KEYBOARD_VALUE_G : 1;
    uint16_t KEYBOARD_VALUE_Z : 1;
    uint16_t KEYBOARD_VALUE_X : 1;
    uint16_t KEYBOARD_VALUE_C : 1;
    uint16_t KEYBOARD_VALUE_V : 1;
    uint16_t KEYBOARD_VALUE_B : 1;
    uint16_t RESERVED;

} __attribute__((__packed__)) RUI_TYPEDEF_REMOTE_CONTROL_T;

typedef union RUI_TYPEDEF_PACKED
{
    /* data */
    RUI_TYPEDEF_GAME_STATUS_T GAME_STATUS;
    RUI_TYPEDEF_GAME_RESULT_T GAME_RESULT;
    RUI_TYPEDEF_GAME_ROBOT_HP_T GAME_ROBOT_HP;
    RUI_TYPEDEF_EVENT_DATA_T EVENT_DATA;
    RUI_TYPEDEF_EXT_SUPPLY_PROJECTILE_ACTION_T EXT_SUPPLY_PROJECTILE_ACTION;
    RUI_TYPEDEF_REFEREE_WARNING_T referee_warning;
    RUI_TYPEDEF_DART_INFO_T dart_info;
    RUI_TYPEDEF_ROBOT_STATUS_T ROBOT_STATUS;
    RUI_TYPEDEF_POWER_HEAT_DATA_T POWER_HEAT_DATA;
    RUI_TYPEDEF_ROBOT_POS_T ROBOT_POS;
    RUI_TYPEDEF_BUFF_T BUFF;
    RUI_TYPEDEF_AIR_SUPPORT_DATA_T AIR_SUPPORT_DATA;
    RUI_TYPEDEF_HURT_DATA_T HURT_DATA;
    RUI_TYPEDEF_SHOOT_DATA_T SHOOT_DATA;
    RUI_TYPEDEF_PROJECTILE_ALLOWANCE_T PROJECTILE_ALLOWANCE;
    RUI_TYPEDEF_RFID_STATUS_T RFID_STATUS;
    RUI_TYPEDEF_DART_CLIENT_CMD_T DART_CLIENT_CMD;
    RUI_TYPEDEF_GROUND_ROBOT_POSITION_T GROUND_ROBOT_POSITION;
    RUI_TYPEDEF_RADAR_MARK_DATA_T RADAR_MARK_DATA;
    RUI_TYPEDEF_SENTRY_INFO_T SENTRY_INFO;
    RUI_TYPEDEF_RADAR_INFO_T RADAR_INFO;
    RUI_TYPEDEF_MAP_COMMAND_T MAP_COMMAND;
    RUI_TYPEDEF_REMOTE_CONTROL_T REMOTE_CONTROL;
    uint8_t DATA_RX[255];

} __attribute__((__packed__)) RUI_TYPEDEF_READ_DATA_T;

typedef struct
{
    RUI_TYPEDEF_GAME_STATUS_T GAME_STATUS;
    RUI_TYPEDEF_GAME_RESULT_T GAME_RESULT;
    RUI_TYPEDEF_GAME_ROBOT_HP_T GAME_ROBOT_HP;
    RUI_TYPEDEF_EVENT_DATA_T EVENT_DATA;
    RUI_TYPEDEF_EXT_SUPPLY_PROJECTILE_ACTION_T EXT_SUPPLY_PROJECTILE_ACTION;
    RUI_TYPEDEF_REFEREE_WARNING_T REFEREE_WARNING;
    RUI_TYPEDEF_DART_INFO_T DART_INFO;
    RUI_TYPEDEF_ROBOT_STATUS_T ROBOT_STATUS;
    RUI_TYPEDEF_POWER_HEAT_DATA_T POWER_HEAT_DATA;
    RUI_TYPEDEF_ROBOT_POS_T ROBOT_POS;
    RUI_TYPEDEF_BUFF_T BUFF;
    RUI_TYPEDEF_AIR_SUPPORT_DATA_T AIR_SUPPORT_DATA;
    RUI_TYPEDEF_HURT_DATA_T HURT_DATA;
    RUI_TYPEDEF_SHOOT_DATA_T SHOOT_DATA;
    RUI_TYPEDEF_PROJECTILE_ALLOWANCE_T PROJECTILE_ALLOWANCE;
    RUI_TYPEDEF_RFID_STATUS_T RFID_STATUS;
    RUI_TYPEDEF_DART_CLIENT_CMD_T DART_CLIENT_CMD;
    RUI_TYPEDEF_GROUND_ROBOT_POSITION_T GROUND_ROBOT_POSITION;
    RUI_TYPEDEF_RADAR_MARK_DATA_T RADAR_MARK_DATA;
    RUI_TYPEDEF_SENTRY_INFO_T SENTRY_INFO;
    RUI_TYPEDEF_RADAR_INFO_T RADAR_INFO;
    RUI_TYPEDEF_MAP_COMMAND_T MAP_COMMAND;
    RUI_TYPEDEF_REMOTE_CONTROL_T REMOTE_CONTROL;
} __attribute__((__packed__)) RUI_TYPEDEF_JUDGE;

typedef union
{
    struct
    {
        /* data */
        RUI_TYPEDEF_FRAME_HEADER_TF FRAME_HEADER;
        uint16_t CMD_ID;
        RUI_TYPEDEF_READ_DATA_T READ_DATA;
        uint16_t FRAME_TAIL;
    } __attribute__((__packed__)) RX_DATA;

    uint8_t DATA[255];

} RUI_TYPEDEF_ALL_RX_DATA_T;

uint8_t RUI_F_READ_DATA_SYSTEM(RUI_TYPEDEF_ALL_RX_DATA_T *ALL_RX_DATA, RUI_TYPEDEF_JUDGE *USER_DATA);

#endif // !__JUDGEMENT_H__

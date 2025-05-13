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
 *            ���汣��     ����崻�     ����BUG
 * 
 *        ��Ի:  
 *                д��¥��д�ּ䣬д�ּ������Ա��  
 *                ������Աд�������ó��򻻾�Ǯ��  
 *                ����ֻ�����������������������ߣ�  
 *                ���������ո��գ����������긴�ꡣ  
 *                ��Ը�������Լ䣬��Ը�Ϲ��ϰ�ǰ��  
 *                ���۱�������Ȥ���������г���Ա��  
 *                ����Ц��߯��񲣬��Ц�Լ���̫����  
 *                ��������Ư���ã��ĸ���ó���Ա��
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
    game_state      = 0x0001 ,  //����״̬���ݣ��̶� 1Hz Ƶ�ʷ���
    Match_results   = 0x0002 ,  //����������ݣ�����������������
    Robot_HP        = 0x0003 ,  //������Ѫ�����ݣ��̶� 3Hz Ƶ��
    Venue_Events    = 0x0101 ,  //�����¼����ݣ��̶� 1Hz Ƶ��
    Supply_action   = 0x0102 ,  //����վ������ʶ���ݣ�����վ�����ͷ�ʱ��������
    Referee_warning = 0x0104 ,  //���о������ݣ������з�/�и�ʱ�������ͣ�����ʱ���� 1Hz Ƶ�ʷ���
    Dart_fire       = 0x0105 ,  //���ڷ���������ݣ��̶� 1Hz Ƶ�ʷ���
    Robot_performan = 0x0201 ,  //������������ϵ���ݣ��̶�10Hz Ƶ�ʷ���  
    time_power      = 0x0202 ,  //ʵʱ�����������ݣ��̶� 50HzƵ�ʷ���
    Robot_location  = 0x0203 ,  //������λ�����ݣ��ڱ������˵ķ���Ƶ��Ϊ 10Hz����������˵ķ���Ƶ��Ϊ 1Hz
    Robot_buff      = 0x0204 ,  //�������������ݣ��̶� 3Hz Ƶ�ʷ���
    Air_support     = 0x0205 ,  //����֧Ԯʱ�����ݣ��̶� 1Hz Ƶ�ʷ���
    Damage_status   = 0x0206 ,  //�˺�״̬���ݣ��˺���������
    time_shooting   = 0x0207 ,  //ʵʱ������ݣ����跢�����
    Allowable_ammo  = 0x0208 ,  //�������������̶� 10Hz Ƶ�ʷ���
    RFID            = 0x0209 ,  //������ RFID ״̬���̶� 3Hz Ƶ�ʷ���
    Dart_directives = 0x020A ,  //����ѡ�ֶ�ָ�����ݣ�����բ�����ߺ�̶� 3Hz Ƶ�ʷ���
    Ground_location = 0x020B ,  //���������λ�����ݣ��̶� 1HzƵ�ʷ���
    Radar_Marking   = 0x020C ,  //�״��ǽ������ݣ��̶� 1Hz Ƶ�ʷ���
    Route_Informat  = 0x020D ,  //�ڱ��������������Ϣͬ�����̶� 1Hz Ƶ�ʷ���
    Radar_Informat  = 0x020E ,  //�״�����������Ϣͬ�����̶�1Hz Ƶ�ʷ���
    Minimap         = 0x0303 ,  //ѡ�ֶ�С��ͼ�������ݣ�ѡ�ֶ˴�������
    remote          = 0x0304 ,  //����ң�����ݣ��̶� 30Hz Ƶ�ʷ���
};

/*0x0001*/
typedef struct __packed
{
    uint8_t game_type : 4;      //��������
    uint8_t game_progress : 4;  //��ǰ�����׶�
    uint16_t stage_remain_time; //��ǰ�׶�ʣ��ʱ�䣬��λ����
    uint64_t SyncTimeStamp;     //UNIX ʱ�䣬����������ȷ���ӵ�����ϵͳ�� NTP ����������Ч

}game_status_t;

/*0x0002*/
typedef struct __packed
{
    uint8_t winner;     //��ʤ��

}game_result_t;

/*0x0003*/
typedef struct __packed
{
    uint16_t red_1_robot_HP;    //�� 1 Ӣ�ۻ�����Ѫ�������û�����δ�ϳ����߱����£���Ѫ��Ϊ 0
    uint16_t red_2_robot_HP;    //�� 2 ���̻�����Ѫ��
    uint16_t red_3_robot_HP;    //�� 3 ����������Ѫ�� 
    uint16_t red_4_robot_HP;    //�� 4 ����������Ѫ�� 
    uint16_t red_5_robot_HP;    //�� 5 ����������Ѫ�� 
    uint16_t red_7_robot_HP;    //�� 7 �ڱ�������Ѫ��
    uint16_t red_outpost_HP;    //�췽ǰ��վѪ�� 
    uint16_t red_base_HP;       //�췽����Ѫ�� 
    uint16_t blue_1_robot_HP;   //�� 1 Ӣ�ۻ�����Ѫ�� 
    uint16_t blue_2_robot_HP;   //�� 2 ���̻�����Ѫ�� 
    uint16_t blue_3_robot_HP;   //�� 3 ����������Ѫ��
    uint16_t blue_4_robot_HP;   //�� 4 ����������Ѫ�� 
    uint16_t blue_5_robot_HP;   //�� 5 ����������Ѫ�� 
    uint16_t blue_7_robot_HP;   //�� 7 �ڱ�������Ѫ�� 
    uint16_t blue_outpost_HP;   //����ǰ��վѪ�� 
    uint16_t blue_base_HP;      //��������Ѫ�� 

}game_robot_HP_t;

/*0x0101*/
typedef struct __packed
{
    uint32_t refueling_Blood : 1;           //��������վǰ��Ѫ���ռ��״̬��1 Ϊ��ռ�� 
    uint32_t supply_Blood : 1;              //��������վ�ڲ���Ѫ���ռ��״̬��1 Ϊ��ռ�� 
    uint32_t Reserved_bits_1 : 1;           //������������ռ��״̬��1 Ϊ��ռ�죨�� RMUL ���ã�
    uint32_t buff_activated : 1;            //�����������ؼ�����ռ��״̬��1 Ϊ��ռ�� 
    uint32_t buff_small : 1;                //����С�������صļ���״̬��1 Ϊ�Ѽ���
    uint32_t buff_big : 1;                  //�������������صļ���״̬��1 Ϊ�Ѽ��� 
    uint32_t Circular_Heights : 2;          //�������θߵص�ռ��״̬��1 Ϊ������ռ�죬2 Ϊ���Է�ռ�� 
    uint32_t Trapezoidal_heights_3 : 2;     //�������θߵص�ռ��״̬��1 Ϊ������ռ�죬2 Ϊ���Է�ռ�� 
    uint32_t Trapezoidal_heights_4 : 2;     //�������θߵص�ռ��״̬��1 Ϊ������ռ�죬2 Ϊ���Է�ռ�� 
    uint32_t Virtual_Shield : 7;            //�����������⻤�ܵ�ʣ��ֵ�ٷֱȣ��������룬����������
    uint32_t Dart_hit_time : 9;             //�������һ�λ��м���ǰ��վ����ص�ʱ�䣨0-420������Ĭ��Ϊ0
    uint32_t Dart_hit_aim : 2;              //�������һ�λ��м���ǰ��վ����صľ���Ŀ�꣬����Ĭ��Ϊ 0��1 Ϊ����ǰ��վ��2 Ϊ���л��ع̶�Ŀ�꣬3 Ϊ���л������Ŀ�� 
    uint32_t Reserved_bits_2 : 2;           //����������ռ�������0 Ϊδ��ռ�죬1 Ϊ������ռ�죬2 Ϊ���Է�ռ�죬3 Ϊ��˫��ռ�졣���� RMUL ���ã�

}event_data_t;

/*0x0102*/
typedef struct __packed
{
    uint8_t reserved;                   //����
    uint8_t supply_robot_id;            //���������� ID��
    uint8_t supply_projectile_step;     //�����ڿ���״̬�� 
    uint8_t supply_projectile_num;      //����������

} ext_supply_projectile_action_t;

/*0x0104*/
typedef struct __packed
{
    uint8_t level;                  //�������һ���ܵ��з��ĵȼ�
    uint8_t offending_robot_id;     //�������һ���ܵ��з���Υ������� ID
    uint8_t count;                  //�������һ���ܵ��з���Υ������˶�Ӧ�з��ȼ���Υ�����

}referee_warning_t;

/*0x0105*/
typedef struct __packed
{
    uint8_t dart_remaining_time;    //�������ڷ���ʣ��ʱ�䣬��λ����
    uint8_t Dart_aimed : 2;         //���һ�μ������ڻ��е�Ŀ�꣬����Ĭ��Ϊ 0��1 Ϊ����ǰ��վ��2 Ϊ���л��ع̶�Ŀ�꣬3 Ϊ���л������Ŀ�� 
    uint8_t Dart_success : 3;       //�Է���������е�Ŀ���ۼƱ����м���������Ĭ��Ϊ 0������Ϊ 4 
    uint8_t Dart_aiming : 2;        //���ڴ�ʱѡ���Ļ���Ŀ�꣬����Ĭ�ϻ�δѡ��/ѡ��ǰ��վʱΪ 0��ѡ�л��ع̶�Ŀ��Ϊ 1��ѡ�л������Ŀ��Ϊ 2 
    uint8_t Reserved_bits : 1;      //����

}dart_info_t;

/*0x0201*/
typedef struct __packed
{
    uint8_t robot_id;                               //�������� ID
    uint8_t robot_level;                            //�����˵ȼ�
    uint16_t current_HP;                            //�����˵�ǰѪ��
    uint16_t maximum_HP;                            //������Ѫ������
    uint16_t shooter_barrel_cooling_value;          //������ǹ������ÿ����ȴֵ
    uint16_t shooter_barrel_heat_limit;             //������ǹ����������
    uint16_t chassis_power_limit;                   //�����˵��̹�������
    uint8_t power_management_gimbal_output : 1;     //gimbal �������0 Ϊ�������1 Ϊ 24V ��� 
    uint8_t power_management_chassis_output : 1;    //chassis �������0 Ϊ�������1 Ϊ 24V ��� 
    uint8_t power_management_shooter_output : 1;    //shooter �������0 Ϊ�������1 Ϊ 24V ���

}robot_status_t;

/*0x0202*/
typedef struct __packed
{
    uint16_t chassis_voltage;               //��Դ����ģ��� chassis �������ѹ����λ��mV��
    uint16_t chassis_current;               //��Դ����ģ��� chassis �������������λ��mA�� 
    float chassis_power;                    //���̹��ʣ���λ��W�� 
    uint16_t buffer_energy;                 //������������λ��J��
    uint16_t shooter_17mm_1_barrel_heat;    //�� 1 �� 17mm ���������ǹ������
    uint16_t shooter_17mm_2_barrel_heat;    //�� 2 �� 17mm ���������ǹ������ 
    uint16_t shooter_42mm_barrel_heat;      //42mm ���������ǹ������ 

}power_heat_data_t;

/*0x0203*/
typedef struct __packed
{
    float x;        //��������λ�� x ���꣬��λ��m 
    float y;        //��������λ�� y ���꣬��λ��m 
    float angle;    //�������˲���ģ��ĳ��򣬵�λ���ȡ�����Ϊ 0 �� 

}robot_pos_t;

/*0x0204*/
typedef struct __packed
{
    uint8_t recovery_buff;      //�����˻�Ѫ���棨�ٷֱȣ�ֵΪ 10 ��ʾÿ��ָ�Ѫ�����޵� 10%�� 
    uint8_t cooling_buff;       //������ǹ����ȴ���ʣ�ֱ��ֵ��ֵΪ 5 ��ʾ 5 ����ȴ��
    uint8_t defence_buff;       //�����˷������棨�ٷֱȣ�ֵΪ 50 ��ʾ 50%�������棩 
    uint8_t vulnerability_buff; //�����˸��������棨�ٷֱȣ�ֵΪ 30 ��ʾ-30%�������棩
    uint16_t attack_buff;       //�����˹������棨�ٷֱȣ�ֵΪ 50 ��ʾ 50%�������棩 

}buff_t;

/*0x0205*/
typedef struct __packed
{
    uint8_t airforce_status;    //���л�����״̬��0 Ϊ������ȴ��1 Ϊ��ȴ��ϣ�2 Ϊ���ڿ���֧Ԯ�� 
    uint8_t time_remain;        //��״̬��ʣ��ʱ��

}air_support_data_t;

/*0x0206*/
typedef struct __packed
{
    uint8_t armor_id : 4;               //����Ѫԭ��Ϊװ��ģ�鱻���蹥������ײ�������߻����ģ������ʱ���� 4 bit ��ɵ���ֵΪװ��ģ������ģ��� ID ��ţ�������ԭ���¿�Ѫʱ������ֵΪ 0 
    uint8_t HP_deduction_reason : 4;    //Ѫ���仯���� 

}hurt_data_t;

/*0x0207*/
typedef struct __packed
{
    uint8_t bullet_type;            //��������
    uint8_t shooter_number;         //������� ID
    uint8_t launching_frequency;    //�������٣���λ��Hz�� 
    float initial_speed;            //������ٶȣ���λ��m/s��

}shoot_data_t;

/*0x0208*/
typedef struct __packed
{
    uint16_t projectile_allowance_17mm;     //17mm ��������������
    uint16_t projectile_allowance_42mm;     //42mm ��������������
    uint16_t remaining_gold_coin;           //ʣ��������

}projectile_allowance_t;

/*0x0209*/
typedef struct __packed
{
    uint32_t bit0 : 1;      //������������� 
    uint32_t bit1 : 1;      //�������θߵ������ 
    uint32_t bit2 : 1;      //�Է����θߵ������
    uint32_t bit3 : 1;      //���� R3/B3 ���θߵ������ 
    uint32_t bit4 : 1;      //�Է� R3/B3 ���θߵ������
    uint32_t bit5 : 1;      //���� R4/B4 ���θߵ������ 
    uint32_t bit6 : 1;      //�Է� R4/B4 ���θߵ������ 
    uint32_t bit7 : 1;      //�����������ؼ���� 
    uint32_t bit8 : 1;      //������������㣨��������һ�����ǰ��
    uint32_t bit9 : 1;      //������������㣨��������һ����º�
    uint32_t bit10 : 1;     //�Է���������㣨�����Է�һ�����ǰ��
    uint32_t bit11 : 1;     //�Է���������㣨�����Է�һ����º�
    uint32_t bit12 : 1;     //����ǰ��վ����� 
    uint32_t bit13 : 1;     //������Ѫ�㣨��⵽��һ����Ϊ���
    uint32_t bit14 : 1;     //�����ڱ�Ѳ����
    uint32_t bit15 : 1;     //�Է��ڱ�Ѳ���� 
    uint32_t bit16 : 1;     //��������Դ������� 
    uint32_t bit17 : 1;     //�Է�����Դ������� 
    uint32_t bit18 : 1;     //�����һ��� 
    uint32_t bit19 : 1;     //��������㣨�� RMUL ���ã�
    uint32_t bit20_31 : 13; //����

}rfid_status_t;

/*0x020A*/
typedef struct __packed
{
    uint8_t dart_launch_opening_status;     //��ǰ���ڷ���վ��״̬��1���ر� 2�����ڿ������߹ر��� 0���Ѿ����� 
    uint8_t reserved;                       //����λ
    uint16_t target_change_time;            //�л�����Ŀ��ʱ�ı���ʣ��ʱ�䣬��λ���룬��/δ�л�������Ĭ��Ϊ 0�� 
    uint16_t latest_launch_cmd_time;        //���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λ���룬��ʼֵΪ 0�� 

}dart_client_cmd_t;

/*0x020B*/
typedef struct __packed
{
    float hero_x;           //����Ӣ�ۻ�����λ�� x �����꣬��λ��m 
    float hero_y;           //����Ӣ�ۻ�����λ�� y �����꣬��λ��m
    float engineer_x;       //�������̻�����λ�� x �����꣬��λ��m 
    float engineer_y;       //�������̻�����λ�� y �����꣬��λ��m
    float standard_3_x;     //���� 3 �Ų���������λ�� x �����꣬��λ��m 
    float standard_3_y;     //���� 3 �Ų���������λ�� y �����꣬��λ��m 
    float standard_4_x;     //���� 4 �Ų���������λ�� x �����꣬��λ��m 
    float standard_4_y;     //���� 4 �Ų���������λ�� y �����꣬��λ��m 
    float standard_5_x;     //���� 5 �Ų���������λ�� x �����꣬��λ��m 
    float standard_5_y;     //���� 5 �Ų���������λ�� y �����꣬��λ��m 

}ground_robot_position_t;

/*0x020C*/
typedef struct __packed
{
    uint8_t mark_hero_progress;         //�Է�Ӣ�ۻ����˱���ǽ��ȣ�0-120 
    uint8_t mark_engineer_progress;     //�Է����̻����˱���ǽ��ȣ�0-120
    uint8_t mark_standard_3_progress;   //�Է� 3 �Ų��������˱���ǽ��ȣ�0-120 
    uint8_t mark_standard_4_progress;   //�Է� 4 �Ų��������˱���ǽ��ȣ�0-120 
    uint8_t mark_standard_5_progress;   //�Է� 5 �Ų��������˱���ǽ��ȣ�0-120 
    uint8_t mark_sentry_progress;       //�Է��ڱ������˱���ǽ��ȣ�0-120 

}radar_mark_data_t;

/*0x020D*/
typedef struct __packed
{
    uint32_t successfully_redeemed : 11;        //��Զ�̶һ��⣬�ڱ��ɹ��һ��ķ�����������Ϊ 0�����ڱ��ɹ��һ�һ���������󣬸�ֵ����Ϊ�ڱ��ɹ��һ��ķ�����ֵ�� 
    uint32_t remote_successfully_redeemed : 4;  //�ڱ��ɹ�Զ�̶һ��������Ĵ���������Ϊ 0�����ڱ��ɹ�Զ�̶һ��������󣬸�ֵ����Ϊ�ڱ��ɹ�Զ�̶һ��������Ĵ����� 
    uint32_t remote_HP : 4;                     //�ڱ��ɹ�Զ�̶һ�Ѫ���Ĵ���������Ϊ 0�����ڱ��ɹ�Զ�̶һ�Ѫ���󣬸�ֵ����Ϊ�ڱ��ɹ�Զ�̶һ�Ѫ���Ĵ����� 
    uint32_t Reserved_bits : 13;                //����

}sentry_info_t;

/*0x020E*/
typedef struct __packed
{
    uint8_t vulnerable_begin : 2;   //�״��Ƿ�ӵ�д���˫�����˵Ļ��ᣬ����Ϊ 0����ֵΪ�״�ӵ�д���˫�����˵Ļ��ᣬ����Ϊ 2 
    uint8_t vulnerable_now : 1;     //�Է��Ƿ����ڱ�����˫������
	uint8_t vulnerable_receive : 5;
}radar_info_t;

/*0x0303*/
typedef struct __packed
{
    float target_position_x;    //Ŀ��λ�� x �����꣬��λ m  ������Ŀ������� ID ʱ����ֵΪ 0 
    float target_position_y;    //Ŀ��λ�� y �����꣬��λ m  ������Ŀ������� ID ʱ����ֵΪ 0 
    uint8_t cmd_keyboard;       //��̨�ְ��µļ��̰���ͨ�ü�ֵ
    uint8_t target_robot_id;    //�Է������� ID 
    uint8_t cmd_source;         //��Ϣ��Դ ID 

}map_command_t;

/*0x0304*/
typedef struct __packed
{
    int16_t mouse_x;                    //��� x ���ƶ��ٶȣ���ֵ��ʶ�����ƶ�
    int16_t mouse_y;                    //��� y ���ƶ��ٶȣ���ֵ��ʶ�����ƶ�
    int16_t mouse_z;                    //�������ƶ��ٶȣ���ֵ��ʶ������
    int8_t left_button_down;            //�������Ƿ��£�0 Ϊδ���£�1 Ϊ����
    int8_t right_button_down;           //����Ҽ��Ƿ��£�0 Ϊδ���£�1 Ϊ����
    uint16_t keyboard_value_W : 1;      //W ��
    uint16_t keyboard_value_S : 1;      //S �� 
    uint16_t keyboard_value_A : 1;      //A �� 
    uint16_t keyboard_value_D : 1;      //D �� 
    uint16_t keyboard_value_Shift : 1;  //Shift �� 
    uint16_t keyboard_value_Ctrl : 1;   //Ctrl �� 
    uint16_t keyboard_value_Q : 1;      //Q �� 
    uint16_t keyboard_value_E : 1;      //E �� 
    uint16_t keyboard_value_R : 1;      //R ��
    uint16_t keyboard_value_F : 1;      //F ��
    uint16_t keyboard_value_G : 1;      //G �� 
    uint16_t keyboard_value_Z : 1;      //Z �� 
    uint16_t keyboard_value_X : 1;      //X �� 
    uint16_t keyboard_value_C : 1;      //C �� 
    uint16_t keyboard_value_V : 1;      //V �� 
    uint16_t keyboard_value_B : 1;      //B �� 
    uint16_t reserved;                  //����λ 

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
    uint16_t  DataLenth;	    //����λ����
	uint8_t   Seq;				//�����
    uint8_t   CRC8;				//crc8λУ��
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

    uint8_t Data[256];
    
}ALL_RX_Data_T;

void Read_Data_first(ALL_RX_Data_T *ALL_RX_Data , User_Data_T *user_data , uint16_t length);
uint16_t Read_Data_system(ALL_RX_Data_T *ALL_RX_Data , User_Data_T *user_data);

extern ALL_RX_Data_T ALL_RX;
extern User_Data_T user_data;

#endif

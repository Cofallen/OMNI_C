/*
 * ......................................&&.........................
 * ....................................&&&..........................
 * .................................&&&&............................
 * ...............................&&&&..............................
 * .............................&&&&&&..............................
 * ...........................&&&&&&....&&&..&&&&&&&&&&&&&&&........
 * ..................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............
 * ................&...&&&&&&&&&&&&&&&&&&&&&&&&&&&&.................
 * .......................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.........
 * ...................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...............
 * ..................&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&............
 * ...............&&&&&@  &&&&&&&&&&..&&&&&&&&&&&&&&&&&&&...........
 * ..............&&&&&&&&&&&&&&&.&&....&&&&&&&&&&&&&..&&&&&.........
 * ..........&&&&&&&&&&&&&&&&&&...&.....&&&&&&&&&&&&&...&&&&........
 * ........&&&&&&&&&&&&&&&&&&&.........&&&&&&&&&&&&&&&....&&&.......
 * .......&&&&&&&&.....................&&&&&&&&&&&&&&&&.....&&......
 * ........&&&&&.....................&&&&&&&&&&&&&&&&&&.............
 * ..........&...................&&&&&&&&&&&&&&&&&&&&&&&............
 * ................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&............
 * ..................&&&&&&&&&&&&&&&&&&&&&&&&&&&&..&&&&&............
 * ..............&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&....&&&&&............
 * ...........&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&......&&&&............
 * .........&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.........&&&&............
 * .......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...........&&&&............
 * ......&&&&&&&&&&&&&&&&&&&...&&&&&&...............&&&.............
 * .....&&&&&&&&&&&&&&&&............................&&..............
 * ....&&&&&&&&&&&&&&&.................&&...........................
 * ...&&&&&&&&&&&&&&&.....................&&&&......................
 * ...&&&&&&&&&&.&&&........................&&&&&...................
 * ..&&&&&&&&&&&..&&..........................&&&&&&&...............
 * ..&&&&&&&&&&&&...&............&&&.....&&&&...&&&&&&&.............
 * ..&&&&&&&&&&&&&.................&&&.....&&&&&&&&&&&&&&...........
 * ..&&&&&&&&&&&&&&&&..............&&&&&&&&&&&&&&&&&&&&&&&&.........
 * ..&&.&&&&&&&&&&&&&&&&&.........&&&&&&&&&&&&&&&&&&&&&&&&&&&.......
 * ...&&..&&&&&&&&&&&&.........&&&&&&&&&&&&&&&&...&&&&&&&&&&&&......
 * ....&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...........&&&&&&&&.....
 * .......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............&&&&&&&....
 * .......&&&&&.&&&&&&&&&&&&&&&&&&..&&&&&&&&...&..........&&&&&&....
 * ........&&&.....&&&&&&&&&&&&&.....&&&&&&&&&&...........&..&&&&...
 * .......&&&........&&&.&&&&&&&&&.....&&&&&.................&&&&...
 * .......&&&...............&&&&&&&.......&&&&&&&&............&&&...
 * ........&&...................&&&&&&.........................&&&..
 * .........&.....................&&&&........................&&....
 * ...............................&&&.......................&&......
 * ................................&&......................&&.......
 * .................................&&..............................
 * ..................................&..............................
 */

/*
 * @version: 
 * @Author: Eugene
 * @Date: 2024-01-08 16:57:53
 * @LastEditors: Andy
 * @LastEditTime: 2024-01-23 14:34:06
 */
#ifndef __WRITE_DATA_H
#define __WRITE_DATA_H

#include "usart.h" 

#define X_CENTER 960
#define Y_CENTER 540

#define BLUE 0
#define RED  1

#define JudgeBufferLength       255
#define JudgeFrameHeader        0xA5        //帧头 
#define FrameHeader_Len         5		//帧头校验长度
#define client_custom_Length    28

/*由于按位切分，所以改枚举为预定义*/
    /*图形数据*/
        /*图形配置 1*/
            /*bit 0-2：图形操作：*/
            #define d_nothing       0
            #define d_increase      1
            #define d_revise        2
            #define d_Delete        3
            /*Bit 3-5：图形类型*/
            #define d_straight line 0
            #define d_rectangle     1
            #define d_Full_circle   2
            #define d_elliptical    3
            #define d_circular_arc  4
            #define d_Floating      5
            #define d_Integer       6
            #define d_character     7
            /*Bit 6-9：图层数，*/
            #define d_zero          0
            #define d_one           1  
            #define d_two           2
            #define d_three         3
            #define d_fore          4
            #define d_five          5
            #define d_six           6
            #define d_seven         7
            #define d_eight         8
            #define d_nine          9
            
/*cmd_id枚举*/
enum Judege_Cmd_ID_Typdef
{
    Robot_interaction_data      = 0x0301,    //机器人交互数据
    Customer_controller         = 0x0302,    //自定义控制器交互数据
    minimap_RX                  = 0x0305,    //客户端小地图接收雷达信息
    Non_link                    = 0x0306,    //操作手可使用自定义控制器模拟键鼠操作选手端
    Route_coordinates           = 0x0307,    //向对应的操作手选手端发送路径坐标数据
    Customized_messages         = 0x0308     //己方机器人可通过常规链路向己方任意选手端发送自定义的消息
};

/*机器人间交互数据ID枚举*/
enum data_cmd_id_Typdef
{
    erase                       = 0x0100,   //客户端删除图形
    Draw_graphics_1             = 0x0101,   //客户端绘制一个图形
    Draw_graphics_2             = 0x0102,   //客户端绘制二个图形
    Draw_graphics_5             = 0x0103,   //客户端绘制五个图形
    Draw_graphics_7             = 0x0104,   //客户端绘制七个图形
    Draw_character              = 0x0110,   //客户端绘制字符图形
    Sentinels_decisions         = 0x0120,   //哨兵自主决策
    Radars_decisions             = 0x0121    //雷达自主决策
};

/*机器人ID和操作手ID枚举*/
enum Robot_ID_and_User_ID_Typdef
{
    Robot_hero_red              = 1,   //英雄(红)
    Robot_engineer_red          = 2,   //工程(红)
    Robot_infantry_3_red        = 3,   //步兵3(红)
    Robot_infantry_4_red        = 4,   //步兵4(红)
    Robot_infantry_5_red        = 5,   //步兵5(红)
    Robot_air_red               = 6,   //空中(红)
    Robot_sentinel_red          = 7,   //哨兵(红)
    Robot_radar_red             = 9,   //雷达站(红)

    Robot_hero_blue             = 101,   //英雄(蓝)
    Robot_engineer_blue         = 102,   //工程(蓝)
    Robot_infantry_3_blue       = 103,   //步兵3(蓝)
    Robot_infantry_4_blue       = 104,   //步兵4(蓝)
    Robot_infantry_5_blue       = 105,   //步兵5(蓝)
    Robot_air_blue              = 106,   //空中(蓝)
    Robot_sentinel_blue         = 107,   //哨兵(蓝)
    Robot_radar_blue            = 109,   //雷达站(蓝)

    User_hero_red               = 0x0101,   //英雄操作手客户端(红)
    User_engineer_red           = 0x0102,   //工程操作手客户端(红)
    User_infantry_3_red         = 0x0103,   //步兵3操作手客户端(红)
    User_infantry_4_red         = 0x0104,   //步兵4操作手客户端(红)
    User_infantry_5_red         = 0x0105,   //步兵5操作手客户端(红)
    User_air_red                = 0x0106,   //云台手操作手客户端(红)

    User_hero_blue              = 0x0165,   //英雄操作手客户端(蓝)
    User_engineer_blue          = 0x0166,   //工程操作手客户端(蓝),
    User_infantry_3_blue        = 0x0167,   //步兵3操作手客户端(蓝),
    User_infantry_4_blue        = 0x0168,   //步兵3操作手客户端(蓝),
    User_infantry_5_blue        = 0x0169,   //步兵3操作手客户端(蓝),
    User_air_blue               = 0x016A,   //云台手操作手客户端(蓝),

};

/*删除图形操作枚举*/
enum operate_tpye_Typdef
{
    do_nothing                   = 0x00,   //空操作
    delete_layer                 = 0x01,   //删除图层
    delete_all                   = 0x02    //删除所有
};

enum layer_Typdef
{
    zero                        = 0x00,
    one                         = 0x01,    
    two                         = 0x02,
    three                       = 0x03,
    fore                        = 0x04,
    five                        = 0x05,
    six                         = 0x06,
    seven                       = 0x07,
    eight                       = 0x08,
    nine                        = 0x09
};

//帧头
typedef struct __packed 
{
    uint8_t   SOF;				//0xA5
    uint16_t  DataLenth;	    //数据位长度
	uint8_t   Seq;				//包序号
    uint8_t   CRC8;				//crc8位校验
}frame_header_Typdef;

typedef struct __packed
{
	/* data */
	enum operate_tpye_Typdef operate_tpye; 
	enum layer_Typdef layer;
}graphic_delete_InitTypeDef;

typedef struct __packed 
{
	/* data */
	uint8_t figure_name[3]; 
	uint32_t operate_tpye:3; 
	uint32_t figure_tpye:3; 
	uint32_t layer:4; 
	uint32_t color:4; 
	uint32_t details_a:9;
	uint32_t details_b:9;
	uint32_t width:10; 
	uint32_t start_x:11; 
	uint32_t start_y:11; 
	uint32_t details_c:10; 
	uint32_t details_d:11; 
	uint32_t details_e:11; 
}graphic_data_InitTypeDef;

typedef struct __packed 
{
    uint32_t Read_strip_resurrection : 1;
    uint32_t Redeem_resurrection : 1;
    uint32_t Redeem_amount : 11;
    uint32_t Ranged_exchange_shots : 4;
    uint32_t Remotely_exchange_HP : 4;
    uint32_t retain : 11;

}sentry_cmd_InitTypeDef;

typedef struct __packed
{
    uint8_t radar_decisions;
}radar_cmd_InitTypeDef;

typedef union 
{
    graphic_delete_InitTypeDef graphic_delete;
    graphic_data_InitTypeDef graphic_data_1;
	graphic_data_InitTypeDef graphic_data_2[2];
	graphic_data_InitTypeDef graphic_data_5[5];
	graphic_data_InitTypeDef graphic_data_7[7];
	graphic_data_InitTypeDef graphic_data_char;
    sentry_cmd_InitTypeDef sentry_cmd;
    radar_cmd_InitTypeDef radar_cmd;

}User_UI_Typdef;

typedef struct __packed 
{
    /* data */
    enum data_cmd_id_Typdef data_cmd_id;
    enum Robot_ID_and_User_ID_Typdef sender_ID;
    enum Robot_ID_and_User_ID_Typdef receiver_ID;
    User_UI_Typdef User_UI;

}Data_T;

/*UI*/
typedef struct __packed 
{
    /* data */
    frame_header_Typdef frame_header;
    enum Judege_Cmd_ID_Typdef cmd_id;
    Data_T Data;
    uint16_t frame_tail;
	
}ALL_TX_Typdef;

/*Radar*/
typedef struct __packed
{
    enum Robot_ID_and_User_ID_Typdef target_robot_id;
    float target_position_x;
    float target_position_y;
    
}map_robot_data_InitTypeDef;

typedef struct __packed
{
    /* data */
    frame_header_Typdef frame_header;
    enum Judege_Cmd_ID_Typdef cmd_id;
    map_robot_data_InitTypeDef Data;
    uint16_t frame_tail;

}Radar_Typdef;

/*Customize the controller*/
typedef union
{
    uint8_t data[30];

}custom_robot_data_InitTypeDef;

typedef struct __packed
{
    /* data */
    frame_header_Typdef frame_header;
    enum Judege_Cmd_ID_Typdef cmd_id;
    custom_robot_data_InitTypeDef Data;
    uint16_t frame_tail;

}controller_Typdef;

/*非链路数据*/
typedef struct __packed
{
    uint16_t key_value;
    uint16_t x_position:12;
    uint16_t mouse_left:4;
    uint16_t y_position:12;
    uint16_t mouse_right:4;
    uint16_t reserved;

}custom_client_data_InitTypeDef;

typedef struct __packed
{
    /* data */
    frame_header_Typdef frame_header;
    enum Judege_Cmd_ID_Typdef cmd_id;
    custom_client_data_InitTypeDef Data;
    uint16_t frame_tail;

}Non_link_data_Typdef;


/*哨兵回传*/
typedef struct __packed
{
    uint8_t intention;
    uint16_t start_position_x;
    uint16_t start_position_y;
    int8_t delta_x[49];
    int8_t delta_y[49];
    uint16_t sender_id;

}map_data_InitTypeDef;

typedef struct __packed
{
    /* data */
    frame_header_Typdef frame_header;
    enum Judege_Cmd_ID_Typdef cmd_id;
    map_data_InitTypeDef Data;
    uint16_t frame_tail;

}Route_data_Typdef;

/*任意车回传*/
typedef struct __packed
{ 
    uint16_t sender_id;
    uint16_t receiver_id;
    uint16_t user_data[30];

} custom_info_InitTypeDef;

typedef struct __packed
{
    /* data */
    frame_header_Typdef frame_header;
    enum Judege_Cmd_ID_Typdef cmd_id;
    custom_info_InitTypeDef Data;
    uint16_t frame_tail;

}Customized_Typdef;

uint8_t Get_CRC8_Check_Sum( uint8_t *pchMessage, uint16_t dwLength, uint8_t ucCRC8 );
uint32_t Verify_CRC8_Check_Sum( uint8_t *pchMessage, uint16_t dwLength);
void Append_CRC8_Check_Sum( uint8_t *pchMessage, uint16_t dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);
void UI_TX_Write_graphics_Delete_Init(graphic_delete_InitTypeDef *graphic_delete);
void UI_TX_Write_graphics_Delete(	ALL_TX_Typdef *UI_TX ,
									enum Robot_ID_and_User_ID_Typdef sender_ID ,
									enum Robot_ID_and_User_ID_Typdef receiver_ID ,
									graphic_delete_InitTypeDef *graphic_delete , 
                                    uint8_t *data_au8);
void UI_TX_Write_graphics_One_Init(graphic_data_InitTypeDef *graphic_data_1);
void UI_TX_Write_graphics_One(	ALL_TX_Typdef *UI_TX ,
								enum Robot_ID_and_User_ID_Typdef sender_ID ,
								enum Robot_ID_and_User_ID_Typdef receiver_ID ,
								graphic_data_InitTypeDef *graphic_data_1 , 
                                uint8_t *data_au8);
void UI_TX_Write_graphics_Two_Init(graphic_data_InitTypeDef *graphic_data_2);
void UI_TX_Write_graphics_Two(	ALL_TX_Typdef *UI_TX ,
								enum Robot_ID_and_User_ID_Typdef sender_ID ,
								enum Robot_ID_and_User_ID_Typdef receiver_ID ,
								graphic_data_InitTypeDef *graphic_data_2 , 
                                uint8_t *data_au8);
void UI_TX_Write_graphics_Five_Init(graphic_data_InitTypeDef *graphic_data_5);
void UI_TX_Write_graphics_Five(	ALL_TX_Typdef *UI_TX ,
								enum Robot_ID_and_User_ID_Typdef sender_ID ,
								enum Robot_ID_and_User_ID_Typdef receiver_ID ,
								graphic_data_InitTypeDef *graphic_data_5 ,
								uint8_t *data_au8);
void UI_TX_Write_graphics_Seven_Init(graphic_data_InitTypeDef *graphic_data_7);
void UI_TX_Write_graphics_Seven(	ALL_TX_Typdef *UI_TX ,
									enum Robot_ID_and_User_ID_Typdef sender_ID ,
									enum Robot_ID_and_User_ID_Typdef receiver_ID ,
									graphic_data_InitTypeDef *graphic_data_7 ,
                                    uint8_t *data_au8);
void UI_TX_Write_char_Init(graphic_data_InitTypeDef *graphic_data_char);
void UI_TX_Write_char(	ALL_TX_Typdef *UI_TX ,
						enum Robot_ID_and_User_ID_Typdef sender_ID ,
						enum Robot_ID_and_User_ID_Typdef receiver_ID ,
						graphic_data_InitTypeDef *graphic_data_char ,
                        uint8_t *data_au8);
void Route_decisions_Init(sentry_cmd_InitTypeDef *Data);
void Route_decisions(	ALL_TX_Typdef *UI_TX ,
						enum Robot_ID_and_User_ID_Typdef sender_ID ,
						enum Robot_ID_and_User_ID_Typdef receiver_ID ,
						sentry_cmd_InitTypeDef *Data ,
                        uint8_t *data_au8);
void Radar_decisions_Init(radar_cmd_InitTypeDef *Data);
void Radar_decisions(	ALL_TX_Typdef *UI_TX ,
						enum Robot_ID_and_User_ID_Typdef sender_ID ,
						enum Robot_ID_and_User_ID_Typdef receiver_ID ,
						radar_cmd_InitTypeDef *Data ,
                        uint8_t *data_au8);
void Radar_UART_Transmit_Init(map_robot_data_InitTypeDef *Data);
void Radar_UART_Transmit(Radar_Typdef *Radar , map_robot_data_InitTypeDef *Data , uint8_t *data_au8);
void controller_UART_Transmit_Init(custom_robot_data_InitTypeDef *Data);
void controller_UART_Transmit(controller_Typdef *controller , custom_robot_data_InitTypeDef *Data , uint8_t *data_au8);
void Non_link_data_UART_Transmit_Init(custom_client_data_InitTypeDef *Data);
void Non_link_data_UART_Transmit(Non_link_data_Typdef *Non_link_data , custom_client_data_InitTypeDef *Data , uint8_t *data_au8);
void Route_data_UART_Transmit_Init(map_data_InitTypeDef *Data , uint8_t frequency);
void Route_data_UART_Transmit(Route_data_Typdef *Route_data , map_data_InitTypeDef *Data , uint8_t *data_au8);
void Customized_UART_Transmit_Init(custom_info_InitTypeDef *Data);
void Customized_UART_Transmit(Customized_Typdef *Customized , custom_info_InitTypeDef *Data , uint8_t *data_au8);

#endif

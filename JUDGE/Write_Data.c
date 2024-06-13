/*
 *                                                     __----~~~~~~~~~~~------___
 *                                    .  .   ~~//====......          __--~ ~~
 *                    -.            \_|//     |||\\  ~~~~~~::::... /~
 *                 ___-==_       _-~o~  \/    |||  \\            _/~~-
 *         __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\        _/~
 *     _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /
 *   .~       .~       |   \\ -_    /  /-   /   ||      \   /
 *  /  ____  /         |     \\ ~-_/  /|- _/   .||       \ /
 *  |~~    ~~|--~~~~--_ \     ~==-/   | \~--===~~        .\
 *           '         ~-|      /|    |-~\~~       __--~~
 *                       |-~~-_/ |    |   ~\_   _-~            /\
 *                            /  \     \__   \/~                \__
 *                        _--~ _/ | .-~~____--~-/                  ~~==.
 *                       ((->/~   '.|||' -_|    ~~-/ ,              . _||
 *                                  -_     ~\      ~~---l__i__i__i--~~_/
 *                                  _-~-__   ~)  \--______________--~~
 *                                //.-~~~-~_--~- |-------~~~~~~~~
 *                                       //.-~~~--\
 *                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *                               神兽保佑            永无BUG
 */

#include "Write_Data.h"
#include "stdint.h"
#include "string.h"
#include "CRC_DJI.h"




/***********************************        战队主控->裁判系统       ***********************************/

/***********************************     战队主控->裁判系统   UI     ***********************************/
#define test 1

void UI_TX_Write_graphics_Delete_Init(graphic_delete_InitTypeDef *graphic_delete)
{
	graphic_delete->layer = test;
	graphic_delete->operate_tpye = test;
}

/*调用该函数时先写一个UI_TX_Write_graphics_Delete_Init函数，初始化graphic_delete结构体的内容*/
void UI_TX_Write_graphics_Delete(	ALL_TX_Typdef *UI_TX ,
									enum Robot_ID_and_User_ID_Typdef sender_ID ,
									enum Robot_ID_and_User_ID_Typdef receiver_ID ,
									graphic_delete_InitTypeDef *graphic_delete , 
									uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.graphic_delete);
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = erase;
	UI_TX->Data.receiver_ID = receiver_ID;
	UI_TX->Data.sender_ID = sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);
	
	/*********************自定义部分开始***********************/
	UI_TX->Data.User_UI.graphic_delete.layer = graphic_delete->layer;
	UI_TX->Data.User_UI.graphic_delete.operate_tpye = graphic_delete->operate_tpye;
	/*********************自定义部分结束***********************/
	
	memcpy(	&data_au8[13] , 
			&UI_TX->Data.User_UI.graphic_delete , 
			sizeof(UI_TX->Data.User_UI.graphic_delete));
	
	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.graphic_delete));

	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_delete)-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_delete)-2]);
			
	//	HAL_UART_Transmit_DMA
}

void UI_TX_Write_graphics_One_Init(graphic_data_InitTypeDef *graphic_data_1)
{
	graphic_data_1->color = test;
	graphic_data_1->details_a = test;
	graphic_data_1->details_b = test;
	graphic_data_1->details_c = test;
	graphic_data_1->figure_name[0] = test;
	graphic_data_1->figure_tpye = test;
	graphic_data_1->layer = test;
	graphic_data_1->operate_tpye = test;
	graphic_data_1->details_d = test;
	graphic_data_1->details_e = test;
	graphic_data_1->start_x = test;
	graphic_data_1->start_y = test;
	graphic_data_1->width = test;
}

void UI_TX_Write_graphics_One(	ALL_TX_Typdef *UI_TX ,
								enum Robot_ID_and_User_ID_Typdef sender_ID ,
								enum Robot_ID_and_User_ID_Typdef receiver_ID ,
								graphic_data_InitTypeDef *graphic_data_1 ,
								uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.graphic_data_1);
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = Draw_graphics_1;
	UI_TX->Data.receiver_ID = receiver_ID;
	UI_TX->Data.sender_ID = sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);
	
	/*********************自定义部分开始***********************/
	UI_TX->Data.User_UI.graphic_data_1.color = graphic_data_1->color;
	UI_TX->Data.User_UI.graphic_data_1.details_a = graphic_data_1->details_a;
	UI_TX->Data.User_UI.graphic_data_1.details_b = graphic_data_1->details_b;
	UI_TX->Data.User_UI.graphic_data_1.details_c = graphic_data_1->details_c;
	UI_TX->Data.User_UI.graphic_data_1.figure_name[0] = graphic_data_1->figure_name[0];
	UI_TX->Data.User_UI.graphic_data_1.figure_tpye = graphic_data_1->figure_tpye;
	UI_TX->Data.User_UI.graphic_data_1.layer = graphic_data_1->layer;
	UI_TX->Data.User_UI.graphic_data_1.operate_tpye = graphic_data_1->operate_tpye;
	UI_TX->Data.User_UI.graphic_data_1.details_d = graphic_data_1->details_d;
	UI_TX->Data.User_UI.graphic_data_1.details_e = graphic_data_1->details_e;
	UI_TX->Data.User_UI.graphic_data_1.start_x = graphic_data_1->start_x;
	UI_TX->Data.User_UI.graphic_data_1.start_y = graphic_data_1->start_y;
	UI_TX->Data.User_UI.graphic_data_1.width = graphic_data_1->width;

	/*********************自定义部分结束***********************/
	
	memcpy(	&data_au8[13] , 
			&UI_TX->Data.User_UI.graphic_data_1 , 
			sizeof(UI_TX->Data.User_UI.graphic_data_1));
	
	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.graphic_data_1));
			
	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)-2]);
	
	//	HAL_UART_Transmit_DMA
}

void UI_TX_Write_graphics_Two_Init(graphic_data_InitTypeDef *graphic_data_2)
{
	graphic_data_2[0].color = test;
	graphic_data_2[0].details_a = test;
	graphic_data_2[0].details_b = test;
	graphic_data_2[0].details_c = test;
	graphic_data_2[0].figure_name[0] = test;
	graphic_data_2[0].figure_tpye = test;
	graphic_data_2[0].layer = test;
	graphic_data_2[0].operate_tpye = test;
	graphic_data_2[0].details_d = test;
	graphic_data_2[0].details_e = test;
	graphic_data_2[0].start_x = test;
	graphic_data_2[0].start_y = test;
	graphic_data_2[0].width = test;
	
	graphic_data_2[1].color = test;
	graphic_data_2[1].details_a = test;
	graphic_data_2[1].details_b = test;
	graphic_data_2[1].details_c = test;
	graphic_data_2[1].figure_name[0] = test;
	graphic_data_2[1].figure_tpye = test;
	graphic_data_2[1].layer = test;
	graphic_data_2[1].operate_tpye = test;
	graphic_data_2[1].details_d = test;
	graphic_data_2[1].details_e = test;
	graphic_data_2[1].start_x = test;
	graphic_data_2[1].start_y = test;
	graphic_data_2[1].width = test;
}

void UI_TX_Write_graphics_Two(	ALL_TX_Typdef *UI_TX ,
								enum Robot_ID_and_User_ID_Typdef sender_ID ,
								enum Robot_ID_and_User_ID_Typdef receiver_ID ,
								graphic_data_InitTypeDef *graphic_data_2 ,
								uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*2;
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = (uint16_t)Draw_graphics_2;
	UI_TX->Data.receiver_ID = (uint16_t)receiver_ID;
	UI_TX->Data.sender_ID = (uint16_t)sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);
	
	/*********************自定义部分开始***********************/
	UI_TX->Data.User_UI.graphic_data_2[0].color = graphic_data_2[0].color;
	UI_TX->Data.User_UI.graphic_data_2[0].details_a = graphic_data_2[0].details_a;
	UI_TX->Data.User_UI.graphic_data_2[0].details_b = graphic_data_2[0].details_b;
	UI_TX->Data.User_UI.graphic_data_2[0].details_c = graphic_data_2[0].details_c;
	UI_TX->Data.User_UI.graphic_data_2[0].figure_name[0] = graphic_data_2[0].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_2[0].figure_tpye = graphic_data_2[0].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_2[0].layer = graphic_data_2[0].layer;
	UI_TX->Data.User_UI.graphic_data_2[0].operate_tpye = graphic_data_2[0].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_2[0].details_d = graphic_data_2[0].details_d;
	UI_TX->Data.User_UI.graphic_data_2[0].details_e = graphic_data_2[0].details_e;
	UI_TX->Data.User_UI.graphic_data_2[0].start_x = graphic_data_2[0].start_x;
	UI_TX->Data.User_UI.graphic_data_2[0].start_y = graphic_data_2[0].start_y;
	UI_TX->Data.User_UI.graphic_data_2[0].width = graphic_data_2[0].width;
	
	UI_TX->Data.User_UI.graphic_data_2[1].color = graphic_data_2[1].color;
	UI_TX->Data.User_UI.graphic_data_2[1].details_a = graphic_data_2[1].details_a;
	UI_TX->Data.User_UI.graphic_data_2[1].details_b = graphic_data_2[1].details_b;
	UI_TX->Data.User_UI.graphic_data_2[1].details_c = graphic_data_2[1].details_c;
	UI_TX->Data.User_UI.graphic_data_2[1].figure_name[0] = graphic_data_2[1].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_2[1].figure_tpye = graphic_data_2[1].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_2[1].layer = graphic_data_2[1].layer;
	UI_TX->Data.User_UI.graphic_data_2[1].operate_tpye = graphic_data_2[1].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_2[1].details_d = graphic_data_2[1].details_d;
	UI_TX->Data.User_UI.graphic_data_2[1].details_e = graphic_data_2[1].details_e;
	UI_TX->Data.User_UI.graphic_data_2[1].start_x = graphic_data_2[1].start_x;
	UI_TX->Data.User_UI.graphic_data_2[1].start_y = graphic_data_2[1].start_y;
	UI_TX->Data.User_UI.graphic_data_2[1].width = graphic_data_2[1].width;

	/*********************自定义部分结束***********************/
	
	memcpy(	&data_au8[13] , 
			UI_TX->Data.User_UI.graphic_data_2 , 
			sizeof(UI_TX->Data.User_UI.graphic_data_1)*2);
	
	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*2);
	
	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*2-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*2-2]);
			
	//	HAL_UART_Transmit_DMA
}

void UI_TX_Write_graphics_Five_Init(graphic_data_InitTypeDef *graphic_data_5)
{
	graphic_data_5[0].color = test;
	graphic_data_5[0].details_a = test;
	graphic_data_5[0].details_b = test;
	graphic_data_5[0].details_c = test;
	graphic_data_5[0].figure_name[0] = test;
	graphic_data_5[0].figure_tpye = test;
	graphic_data_5[0].layer = test;
	graphic_data_5[0].operate_tpye = test;
	graphic_data_5[0].details_d = test;
	graphic_data_5[0].details_e = test;
	graphic_data_5[0].start_x = test;
	graphic_data_5[0].start_y = test;
	graphic_data_5[0].width = test;
	
	graphic_data_5[1].color = test;
	graphic_data_5[1].details_a = test;
	graphic_data_5[1].details_b = test;
	graphic_data_5[1].details_c = test;
	graphic_data_5[1].figure_name[0] = test;
	graphic_data_5[1].figure_tpye = test;
	graphic_data_5[1].layer = test;
	graphic_data_5[1].operate_tpye = test;
	graphic_data_5[1].details_d = test;
	graphic_data_5[1].details_e = test;
	graphic_data_5[1].start_x = test;
	graphic_data_5[1].start_y = test;
	graphic_data_5[1].width = test;
	
	graphic_data_5[2].color = test;
	graphic_data_5[2].details_a = test;
	graphic_data_5[2].details_b = test;
	graphic_data_5[2].details_c = test;
	graphic_data_5[2].figure_name[0] = test;
	graphic_data_5[2].figure_tpye = test;
	graphic_data_5[2].layer = test;
	graphic_data_5[2].operate_tpye = test;
	graphic_data_5[2].details_d = test;
	graphic_data_5[2].details_e = test;
	graphic_data_5[2].start_x = test;
	graphic_data_5[2].start_y = test;
	graphic_data_5[2].width = test;
	
	graphic_data_5[3].color = test;
	graphic_data_5[3].details_a = test;
	graphic_data_5[3].details_b = test;
	graphic_data_5[3].details_c = test;
	graphic_data_5[3].figure_name[0] = test;
	graphic_data_5[3].figure_tpye = test;
	graphic_data_5[3].layer = test;
	graphic_data_5[3].operate_tpye = test;
	graphic_data_5[3].details_d = test;
	graphic_data_5[3].details_e = test;
	graphic_data_5[3].start_x = test;
	graphic_data_5[3].start_y = test;
	graphic_data_5[3].width = test;
	
	graphic_data_5[4].color = test;
	graphic_data_5[4].details_a = test;
	graphic_data_5[4].details_b = test;
	graphic_data_5[4].details_c = test;
	graphic_data_5[4].figure_name[0] = test;
	graphic_data_5[4].figure_tpye = test;
	graphic_data_5[4].layer = test;
	graphic_data_5[4].operate_tpye = test;
	graphic_data_5[4].details_d = test;
	graphic_data_5[4].details_e = test;
	graphic_data_5[4].start_x = test;
	graphic_data_5[4].start_y = test;
	graphic_data_5[4].width = test;
}

void UI_TX_Write_graphics_Five(	ALL_TX_Typdef *UI_TX ,
								enum Robot_ID_and_User_ID_Typdef sender_ID ,
								enum Robot_ID_and_User_ID_Typdef receiver_ID ,
								graphic_data_InitTypeDef *graphic_data_5 ,
								uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*5;
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = (uint16_t)Draw_graphics_5;
	UI_TX->Data.receiver_ID = (uint16_t)receiver_ID;
	UI_TX->Data.sender_ID = (uint16_t)sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);
	
	/*********************自定义部分开始***********************/
	UI_TX->Data.User_UI.graphic_data_5[0].color = graphic_data_5[0].color;
	UI_TX->Data.User_UI.graphic_data_5[0].details_a = graphic_data_5[0].details_a;
	UI_TX->Data.User_UI.graphic_data_5[0].details_b = graphic_data_5[0].details_b;
	UI_TX->Data.User_UI.graphic_data_5[0].details_c = graphic_data_5[0].details_c;
	UI_TX->Data.User_UI.graphic_data_5[0].figure_name[0] = graphic_data_5[0].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_5[0].figure_tpye = graphic_data_5[0].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_5[0].layer = graphic_data_5[0].layer;
	UI_TX->Data.User_UI.graphic_data_5[0].operate_tpye = graphic_data_5[0].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_5[0].details_d = graphic_data_5[0].details_d;
	UI_TX->Data.User_UI.graphic_data_5[0].details_e = graphic_data_5[0].details_e;
	UI_TX->Data.User_UI.graphic_data_5[0].start_x = graphic_data_5[0].start_x;
	UI_TX->Data.User_UI.graphic_data_5[0].start_y = graphic_data_5[0].start_y;
	UI_TX->Data.User_UI.graphic_data_5[0].width = graphic_data_5[0].width;
	
	UI_TX->Data.User_UI.graphic_data_5[1].color = graphic_data_5[1].color;
	UI_TX->Data.User_UI.graphic_data_5[1].details_a = graphic_data_5[1].details_a;
	UI_TX->Data.User_UI.graphic_data_5[1].details_b = graphic_data_5[1].details_b;
	UI_TX->Data.User_UI.graphic_data_5[1].details_c = graphic_data_5[1].details_c;
	UI_TX->Data.User_UI.graphic_data_5[1].figure_name[0] = graphic_data_5[1].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_5[1].figure_tpye = graphic_data_5[1].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_5[1].layer = graphic_data_5[1].layer;
	UI_TX->Data.User_UI.graphic_data_5[1].operate_tpye = graphic_data_5[1].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_5[1].details_d = graphic_data_5[1].details_d;
	UI_TX->Data.User_UI.graphic_data_5[1].details_e = graphic_data_5[1].details_e;
	UI_TX->Data.User_UI.graphic_data_5[1].start_x = graphic_data_5[1].start_x;
	UI_TX->Data.User_UI.graphic_data_5[1].start_y = graphic_data_5[1].start_y;
	UI_TX->Data.User_UI.graphic_data_5[1].width = graphic_data_5[1].width;
	
	UI_TX->Data.User_UI.graphic_data_5[2].color = graphic_data_5[2].color;
	UI_TX->Data.User_UI.graphic_data_5[2].details_a = graphic_data_5[2].details_a;
	UI_TX->Data.User_UI.graphic_data_5[2].details_b = graphic_data_5[2].details_b;
	UI_TX->Data.User_UI.graphic_data_5[2].details_c = graphic_data_5[2].details_c;
	UI_TX->Data.User_UI.graphic_data_5[2].figure_name[0] = graphic_data_5[2].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_5[2].figure_tpye = graphic_data_5[2].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_5[2].layer = graphic_data_5[2].layer;
	UI_TX->Data.User_UI.graphic_data_5[2].operate_tpye = graphic_data_5[2].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_5[2].details_d = graphic_data_5[2].details_d;
	UI_TX->Data.User_UI.graphic_data_5[2].details_e = graphic_data_5[2].details_e;
	UI_TX->Data.User_UI.graphic_data_5[2].start_x = graphic_data_5[2].start_x;
	UI_TX->Data.User_UI.graphic_data_5[2].start_y = graphic_data_5[2].start_y;
	UI_TX->Data.User_UI.graphic_data_5[2].width = graphic_data_5[2].width;
	
	UI_TX->Data.User_UI.graphic_data_5[3].color = graphic_data_5[3].color;
	UI_TX->Data.User_UI.graphic_data_5[3].details_a = graphic_data_5[3].details_a;
	UI_TX->Data.User_UI.graphic_data_5[3].details_b = graphic_data_5[3].details_b;
	UI_TX->Data.User_UI.graphic_data_5[3].details_c = graphic_data_5[3].details_c;
	UI_TX->Data.User_UI.graphic_data_5[3].figure_name[0] = graphic_data_5[3].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_5[3].figure_tpye = graphic_data_5[3].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_5[3].layer = graphic_data_5[3].layer;
	UI_TX->Data.User_UI.graphic_data_5[3].operate_tpye = graphic_data_5[3].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_5[3].details_d = graphic_data_5[3].details_d;
	UI_TX->Data.User_UI.graphic_data_5[3].details_e = graphic_data_5[3].details_e;
	UI_TX->Data.User_UI.graphic_data_5[3].start_x = graphic_data_5[3].start_x;
	UI_TX->Data.User_UI.graphic_data_5[3].start_y = graphic_data_5[3].start_y;
	UI_TX->Data.User_UI.graphic_data_5[3].width = graphic_data_5[3].width;
	
	UI_TX->Data.User_UI.graphic_data_5[4].color = graphic_data_5[4].color;
	UI_TX->Data.User_UI.graphic_data_5[4].details_a = graphic_data_5[4].details_a;
	UI_TX->Data.User_UI.graphic_data_5[4].details_b = graphic_data_5[4].details_b;
	UI_TX->Data.User_UI.graphic_data_5[4].details_c = graphic_data_5[4].details_c;
	UI_TX->Data.User_UI.graphic_data_5[4].figure_name[0] = graphic_data_5[4].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_5[4].figure_tpye = graphic_data_5[4].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_5[4].layer = graphic_data_5[4].layer;
	UI_TX->Data.User_UI.graphic_data_5[4].operate_tpye = graphic_data_5[4].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_5[4].details_d = graphic_data_5[4].details_d;
	UI_TX->Data.User_UI.graphic_data_5[4].details_e = graphic_data_5[4].details_e;
	UI_TX->Data.User_UI.graphic_data_5[4].start_x = graphic_data_5[4].start_x;
	UI_TX->Data.User_UI.graphic_data_5[4].start_y = graphic_data_5[4].start_y;
	UI_TX->Data.User_UI.graphic_data_5[4].width = graphic_data_5[4].width;

	/*********************自定义部分结束***********************/
	
	memcpy(	&data_au8[13] , 
			UI_TX->Data.User_UI.graphic_data_5 , 
			sizeof(UI_TX->Data.User_UI.graphic_data_1)*5);
	
	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*5);
	
	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*5-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*5-2]);
			
	//	HAL_UART_Transmit_DMA
}

void UI_TX_Write_graphics_Seven_Init(graphic_data_InitTypeDef *graphic_data_7)
{
	graphic_data_7[0].color = test;
	graphic_data_7[0].details_a = test;
	graphic_data_7[0].details_b = test;
	graphic_data_7[0].details_c = test;
	graphic_data_7[0].figure_name[0] = test;
	graphic_data_7[0].figure_tpye = test;
	graphic_data_7[0].layer = test;
	graphic_data_7[0].operate_tpye = test;
	graphic_data_7[0].details_d = test;
	graphic_data_7[0].details_e = test;
	graphic_data_7[0].start_x = test;
	graphic_data_7[0].start_y = test;
	graphic_data_7[0].width = test;
	
	graphic_data_7[1].color = test;
	graphic_data_7[1].details_a = test;
	graphic_data_7[1].details_b = test;
	graphic_data_7[1].details_c = test;
	graphic_data_7[1].figure_name[0] = test;
	graphic_data_7[1].figure_tpye = test;
	graphic_data_7[1].layer = test;
	graphic_data_7[1].operate_tpye = test;
	graphic_data_7[1].details_d = test;
	graphic_data_7[1].details_e = test;
	graphic_data_7[1].start_x = test;
	graphic_data_7[1].start_y = test;
	graphic_data_7[1].width = test;
	
	graphic_data_7[2].color = test;
	graphic_data_7[2].details_a = test;
	graphic_data_7[2].details_b = test;
	graphic_data_7[2].details_c = test;
	graphic_data_7[2].figure_name[0] = test;
	graphic_data_7[2].figure_tpye = test;
	graphic_data_7[2].layer = test;
	graphic_data_7[2].operate_tpye = test;
	graphic_data_7[2].details_d = test;
	graphic_data_7[2].details_e = test;
	graphic_data_7[2].start_x = test;
	graphic_data_7[2].start_y = test;
	graphic_data_7[2].width = test;
	
	graphic_data_7[3].color = test;
	graphic_data_7[3].details_a = test;
	graphic_data_7[3].details_b = test;
	graphic_data_7[3].details_c = test;
	graphic_data_7[3].figure_name[0] = test;
	graphic_data_7[3].figure_tpye = test;
	graphic_data_7[3].layer = test;
	graphic_data_7[3].operate_tpye = test;
	graphic_data_7[3].details_d = test;
	graphic_data_7[3].details_e = test;
	graphic_data_7[3].start_x = test;
	graphic_data_7[3].start_y = test;
	graphic_data_7[3].width = test;
	
	graphic_data_7[4].color = test;
	graphic_data_7[4].details_a = test;
	graphic_data_7[4].details_b = test;
	graphic_data_7[4].details_c = test;
	graphic_data_7[4].figure_name[0] = test;
	graphic_data_7[4].figure_tpye = test;
	graphic_data_7[4].layer = test;
	graphic_data_7[4].operate_tpye = test;
	graphic_data_7[4].details_d = test;
	graphic_data_7[4].details_e = test;
	graphic_data_7[4].start_x = test;
	graphic_data_7[4].start_y = test;
	graphic_data_7[4].width = test;
	
	graphic_data_7[5].color = test;
	graphic_data_7[5].details_a = test;
	graphic_data_7[5].details_b = test;
	graphic_data_7[5].details_c = test;
	graphic_data_7[5].figure_name[0] = test;
	graphic_data_7[5].figure_tpye = test;
	graphic_data_7[5].layer = test;
	graphic_data_7[5].operate_tpye = test;
	graphic_data_7[5].details_d = test;
	graphic_data_7[5].details_e = test;
	graphic_data_7[5].start_x = test;
	graphic_data_7[5].start_y = test;
	graphic_data_7[5].width = test;
	
	graphic_data_7[6].color = test;
	graphic_data_7[6].details_a = test;
	graphic_data_7[6].details_b = test;
	graphic_data_7[6].details_c = test;
	graphic_data_7[6].figure_name[0] = test;
	graphic_data_7[6].figure_tpye = test;
	graphic_data_7[6].layer = test;
	graphic_data_7[6].operate_tpye = test;
	graphic_data_7[6].details_d = test;
	graphic_data_7[6].details_e = test;
	graphic_data_7[6].start_x = test;
	graphic_data_7[6].start_y = test;
	graphic_data_7[6].width = test;
}

void UI_TX_Write_graphics_Seven(	ALL_TX_Typdef *UI_TX ,
									enum Robot_ID_and_User_ID_Typdef sender_ID ,
									enum Robot_ID_and_User_ID_Typdef receiver_ID ,
									graphic_data_InitTypeDef *graphic_data_7 ,
									uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*7;
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = (uint16_t)Draw_graphics_7;
	UI_TX->Data.receiver_ID = (uint16_t)receiver_ID;
	UI_TX->Data.sender_ID = (uint16_t)sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);
	
	/*********************自定义部分开始***********************/
	UI_TX->Data.User_UI.graphic_data_7[0].color = graphic_data_7[0].color;
	UI_TX->Data.User_UI.graphic_data_7[0].details_a = graphic_data_7[0].details_a;
	UI_TX->Data.User_UI.graphic_data_7[0].details_b = graphic_data_7[0].details_b;
	UI_TX->Data.User_UI.graphic_data_7[0].details_c = graphic_data_7[0].details_c;
	UI_TX->Data.User_UI.graphic_data_7[0].figure_name[0] = graphic_data_7[0].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_7[0].figure_tpye = graphic_data_7[0].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_7[0].layer = graphic_data_7[0].layer;
	UI_TX->Data.User_UI.graphic_data_7[0].operate_tpye = graphic_data_7[0].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_7[0].details_d = graphic_data_7[0].details_d;
	UI_TX->Data.User_UI.graphic_data_7[0].details_e = graphic_data_7[0].details_e;
	UI_TX->Data.User_UI.graphic_data_7[0].start_x = graphic_data_7[0].start_x;
	UI_TX->Data.User_UI.graphic_data_7[0].start_y = graphic_data_7[0].start_y;
	UI_TX->Data.User_UI.graphic_data_7[0].width = graphic_data_7[0].width;
	
	UI_TX->Data.User_UI.graphic_data_7[1].color = graphic_data_7[1].color;
	UI_TX->Data.User_UI.graphic_data_7[1].details_a = graphic_data_7[1].details_a;
	UI_TX->Data.User_UI.graphic_data_7[1].details_b = graphic_data_7[1].details_b;
	UI_TX->Data.User_UI.graphic_data_7[1].details_c = graphic_data_7[1].details_c;
	UI_TX->Data.User_UI.graphic_data_7[1].figure_name[0] = graphic_data_7[1].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_7[1].figure_tpye = graphic_data_7[1].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_7[1].layer = graphic_data_7[1].layer;
	UI_TX->Data.User_UI.graphic_data_7[1].operate_tpye = graphic_data_7[1].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_7[1].details_d = graphic_data_7[1].details_d;
	UI_TX->Data.User_UI.graphic_data_7[1].details_e = graphic_data_7[1].details_e;
	UI_TX->Data.User_UI.graphic_data_7[1].start_x = graphic_data_7[1].start_x;
	UI_TX->Data.User_UI.graphic_data_7[1].start_y = graphic_data_7[1].start_y;
	UI_TX->Data.User_UI.graphic_data_7[1].width = graphic_data_7[1].width;
	
	UI_TX->Data.User_UI.graphic_data_7[2].color = graphic_data_7[2].color;
	UI_TX->Data.User_UI.graphic_data_7[2].details_a = graphic_data_7[2].details_a;
	UI_TX->Data.User_UI.graphic_data_7[2].details_b = graphic_data_7[2].details_b;
	UI_TX->Data.User_UI.graphic_data_7[2].details_c = graphic_data_7[2].details_c;
	UI_TX->Data.User_UI.graphic_data_7[2].figure_name[0] = graphic_data_7[2].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_7[2].figure_tpye = graphic_data_7[2].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_7[2].layer = graphic_data_7[2].layer;
	UI_TX->Data.User_UI.graphic_data_7[2].operate_tpye = graphic_data_7[2].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_7[2].details_d = graphic_data_7[2].details_d;
	UI_TX->Data.User_UI.graphic_data_7[2].details_e = graphic_data_7[2].details_e;
	UI_TX->Data.User_UI.graphic_data_7[2].start_x = graphic_data_7[2].start_x;
	UI_TX->Data.User_UI.graphic_data_7[2].start_y = graphic_data_7[2].start_y;
	UI_TX->Data.User_UI.graphic_data_7[2].width = graphic_data_7[2].width;
	
	UI_TX->Data.User_UI.graphic_data_7[3].color = graphic_data_7[3].color;
	UI_TX->Data.User_UI.graphic_data_7[3].details_a = graphic_data_7[3].details_a;
	UI_TX->Data.User_UI.graphic_data_7[3].details_b = graphic_data_7[3].details_b;
	UI_TX->Data.User_UI.graphic_data_7[3].details_c = graphic_data_7[3].details_c;
	UI_TX->Data.User_UI.graphic_data_7[3].figure_name[0] = graphic_data_7[3].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_7[3].figure_tpye = graphic_data_7[3].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_7[3].layer = graphic_data_7[3].layer;
	UI_TX->Data.User_UI.graphic_data_7[3].operate_tpye = graphic_data_7[3].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_7[3].details_d = graphic_data_7[3].details_d;
	UI_TX->Data.User_UI.graphic_data_7[3].details_e = graphic_data_7[3].details_e;
	UI_TX->Data.User_UI.graphic_data_7[3].start_x = graphic_data_7[3].start_x;
	UI_TX->Data.User_UI.graphic_data_7[3].start_y = graphic_data_7[3].start_y;
	UI_TX->Data.User_UI.graphic_data_7[3].width = graphic_data_7[3].width;
	
	UI_TX->Data.User_UI.graphic_data_7[4].color = graphic_data_7[4].color;
	UI_TX->Data.User_UI.graphic_data_7[4].details_a = graphic_data_7[4].details_a;
	UI_TX->Data.User_UI.graphic_data_7[4].details_b = graphic_data_7[4].details_b;
	UI_TX->Data.User_UI.graphic_data_7[4].details_c = graphic_data_7[4].details_c;
	UI_TX->Data.User_UI.graphic_data_7[4].figure_name[0] = graphic_data_7[4].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_7[4].figure_tpye = graphic_data_7[4].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_7[4].layer = graphic_data_7[4].layer;
	UI_TX->Data.User_UI.graphic_data_7[4].operate_tpye = graphic_data_7[4].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_7[4].details_d = graphic_data_7[4].details_d;
	UI_TX->Data.User_UI.graphic_data_7[4].details_e = graphic_data_7[4].details_e;
	UI_TX->Data.User_UI.graphic_data_7[4].start_x = graphic_data_7[4].start_x;
	UI_TX->Data.User_UI.graphic_data_7[4].start_y = graphic_data_7[4].start_y;
	UI_TX->Data.User_UI.graphic_data_7[4].width = graphic_data_7[4].width;

	UI_TX->Data.User_UI.graphic_data_7[5].color = graphic_data_7[5].color;
	UI_TX->Data.User_UI.graphic_data_7[5].details_a = graphic_data_7[5].details_a;
	UI_TX->Data.User_UI.graphic_data_7[5].details_b = graphic_data_7[5].details_b;
	UI_TX->Data.User_UI.graphic_data_7[5].details_c = graphic_data_7[5].details_c;
	UI_TX->Data.User_UI.graphic_data_7[5].figure_name[0] = graphic_data_7[5].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_7[5].figure_tpye = graphic_data_7[5].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_7[5].layer = graphic_data_7[5].layer;
	UI_TX->Data.User_UI.graphic_data_7[5].operate_tpye = graphic_data_7[5].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_7[5].details_d = graphic_data_7[5].details_d;
	UI_TX->Data.User_UI.graphic_data_7[5].details_e = graphic_data_7[5].details_e;
	UI_TX->Data.User_UI.graphic_data_7[5].start_x = graphic_data_7[5].start_x;
	UI_TX->Data.User_UI.graphic_data_7[5].start_y = graphic_data_7[5].start_y;
	UI_TX->Data.User_UI.graphic_data_7[5].width = graphic_data_7[5].width;
	
	UI_TX->Data.User_UI.graphic_data_7[6].color = graphic_data_7[6].color;
	UI_TX->Data.User_UI.graphic_data_7[6].details_a = graphic_data_7[6].details_a;
	UI_TX->Data.User_UI.graphic_data_7[6].details_b = graphic_data_7[6].details_b;
	UI_TX->Data.User_UI.graphic_data_7[6].details_c = graphic_data_7[6].details_c;
	UI_TX->Data.User_UI.graphic_data_7[6].figure_name[0] = graphic_data_7[6].figure_name[0];
	UI_TX->Data.User_UI.graphic_data_7[6].figure_tpye = graphic_data_7[6].figure_tpye;
	UI_TX->Data.User_UI.graphic_data_7[6].layer = graphic_data_7[6].layer;
	UI_TX->Data.User_UI.graphic_data_7[6].operate_tpye = graphic_data_7[6].operate_tpye;
	UI_TX->Data.User_UI.graphic_data_7[6].details_d = graphic_data_7[6].details_d;
	UI_TX->Data.User_UI.graphic_data_7[6].details_e = graphic_data_7[6].details_e;
	UI_TX->Data.User_UI.graphic_data_7[6].start_x = graphic_data_7[6].start_x;
	UI_TX->Data.User_UI.graphic_data_7[6].start_y = graphic_data_7[6].start_y;
	UI_TX->Data.User_UI.graphic_data_7[6].width = graphic_data_7[6].width;
	/*********************自定义部分结束***********************/
	
	memcpy(	&data_au8[13] , 
			UI_TX->Data.User_UI.graphic_data_7 , 
			sizeof(UI_TX->Data.User_UI.graphic_data_1)*7);
	
	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*7);
	
	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*7-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_1)*7-2]);
			
	//	HAL_UART_Transmit_DMA
}

void UI_TX_Write_char_Init(graphic_data_InitTypeDef *graphic_data_char)
{
	graphic_data_char->color = test;
	graphic_data_char->details_a = test;
	graphic_data_char->details_b = test;
	graphic_data_char->details_c = test;
	graphic_data_char->figure_name[0] = test;
	graphic_data_char->figure_tpye = test;
	graphic_data_char->layer = test;
	graphic_data_char->operate_tpye = test;
	graphic_data_char->details_d = test;
	graphic_data_char->details_e = test;
	graphic_data_char->start_x = test;
	graphic_data_char->start_y = test;
	graphic_data_char->width = test;
}

void UI_TX_Write_char(	ALL_TX_Typdef *UI_TX ,
						enum Robot_ID_and_User_ID_Typdef sender_ID ,
						enum Robot_ID_and_User_ID_Typdef receiver_ID ,
						graphic_data_InitTypeDef *graphic_data_char ,
						uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.graphic_data_char);
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = Draw_character;
	UI_TX->Data.receiver_ID = receiver_ID;
	UI_TX->Data.sender_ID = sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);
	
	/*********************自定义部分开始***********************/
	UI_TX->Data.User_UI.graphic_data_char.color = graphic_data_char->color;
	UI_TX->Data.User_UI.graphic_data_char.details_a = graphic_data_char->details_a;
	UI_TX->Data.User_UI.graphic_data_char.details_b = graphic_data_char->details_b;
	UI_TX->Data.User_UI.graphic_data_char.details_c = graphic_data_char->details_c;
	UI_TX->Data.User_UI.graphic_data_char.figure_name[0] = graphic_data_char->figure_name[0];
	UI_TX->Data.User_UI.graphic_data_char.figure_tpye = graphic_data_char->figure_tpye;
	UI_TX->Data.User_UI.graphic_data_char.layer = graphic_data_char->layer;
	UI_TX->Data.User_UI.graphic_data_char.operate_tpye = graphic_data_char->operate_tpye;
	UI_TX->Data.User_UI.graphic_data_char.details_d = graphic_data_char->details_d;
	UI_TX->Data.User_UI.graphic_data_char.details_e = graphic_data_char->details_e;
	UI_TX->Data.User_UI.graphic_data_char.start_x = graphic_data_char->start_x;
	UI_TX->Data.User_UI.graphic_data_char.start_y = graphic_data_char->start_y;
	UI_TX->Data.User_UI.graphic_data_char.width = graphic_data_char->width;
	/*********************自定义部分结束***********************/
	
	memcpy(	&data_au8[13] , 
			&UI_TX->Data.User_UI.graphic_data_char , 
			sizeof(UI_TX->Data.User_UI.graphic_data_char));
	
	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.graphic_data_char));
			
	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_char)-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.graphic_data_char)-2]);
	
	//	HAL_UART_Transmit_DMA
}

void Route_decisions_Init(sentry_cmd_InitTypeDef *Data)
{
	Data->Ranged_exchange_shots = test;
	Data->Read_strip_resurrection = test;
	Data->Redeem_amount = test;
	Data->Redeem_resurrection = test;
	Data->Remotely_exchange_HP = test;
	Data->retain = test;
}

void Route_decisions(	ALL_TX_Typdef *UI_TX ,
						enum Robot_ID_and_User_ID_Typdef sender_ID ,
						enum Robot_ID_and_User_ID_Typdef receiver_ID ,
						sentry_cmd_InitTypeDef *Data ,
						uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.sentry_cmd);
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = Sentinels_decisions;
	UI_TX->Data.receiver_ID = receiver_ID;
	UI_TX->Data.sender_ID = sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);

	UI_TX->Data.User_UI.sentry_cmd.Ranged_exchange_shots = Data->Ranged_exchange_shots;
	UI_TX->Data.User_UI.sentry_cmd.Read_strip_resurrection = Data->Read_strip_resurrection;
	UI_TX->Data.User_UI.sentry_cmd.Redeem_amount = Data->Redeem_amount;
	UI_TX->Data.User_UI.sentry_cmd.Redeem_resurrection = Data->Redeem_resurrection;
	UI_TX->Data.User_UI.sentry_cmd.Remotely_exchange_HP = Data->Remotely_exchange_HP;
	UI_TX->Data.User_UI.sentry_cmd.retain = Data->retain;
	memcpy(	&data_au8[13] , 
			&UI_TX->Data.User_UI.sentry_cmd , 
			sizeof(UI_TX->Data.User_UI.sentry_cmd));
	
	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.sentry_cmd));
			
	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.sentry_cmd)-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.sentry_cmd)-2]);
}

void Radar_decisions_Init(radar_cmd_InitTypeDef *Data)
{
	Data->radar_decisions = test;
}

void Radar_decisions(	ALL_TX_Typdef *UI_TX ,
						enum Robot_ID_and_User_ID_Typdef sender_ID ,
						enum Robot_ID_and_User_ID_Typdef receiver_ID ,
						radar_cmd_InitTypeDef *Data ,
						uint8_t *data_au8)
{
	UI_TX->frame_header.SOF = 0xA5;
	UI_TX->frame_header.DataLenth = 6 + sizeof(UI_TX->Data.User_UI.radar_cmd);
	UI_TX->frame_header.Seq = 1;
	memcpy(data_au8 , &UI_TX->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	
	UI_TX->cmd_id = (uint16_t)Robot_interaction_data;
	memcpy(&data_au8[5] , &UI_TX->cmd_id , 2);
	
	UI_TX->Data.data_cmd_id = Radars_decisions;
	UI_TX->Data.receiver_ID = receiver_ID;
	UI_TX->Data.sender_ID = sender_ID;
	memcpy(&data_au8[7] , &UI_TX->Data , 6);

	UI_TX->Data.User_UI.radar_cmd.radar_decisions = Data->radar_decisions;
	data_au8[13] = UI_TX->Data.User_UI.radar_cmd.radar_decisions;

	Append_CRC16_Check_Sum(data_au8 , 13 + sizeof(UI_TX->Data.User_UI.radar_cmd));
			
	UI_TX->frame_tail = (((uint16_t)data_au8[13 + sizeof(UI_TX->Data.User_UI.radar_cmd)-1])<<8) | 
						(data_au8[13 + sizeof(UI_TX->Data.User_UI.radar_cmd)-2]);
}

/***********************************    战队主控->裁判系统    雷达    **********************************/
void Radar_UART_Transmit_Init(map_robot_data_InitTypeDef *Data)
{
	Data->target_robot_id = test;
	Data->target_position_x = test;
	Data->target_position_y = test;
}

void Radar_UART_Transmit(Radar_Typdef *Radar , map_robot_data_InitTypeDef *Data , uint8_t *data_au8)
{
	Radar->frame_header.SOF = 0xA5;
	Radar->frame_header.DataLenth = sizeof(Radar->Data);
	Radar->frame_header.Seq = 1;
	memcpy(data_au8 , &Radar->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	Radar->frame_header.CRC8 = data_au8[4];
	
	Radar->cmd_id = (uint16_t)minimap_RX;
	memcpy(&data_au8[5] , &Radar->cmd_id , 2);

	Radar->Data.target_robot_id = Data->target_robot_id;
	Radar->Data.target_position_x = Data->target_position_x;
	Radar->Data.target_position_y = Data->target_position_y;
	memcpy(&data_au8[7] , &Radar->Data , sizeof(Radar->Data));

	Append_CRC16_Check_Sum(data_au8 , 7 + sizeof(Radar->Data));
	Radar->frame_tail = (((uint16_t)data_au8[7 + sizeof(Radar->Data)-1])<<8) | 
						(data_au8[7 + sizeof(Radar->Data)-2]);
}

/*******************************    战队主控->裁判系统    自定义控制器    *******************************/
void controller_UART_Transmit_Init(custom_robot_data_InitTypeDef *Data)
{
	memset(Data->data , 0 , 30);
}

void controller_UART_Transmit(controller_Typdef *controller , custom_robot_data_InitTypeDef *Data , uint8_t *data_au8)
{
	controller->frame_header.SOF = 0xA5;
	controller->frame_header.DataLenth = sizeof(controller->Data);
	controller->frame_header.Seq = 1;
	memcpy(data_au8 , &controller->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	controller->frame_header.CRC8 = data_au8[4];
	
	controller->cmd_id = (uint16_t)Customer_controller;
	memcpy(&data_au8[5] , &controller->cmd_id , 2);

	memcpy(controller->Data.data , Data->data , 30);
	memcpy(&data_au8[7] , &controller->Data , sizeof(controller->Data));

	Append_CRC16_Check_Sum(data_au8 , 7 + sizeof(controller->Data));
	controller->frame_tail = (((uint16_t)data_au8[7 + sizeof(controller->Data)-1])<<8) | 
								(data_au8[7 + sizeof(controller->Data)-2]);
}

/*********************************    战队主控->裁判系统    模拟键鼠    *********************************/
void Non_link_data_UART_Transmit_Init(custom_client_data_InitTypeDef *Data)
{
	Data->key_value 	= test;
	Data->mouse_left 	= test;
	Data->mouse_right 	= test;
	Data->reserved 		= test;
	Data->x_position 	= test;
	Data->y_position 	= test;
}

void Non_link_data_UART_Transmit(Non_link_data_Typdef *Non_link_data , custom_client_data_InitTypeDef *Data , uint8_t *data_au8)
{
	Non_link_data->frame_header.SOF = 0xA5;
	Non_link_data->frame_header.DataLenth = sizeof(Non_link_data->Data);
	Non_link_data->frame_header.Seq = 1;
	memcpy(data_au8 , &Non_link_data->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	Non_link_data->frame_header.CRC8 = data_au8[4];

	Non_link_data->cmd_id = (uint16_t)Non_link;
	memcpy(&data_au8[5] , &Non_link_data->cmd_id , 2);

	Non_link_data->Data.key_value 	= Data->key_value;
	Non_link_data->Data.mouse_left 	= Data->mouse_left;
	Non_link_data->Data.mouse_right = Data->mouse_right;
	Non_link_data->Data.reserved 	= Data->reserved;
	Non_link_data->Data.x_position 	= Data->x_position;
	Non_link_data->Data.y_position 	= Data->y_position;
	memcpy(&data_au8[7] , &Non_link_data->Data , sizeof(Non_link_data->Data));

	Append_CRC16_Check_Sum(data_au8 , 7 + sizeof(Non_link_data->Data));
	Non_link_data->frame_tail = (((uint16_t)data_au8[7 + sizeof(Non_link_data->Data)-1])<<8) | 
								(data_au8[7 + sizeof(Non_link_data->Data)-2]);
}

/*********************************    战队主控->裁判系统    哨兵回传    *********************************/
void Route_data_UART_Transmit_Init(map_data_InitTypeDef *Data , uint8_t frequency)
{
	Data->delta_x[frequency] = test;
	Data->delta_y[frequency] = test;
	Data->intention = test;
	Data->sender_id = test;
	Data->start_position_x = test;
	Data->start_position_y = test;
}

void Route_data_UART_Transmit(Route_data_Typdef *Route_data , map_data_InitTypeDef *Data , uint8_t *data_au8)
{
	Route_data->frame_header.SOF = 0xA5;
	Route_data->frame_header.DataLenth = sizeof(Route_data->Data);
	Route_data->frame_header.Seq = 1;
	memcpy(data_au8 , &Route_data->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	Route_data->frame_header.CRC8 = data_au8[4];

	Route_data->cmd_id = (uint16_t)Route_coordinates;
	memcpy(&data_au8[5] , &Route_data->cmd_id , 2);

	Route_data->Data.intention = Data->intention;
	Route_data->Data.sender_id = Data->sender_id;
	Route_data->Data.start_position_x = Data->start_position_x;
	Route_data->Data.start_position_y = Data->start_position_y;
	memcpy(Route_data->Data.delta_x , Data->delta_x , 49);
	memcpy(Route_data->Data.delta_y , Data->delta_y , 49);
	memcpy(&data_au8[7] , &Route_data->Data , sizeof(Route_data->Data));

	Append_CRC16_Check_Sum(data_au8 , 7 + sizeof(Route_data->Data));
	Route_data->frame_tail = (((uint16_t)data_au8[7 + sizeof(Route_data->Data)-1])<<8) | 
								(data_au8[7 + sizeof(Route_data->Data)-2]);
}

/*********************************    战队主控->裁判系统    任意车回传    *******************************/
void Customized_UART_Transmit_Init(custom_info_InitTypeDef *Data)
{
	Data->sender_id = test;
	Data->receiver_id = test;
	memset(Data->user_data , 0 , 30);
}

void Customized_UART_Transmit(Customized_Typdef *Customized , custom_info_InitTypeDef *Data , uint8_t *data_au8)
{
	Customized->frame_header.SOF = 0xA5;
	Customized->frame_header.DataLenth = sizeof(Customized->Data);
	Customized->frame_header.Seq = 1;
	memcpy(data_au8 , &Customized->frame_header , 5);
	Append_CRC8_Check_Sum(data_au8 , 5);
	Customized->frame_header.CRC8 = data_au8[4];

	Customized->cmd_id = (uint16_t)Customized_messages;
	memcpy(&data_au8[5] , &Customized->cmd_id , 2);

	Customized->Data.sender_id = Data->sender_id;
	Customized->Data.receiver_id = Data->receiver_id;
	memcpy(Customized->Data.user_data , Data->user_data , 30);
	memcpy(&data_au8[7] , &Customized->Data , sizeof(Customized->Data));

	Append_CRC16_Check_Sum(data_au8 , 7 + sizeof(Customized->Data));
	Customized->frame_tail = (((uint16_t)data_au8[7 + sizeof(Customized->Data)-1])<<8) | 
								(data_au8[7 + sizeof(Customized->Data)-2]);
}

/***********************************    战队主控->裁判系统   end    ***********************************/

//
// Created by RM UI Designer
//

#include "ui_default_init5_4.h"
#include "string.h"
#include "CAP.h"

#define FRAME_ID 0
#define GROUP_ID 5
#define START_ID 7

ui_string_frame_t ui_default_init5_4;

ui_interface_string_t* ui_default_init5_static55 = &ui_default_init5_4.option;

void _ui_init_default_init5_4() {
    ui_default_init5_4.option.figure_name[0] = FRAME_ID;
    ui_default_init5_4.option.figure_name[1] = GROUP_ID;
    ui_default_init5_4.option.figure_name[2] = START_ID;
    ui_default_init5_4.option.operate_tpyel = 1;
    ui_default_init5_4.option.figure_tpye = 7;
    ui_default_init5_4.option.layer = 5;
    ui_default_init5_4.option.font_size = 30;
    ui_default_init5_4.option.start_x = 180;
    ui_default_init5_4.option.start_y = 750;
    ui_default_init5_4.option.color = 2;
    ui_default_init5_4.option.str_length = 1;
    ui_default_init5_4.option.width = 2;
    strcpy(ui_default_init5_static55->string, "X");

    ui_proc_string_frame(&ui_default_init5_4);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_4, sizeof(ui_default_init5_4));
}

void _ui_update_default_init5_4() {
    ui_default_init5_4.option.operate_tpyel = 2;

    if (capData_JHB.Send_data_typedef.Send_data.switchControl == 0x01) {
        ui_default_init5_4.option.figure_tpye = 7;
        ui_default_init5_4.option.layer = 5;
        ui_default_init5_4.option.font_size = 30;
        ui_default_init5_4.option.start_x = 180;
        ui_default_init5_4.option.start_y = 750;
        ui_default_init5_4.option.color = 2;
        ui_default_init5_4.option.str_length = 2;
        ui_default_init5_4.option.width = 2;
        strcpy(ui_default_init5_static55->string, "X");
    } else
    {
        ui_default_init5_4.option.figure_tpye = 7;
        ui_default_init5_4.option.layer = 5;
        ui_default_init5_4.option.font_size = 30;
        ui_default_init5_4.option.start_x = 180;
        ui_default_init5_4.option.start_y = 750;
        ui_default_init5_4.option.color = 1;
        ui_default_init5_4.option.str_length = 1;
        ui_default_init5_4.option.width = 2;
        strcpy(ui_default_init5_static55->string, "X");
    }
    
    ui_proc_string_frame(&ui_default_init5_4);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_4, sizeof(ui_default_init5_4));
}

void _ui_remove_default_init5_4() {
    ui_default_init5_4.option.operate_tpyel = 3;

    ui_proc_string_frame(&ui_default_init5_4);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_4, sizeof(ui_default_init5_4));
}
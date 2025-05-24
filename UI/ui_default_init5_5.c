//
// Created by RM UI Designer
//

#include "ui_default_init5_5.h"
#include "string.h"
#include "TIM_DEV.h"
#include "DEFINE.h"

#define FRAME_ID 0
#define GROUP_ID 5
#define START_ID 5

ui_string_frame_t ui_default_init5_5;

ui_interface_string_t* ui_default_init5_static56 = &ui_default_init5_5.option;

void _ui_init_default_init5_5() {
    ui_default_init5_5.option.figure_name[0] = FRAME_ID;
    ui_default_init5_5.option.figure_name[1] = GROUP_ID;
    ui_default_init5_5.option.figure_name[2] = START_ID;
    ui_default_init5_5.option.operate_tpyel = 1;
    ui_default_init5_5.option.figure_tpye = 7;
    ui_default_init5_5.option.layer = 5;
    ui_default_init5_5.option.font_size = 50;
    ui_default_init5_5.option.start_x = 180;
    ui_default_init5_5.option.start_y = 660;
    ui_default_init5_5.option.color = 2;
    ui_default_init5_5.option.str_length = 1;
    ui_default_init5_5.option.width = 10;
    strcpy(ui_default_init5_static56->string, "V");

    ui_proc_string_frame(&ui_default_init5_5);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_5, sizeof(ui_default_init5_5));
}

void _ui_update_default_init5_5() {
    ui_default_init5_5.option.operate_tpyel = 2;

    if (MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.ENABLE == 1) {
        ui_default_init5_5.option.figure_tpye = 7;
        ui_default_init5_5.option.layer = 5;
        ui_default_init5_5.option.font_size = 50;
        ui_default_init5_5.option.start_x = 180;
        ui_default_init5_5.option.start_y = 660;
        ui_default_init5_5.option.color = 2;
        ui_default_init5_5.option.str_length = 1;
        ui_default_init5_5.option.width = 10;
        strcpy(ui_default_init5_static56->string, "V");
    } else
    {
        ui_default_init5_5.option.figure_tpye = 7;
        ui_default_init5_5.option.layer = 5;
        ui_default_init5_5.option.font_size = 50;
        ui_default_init5_5.option.start_x = 180;
        ui_default_init5_5.option.start_y = 660;
        ui_default_init5_5.option.color = 1;
        ui_default_init5_5.option.str_length = 1;
        ui_default_init5_5.option.width = 10;
        strcpy(ui_default_init5_static56->string, "V");
    }
    
    ui_proc_string_frame(&ui_default_init5_5);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_5, sizeof(ui_default_init5_5));
}

void _ui_remove_default_init5_5() {
    ui_default_init5_5.option.operate_tpyel = 3;

    ui_proc_string_frame(&ui_default_init5_5);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_5, sizeof(ui_default_init5_5));
}
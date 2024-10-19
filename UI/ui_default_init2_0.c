//
// Created by RM UI Designer
//

#include "ui_default_init2_0.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_init2_0;
ui_interface_line_t *ui_default_init2_static20 = (ui_interface_line_t *)&(ui_default_init2_0.data[0]);
ui_interface_line_t *ui_default_init2_static21 = (ui_interface_line_t *)&(ui_default_init2_0.data[1]);
ui_interface_number_t *ui_default_init2_static22 = (ui_interface_number_t *)&(ui_default_init2_0.data[2]);
ui_interface_number_t *ui_default_init2_static23 = (ui_interface_number_t *)&(ui_default_init2_0.data[3]);
ui_interface_number_t *ui_default_init2_static24 = (ui_interface_number_t *)&(ui_default_init2_0.data[4]);
ui_interface_number_t *ui_default_init2_static25 = (ui_interface_number_t *)&(ui_default_init2_0.data[5]);
ui_interface_number_t *ui_default_init2_static26 = (ui_interface_number_t *)&(ui_default_init2_0.data[6]);

void _ui_init_default_init2_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init2_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_init2_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_init2_0.data[i].figure_name[2] = i + START_ID;
        ui_default_init2_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_init2_0.data[i].operate_tpyel = 0;
    }

    ui_default_init2_static20->figure_tpye = 0;
    ui_default_init2_static20->layer = 4;
    ui_default_init2_static20->start_x = 1315;
    ui_default_init2_static20->start_y = 597;
    ui_default_init2_static20->end_x = 1339;
    ui_default_init2_static20->end_y = 602;
    ui_default_init2_static20->color = 8;
    ui_default_init2_static20->width = 10;

    ui_default_init2_static21->figure_tpye = 0;
    ui_default_init2_static21->layer = 4;
    ui_default_init2_static21->start_x = 1317;
    ui_default_init2_static21->start_y = 468;
    ui_default_init2_static21->end_x = 1341;
    ui_default_init2_static21->end_y = 463;
    ui_default_init2_static21->color = 8;
    ui_default_init2_static21->width = 10;

    ui_default_init2_static22->figure_tpye = 6;
    ui_default_init2_static22->layer = 4;
    ui_default_init2_static22->font_size = 13;
    ui_default_init2_static22->start_x = 1182;
    ui_default_init2_static22->start_y = 766;
    ui_default_init2_static22->color = 8;
    ui_default_init2_static22->number = 130;
    ui_default_init2_static22->width = 1;

    ui_default_init2_static23->figure_tpye = 6;
    ui_default_init2_static23->layer = 4;
    ui_default_init2_static23->font_size = 13;
    ui_default_init2_static23->start_x = 1245;
    ui_default_init2_static23->start_y = 659;
    ui_default_init2_static23->color = 8;
    ui_default_init2_static23->number = 110;
    ui_default_init2_static23->width = 1;

    ui_default_init2_static24->figure_tpye = 6;
    ui_default_init2_static24->layer = 4;
    ui_default_init2_static24->font_size = 13;
    ui_default_init2_static24->start_x = 1276;
    ui_default_init2_static24->start_y = 552;
    ui_default_init2_static24->color = 8;
    ui_default_init2_static24->number = 90;
    ui_default_init2_static24->width = 1;

    ui_default_init2_static25->figure_tpye = 6;
    ui_default_init2_static25->layer = 4;
    ui_default_init2_static25->font_size = 13;
    ui_default_init2_static25->start_x = 1264;
    ui_default_init2_static25->start_y = 434;
    ui_default_init2_static25->color = 8;
    ui_default_init2_static25->number = 70;
    ui_default_init2_static25->width = 1;

    ui_default_init2_static26->figure_tpye = 6;
    ui_default_init2_static26->layer = 4;
    ui_default_init2_static26->font_size = 13;
    ui_default_init2_static26->start_x = 1185;
    ui_default_init2_static26->start_y = 331;
    ui_default_init2_static26->color = 8;
    ui_default_init2_static26->number = 50;
    ui_default_init2_static26->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init2_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init2_0, sizeof(ui_default_init2_0));
}

void _ui_update_default_init2_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init2_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init2_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init2_0, sizeof(ui_default_init2_0));
}

void _ui_remove_default_init2_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init2_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init2_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init2_0, sizeof(ui_default_init2_0));
}

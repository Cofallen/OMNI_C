//
// Created by RM UI Designer
//

#include "ui_default_init3_0.h"

#define FRAME_ID 0
#define GROUP_ID 3
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_init3_0;
ui_interface_round_t *ui_default_init3_static30 = (ui_interface_round_t *)&(ui_default_init3_0.data[0]);
ui_interface_line_t *ui_default_init3_static31 = (ui_interface_line_t *)&(ui_default_init3_0.data[1]);
ui_interface_line_t *ui_default_init3_static32 = (ui_interface_line_t *)&(ui_default_init3_0.data[2]);
ui_interface_line_t *ui_default_init3_static33 = (ui_interface_line_t *)&(ui_default_init3_0.data[3]);
ui_interface_line_t *ui_default_init3_static34 = (ui_interface_line_t *)&(ui_default_init3_0.data[4]);
ui_interface_line_t *ui_default_init3_static35 = (ui_interface_line_t *)&(ui_default_init3_0.data[5]);
ui_interface_line_t *ui_default_init3_static36 = (ui_interface_line_t *)&(ui_default_init3_0.data[6]);

void _ui_init_default_init3_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init3_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_init3_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_init3_0.data[i].figure_name[2] = i + START_ID;
        ui_default_init3_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_init3_0.data[i].operate_tpyel = 0;
    }

    ui_default_init3_static30->figure_tpye = 2;
    ui_default_init3_static30->layer = 3;
    ui_default_init3_static30->r = 15;
    ui_default_init3_static30->start_x = 959;
    ui_default_init3_static30->start_y = 539;
    ui_default_init3_static30->color = 8;
    ui_default_init3_static30->width = 3;

    ui_default_init3_static31->figure_tpye = 0;
    ui_default_init3_static31->layer = 3;
    ui_default_init3_static31->start_x = 650;
    ui_default_init3_static31->start_y = 539;
    ui_default_init3_static31->end_x = 1270;
    ui_default_init3_static31->end_y = 539;
    ui_default_init3_static31->color = 8;
    ui_default_init3_static31->width = 3;

    ui_default_init3_static32->figure_tpye = 0;
    ui_default_init3_static32->layer = 3;
    ui_default_init3_static32->start_x = 959;
    ui_default_init3_static32->start_y = 320;
    ui_default_init3_static32->end_x = 959;
    ui_default_init3_static32->end_y = 620;
    ui_default_init3_static32->color = 8;
    ui_default_init3_static32->width = 1;

    ui_default_init3_static33->figure_tpye = 0;
    ui_default_init3_static33->layer = 3;
    ui_default_init3_static33->start_x = 920;
    ui_default_init3_static33->start_y = 513;
    ui_default_init3_static33->end_x = 1000;
    ui_default_init3_static33->end_y = 513;
    ui_default_init3_static33->color = 8;
    ui_default_init3_static33->width = 1;

    ui_default_init3_static34->figure_tpye = 0;
    ui_default_init3_static34->layer = 3;
    ui_default_init3_static34->start_x = 880;
    ui_default_init3_static34->start_y = 486;
    ui_default_init3_static34->end_x = 1040;
    ui_default_init3_static34->end_y = 486;
    ui_default_init3_static34->color = 8;
    ui_default_init3_static34->width = 1;

    ui_default_init3_static35->figure_tpye = 0;
    ui_default_init3_static35->layer = 3;
    ui_default_init3_static35->start_x = 840;
    ui_default_init3_static35->start_y = 459;
    ui_default_init3_static35->end_x = 1081;
    ui_default_init3_static35->end_y = 459;
    ui_default_init3_static35->color = 8;
    ui_default_init3_static35->width = 1;

    ui_default_init3_static36->figure_tpye = 0;
    ui_default_init3_static36->layer = 3;
    ui_default_init3_static36->start_x = 800;
    ui_default_init3_static36->start_y = 432;
    ui_default_init3_static36->end_x = 1121;
    ui_default_init3_static36->end_y = 432;
    ui_default_init3_static36->color = 8;
    ui_default_init3_static36->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init3_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init3_0, sizeof(ui_default_init3_0));
}

void _ui_update_default_init3_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init3_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init3_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init3_0, sizeof(ui_default_init3_0));
}

void _ui_remove_default_init3_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init3_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init3_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init3_0, sizeof(ui_default_init3_0));
}

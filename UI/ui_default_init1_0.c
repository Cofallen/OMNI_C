//
// Created by RM UI Designer
//

#include "ui_default_init1_0.h"

#define FRAME_ID 0
#define GROUP_ID 1
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_init1_0;
ui_interface_line_t *ui_default_init1_static10 = (ui_interface_line_t *)&(ui_default_init1_0.data[0]);
ui_interface_line_t *ui_default_init1_static11 = (ui_interface_line_t *)&(ui_default_init1_0.data[1]);
ui_interface_line_t *ui_default_init1_static12 = (ui_interface_line_t *)&(ui_default_init1_0.data[2]);
ui_interface_line_t *ui_default_init1_static13 = (ui_interface_line_t *)&(ui_default_init1_0.data[3]);
ui_interface_line_t *ui_default_init1_static14 = (ui_interface_line_t *)&(ui_default_init1_0.data[4]);
ui_interface_line_t *ui_default_init1_static15 = (ui_interface_line_t *)&(ui_default_init1_0.data[5]);
ui_interface_line_t *ui_default_init1_static16 = (ui_interface_line_t *)&(ui_default_init1_0.data[6]);

void _ui_init_default_init1_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init1_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_init1_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_init1_0.data[i].figure_name[2] = i + START_ID;
        ui_default_init1_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_init1_0.data[i].operate_tpyel = 0;
    }

    ui_default_init1_static10->figure_tpye = 0;
    ui_default_init1_static10->layer = 7;
    ui_default_init1_static10->start_x = 1223;
    ui_default_init1_static10->start_y = 760;
    ui_default_init1_static10->end_x = 1249;
    ui_default_init1_static10->end_y = 782;
    ui_default_init1_static10->color = 8;
    ui_default_init1_static10->width = 10;

    ui_default_init1_static11->figure_tpye = 0;
    ui_default_init1_static11->layer = 7;
    ui_default_init1_static11->start_x = 1288;
    ui_default_init1_static11->start_y = 652;
    ui_default_init1_static11->end_x = 1317;
    ui_default_init1_static11->end_y = 666;
    ui_default_init1_static11->color = 8;
    ui_default_init1_static11->width = 10;

    ui_default_init1_static12->figure_tpye = 0;
    ui_default_init1_static12->layer = 7;
    ui_default_init1_static12->start_x = 1310;
    ui_default_init1_static12->start_y = 536;
    ui_default_init1_static12->end_x = 1348;
    ui_default_init1_static12->end_y = 536;
    ui_default_init1_static12->color = 8;
    ui_default_init1_static12->width = 10;

    ui_default_init1_static13->figure_tpye = 0;
    ui_default_init1_static13->layer = 7;
    ui_default_init1_static13->start_x = 1290;
    ui_default_init1_static13->start_y = 412;
    ui_default_init1_static13->end_x = 1319;
    ui_default_init1_static13->end_y = 398;
    ui_default_init1_static13->color = 8;
    ui_default_init1_static13->width = 10;

    ui_default_init1_static14->figure_tpye = 0;
    ui_default_init1_static14->layer = 7;
    ui_default_init1_static14->start_x = 1241;
    ui_default_init1_static14->start_y = 280;
    ui_default_init1_static14->end_x = 1215;
    ui_default_init1_static14->end_y = 304;
    ui_default_init1_static14->color = 8;
    ui_default_init1_static14->width = 10;

    ui_default_init1_static15->figure_tpye = 0;
    ui_default_init1_static15->layer = 7;
    ui_default_init1_static15->start_x = 1272;
    ui_default_init1_static15->start_y = 717;
    ui_default_init1_static15->end_x = 1292;
    ui_default_init1_static15->end_y = 729;
    ui_default_init1_static15->color = 8;
    ui_default_init1_static15->width = 10;

    ui_default_init1_static16->figure_tpye = 0;
    ui_default_init1_static16->layer = 7;
    ui_default_init1_static16->start_x = 1289;
    ui_default_init1_static16->start_y = 332;
    ui_default_init1_static16->end_x = 1269;
    ui_default_init1_static16->end_y = 344;
    ui_default_init1_static16->color = 8;
    ui_default_init1_static16->width = 10;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init1_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init1_0, sizeof(ui_default_init1_0));
}

void _ui_update_default_init1_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init1_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init1_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init1_0, sizeof(ui_default_init1_0));
}

void _ui_remove_default_init1_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init1_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init1_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init1_0, sizeof(ui_default_init1_0));
}

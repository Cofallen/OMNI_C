//
// Created by RM UI Designer
//

#include "ui_default_init5_3.h"

#define FRAME_ID 0
#define GROUP_ID 5
#define START_ID 3
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_init5_3;
ui_interface_line_t *ui_default_init5_static50 = (ui_interface_line_t *)&(ui_default_init5_3.data[0]);
ui_interface_line_t *ui_default_init5_static51 = (ui_interface_line_t *)&(ui_default_init5_3.data[1]);

void _ui_init_default_init5_3() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init5_3.data[i].figure_name[0] = FRAME_ID;
        ui_default_init5_3.data[i].figure_name[1] = GROUP_ID;
        ui_default_init5_3.data[i].figure_name[2] = i + START_ID;
        ui_default_init5_3.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_init5_3.data[i].operate_tpyel = 0;
    }

    ui_default_init5_static50->figure_tpye = 0;
    ui_default_init5_static50->layer = 5;
    ui_default_init5_static50->start_x = 492;
    ui_default_init5_static50->start_y = 164;
    ui_default_init5_static50->end_x = 613;
    ui_default_init5_static50->end_y = 326;
    ui_default_init5_static50->color = 2;
    ui_default_init5_static50->width = 2;

    ui_default_init5_static51->figure_tpye = 0;
    ui_default_init5_static51->layer = 5;
    ui_default_init5_static51->start_x = 1428;
    ui_default_init5_static51->start_y = 164;
    ui_default_init5_static51->end_x = 1307;
    ui_default_init5_static51->end_y = 326;
    ui_default_init5_static51->color = 2;
    ui_default_init5_static51->width = 2;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init5_3);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_3, sizeof(ui_default_init5_3));
}

void _ui_update_default_init5_3() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init5_3.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init5_3);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_3, sizeof(ui_default_init5_3));
}

void _ui_remove_default_init5_3() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init5_3.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init5_3);
    SEND_MESSAGE((uint8_t *) &ui_default_init5_3, sizeof(ui_default_init5_3));
}

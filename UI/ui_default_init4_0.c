//
// Created by RM UI Designer
//

#include "ui_default_init4_0.h"

#define FRAME_ID 0
#define GROUP_ID 4
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_init4_0;
ui_interface_round_t *ui_default_init4_static40 = (ui_interface_round_t *)&(ui_default_init4_0.data[0]);
ui_interface_rect_t *ui_default_init4_static41 = (ui_interface_rect_t *)&(ui_default_init4_0.data[1]);
ui_interface_rect_t *ui_default_init4_static42 = (ui_interface_rect_t *)&(ui_default_init4_0.data[2]);
ui_interface_number_t *ui_default_init4_static43 = (ui_interface_number_t *)&(ui_default_init4_0.data[3]);
ui_interface_number_t *ui_default_init4_static44 = (ui_interface_number_t *)&(ui_default_init4_0.data[4]);
ui_interface_number_t *ui_default_init4_static45 = (ui_interface_number_t *)&(ui_default_init4_0.data[5]);
ui_interface_round_t *ui_default_init4_static46 = (ui_interface_round_t *)&(ui_default_init4_0.data[6]);

void _ui_init_default_init4_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init4_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_init4_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_init4_0.data[i].figure_name[2] = i + START_ID;
        ui_default_init4_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_init4_0.data[i].operate_tpyel = 0;
    }

    ui_default_init4_static40->figure_tpye = 2;
    ui_default_init4_static40->layer = 6;
    ui_default_init4_static40->r = 80;
    ui_default_init4_static40->start_x = 1638;
    ui_default_init4_static40->start_y = 722;
    ui_default_init4_static40->color = 8;
    ui_default_init4_static40->width = 4;

    ui_default_init4_static41->figure_tpye = 1;
    ui_default_init4_static41->layer = 6;
    ui_default_init4_static41->start_x = 1587;
    ui_default_init4_static41->start_y = 673;
    ui_default_init4_static41->color = 8;
    ui_default_init4_static41->width = 4;
    ui_default_init4_static41->end_x = 1617;
    ui_default_init4_static41->end_y = 773;

    ui_default_init4_static42->figure_tpye = 1;
    ui_default_init4_static42->layer = 6;
    ui_default_init4_static42->start_x = 1661;
    ui_default_init4_static42->start_y = 673;
    ui_default_init4_static42->color = 8;
    ui_default_init4_static42->width = 4;
    ui_default_init4_static42->end_x = 1691;
    ui_default_init4_static42->end_y = 773;

    ui_default_init4_static43->figure_tpye = 6;
    ui_default_init4_static43->layer = 6;
    ui_default_init4_static43->font_size = 23;
    ui_default_init4_static43->start_x = 1628;
    ui_default_init4_static43->start_y = 780;
    ui_default_init4_static43->color = 8;
    ui_default_init4_static43->number = 7;
    ui_default_init4_static43->width = 2;

    ui_default_init4_static44->figure_tpye = 6;
    ui_default_init4_static44->layer = 6;
    ui_default_init4_static44->font_size = 23;
    ui_default_init4_static44->start_x = 1630;
    ui_default_init4_static44->start_y = 742;
    ui_default_init4_static44->color = 8;
    ui_default_init4_static44->number = 6;
    ui_default_init4_static44->width = 2;

    ui_default_init4_static45->figure_tpye = 6;
    ui_default_init4_static45->layer = 6;
    ui_default_init4_static45->font_size = 23;
    ui_default_init4_static45->start_x = 1630;
    ui_default_init4_static45->start_y = 705;
    ui_default_init4_static45->color = 8;
    ui_default_init4_static45->number = 5;
    ui_default_init4_static45->width = 2;

    ui_default_init4_static46->figure_tpye = 2;
    ui_default_init4_static46->layer = 6;
    ui_default_init4_static46->r = 5;
    ui_default_init4_static46->start_x = 6;
    ui_default_init4_static46->start_y = 6;
    ui_default_init4_static46->color = 8;
    ui_default_init4_static46->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init4_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init4_0, sizeof(ui_default_init4_0));
}

void _ui_update_default_init4_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init4_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init4_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init4_0, sizeof(ui_default_init4_0));
}

void _ui_remove_default_init4_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init4_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init4_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init4_0, sizeof(ui_default_init4_0));
}

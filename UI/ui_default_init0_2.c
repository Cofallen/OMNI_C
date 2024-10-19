//
// Created by RM UI Designer
//

#include "ui_default_init0_2.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 2
#define OBJ_NUM 5
#define FRAME_OBJ_NUM 5

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_init0_2;
ui_interface_line_t *ui_default_init0_static00 = (ui_interface_line_t *)&(ui_default_init0_2.data[0]);
ui_interface_line_t *ui_default_init0_static01 = (ui_interface_line_t *)&(ui_default_init0_2.data[1]);
ui_interface_line_t *ui_default_init0_static02 = (ui_interface_line_t *)&(ui_default_init0_2.data[2]);
ui_interface_number_t *ui_default_init0_static03 = (ui_interface_number_t *)&(ui_default_init0_2.data[3]);
ui_interface_number_t *ui_default_init0_static04 = (ui_interface_number_t *)&(ui_default_init0_2.data[4]);

void _ui_init_default_init0_2() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init0_2.data[i].figure_name[0] = FRAME_ID;
        ui_default_init0_2.data[i].figure_name[1] = GROUP_ID;
        ui_default_init0_2.data[i].figure_name[2] = i + START_ID;
        ui_default_init0_2.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_init0_2.data[i].operate_tpyel = 0;
    }

    ui_default_init0_static00->figure_tpye = 0;
    ui_default_init0_static00->layer = 8;
    ui_default_init0_static00->start_x = 669;
    ui_default_init0_static00->start_y = 798;
    ui_default_init0_static00->end_x = 689;
    ui_default_init0_static00->end_y = 781;
    ui_default_init0_static00->color = 8;
    ui_default_init0_static00->width = 10;

    ui_default_init0_static01->figure_tpye = 0;
    ui_default_init0_static01->layer = 8;
    ui_default_init0_static01->start_x = 575;
    ui_default_init0_static01->start_y = 540;
    ui_default_init0_static01->end_x = 601;
    ui_default_init0_static01->end_y = 540;
    ui_default_init0_static01->color = 8;
    ui_default_init0_static01->width = 10;

    ui_default_init0_static02->figure_tpye = 0;
    ui_default_init0_static02->layer = 8;
    ui_default_init0_static02->start_x = 660;
    ui_default_init0_static02->start_y = 285;
    ui_default_init0_static02->end_x = 675;
    ui_default_init0_static02->end_y = 302;
    ui_default_init0_static02->color = 8;
    ui_default_init0_static02->width = 10;

    ui_default_init0_static03->figure_tpye = 6;
    ui_default_init0_static03->layer = 8;
    ui_default_init0_static03->font_size = 13;
    ui_default_init0_static03->start_x = 700;
    ui_default_init0_static03->start_y = 303;
    ui_default_init0_static03->color = 8;
    ui_default_init0_static03->number = 500;
    ui_default_init0_static03->width = 1;

    ui_default_init0_static04->figure_tpye = 6;
    ui_default_init0_static04->layer = 8;
    ui_default_init0_static04->font_size = 13;
    ui_default_init0_static04->start_x = 639;
    ui_default_init0_static04->start_y = 434;
    ui_default_init0_static04->color = 8;
    ui_default_init0_static04->number = 250;
    ui_default_init0_static04->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init0_2);
    SEND_MESSAGE((uint8_t *) &ui_default_init0_2, sizeof(ui_default_init0_2));
}

void _ui_update_default_init0_2() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init0_2.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init0_2);
    SEND_MESSAGE((uint8_t *) &ui_default_init0_2, sizeof(ui_default_init0_2));
}

void _ui_remove_default_init0_2() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init0_2.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init0_2);
    SEND_MESSAGE((uint8_t *) &ui_default_init0_2, sizeof(ui_default_init0_2));
}

//
// Created by RM UI Designer
//

#include "ui_default_init6_0.h"

#define FRAME_ID 0
#define GROUP_ID 6
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_init6_0;
ui_interface_line_t *ui_default_init6_Dynamic0 = (ui_interface_line_t *)&(ui_default_init6_0.data[0]);
ui_interface_line_t *ui_default_init6_Dynamic1 = (ui_interface_line_t *)&(ui_default_init6_0.data[1]);
ui_interface_arc_t *ui_default_init6_Dynamic2 = (ui_interface_arc_t *)&(ui_default_init6_0.data[2]);
ui_interface_arc_t *ui_default_init6_Dynamic3 = (ui_interface_arc_t *)&(ui_default_init6_0.data[3]);
ui_interface_arc_t *ui_default_init6_Dynamic4 = (ui_interface_arc_t *)&(ui_default_init6_0.data[4]);
ui_interface_arc_t *ui_default_init6_Dynamic5 = (ui_interface_arc_t *)&(ui_default_init6_0.data[5]);
ui_interface_arc_t *ui_default_init6_Dynamic6 = (ui_interface_arc_t *)&(ui_default_init6_0.data[6]);

void _ui_init_default_init6_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init6_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_init6_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_init6_0.data[i].figure_name[2] = i + START_ID;
        ui_default_init6_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_init6_0.data[i].operate_tpyel = 0;
    }
    //直线(摩擦轮速度)
    ui_default_init6_Dynamic0->figure_tpye = 0;
    ui_default_init6_Dynamic0->layer = 2;
    ui_default_init6_Dynamic0->start_x = 1591;
    ui_default_init6_Dynamic0->start_y = 673;
    ui_default_init6_Dynamic0->end_x = 1591;
    ui_default_init6_Dynamic0->end_y = 774;
    ui_default_init6_Dynamic0->color = 6;
    ui_default_init6_Dynamic0->width = 26;
    //直线(摩擦轮速度)
    ui_default_init6_Dynamic1->figure_tpye = 0;
    ui_default_init6_Dynamic1->layer = 2;
    ui_default_init6_Dynamic1->start_x = 1665;
    ui_default_init6_Dynamic1->start_y = 673;
    ui_default_init6_Dynamic1->end_x = 1665;
    ui_default_init6_Dynamic1->end_y = 774;
    ui_default_init6_Dynamic1->color = 6;
    ui_default_init6_Dynamic1->width = 30;
    //圆弧(电容电压)
    ui_default_init6_Dynamic2->figure_tpye = 4;
    ui_default_init6_Dynamic2->layer = 2;
    ui_default_init6_Dynamic2->rx = 92;
    ui_default_init6_Dynamic2->ry = 92;
    ui_default_init6_Dynamic2->start_x = 1629;
    ui_default_init6_Dynamic2->start_y = 713;
    ui_default_init6_Dynamic2->color = 6;
    ui_default_init6_Dynamic2->width = 20;
    ui_default_init6_Dynamic2->start_angle = 200;
    ui_default_init6_Dynamic2->end_angle = 160;
    //Pitch指针(设计器里设计成了直线,现在的圆弧是后面自己改的)
    ui_default_init6_Dynamic3->figure_tpye = 4;
    ui_default_init6_Dynamic3->layer = 2;
    ui_default_init6_Dynamic3->rx = 370;
    ui_default_init6_Dynamic3->ry = 370;
    ui_default_init6_Dynamic3->start_x = 948;
    ui_default_init6_Dynamic3->start_y = 524;
    ui_default_init6_Dynamic3->color = 4;
    ui_default_init6_Dynamic3->width = 30;
    ui_default_init6_Dynamic3->start_angle = 89;
    ui_default_init6_Dynamic3->end_angle = 91;
    //椭圆(自瞄指示器)
    ui_default_init6_Dynamic4->figure_tpye = 4;
    ui_default_init6_Dynamic4->layer = 2;
    ui_default_init6_Dynamic4->rx = 300;
    ui_default_init6_Dynamic4->ry = 200;
    ui_default_init6_Dynamic4->start_x = 948;
    ui_default_init6_Dynamic4->start_y = 481;
    ui_default_init6_Dynamic4->color = 6;
    ui_default_init6_Dynamic4->width = 20;
    ui_default_init6_Dynamic4->start_angle = 345;
    ui_default_init6_Dynamic4->end_angle = 15;
    //圆弧(实体弹量)
    ui_default_init6_Dynamic5->figure_tpye = 4;
    ui_default_init6_Dynamic5->layer = 2;
    ui_default_init6_Dynamic5->rx = 385;
    ui_default_init6_Dynamic5->ry = 385;
    ui_default_init6_Dynamic5->start_x = 958;
    ui_default_init6_Dynamic5->start_y = 530;
    ui_default_init6_Dynamic5->color = 6;
    ui_default_init6_Dynamic5->width = 20;
    ui_default_init6_Dynamic5->start_angle = 228;
    ui_default_init6_Dynamic5->end_angle = 270;
    //圆弧(云台底盘夹角)
    ui_default_init6_Dynamic6->figure_tpye = 4;
    ui_default_init6_Dynamic6->layer = 2;
    ui_default_init6_Dynamic6->rx = 385;
    ui_default_init6_Dynamic6->ry = 385;
    ui_default_init6_Dynamic6->start_x = 958;
    ui_default_init6_Dynamic6->start_y = 530;
    ui_default_init6_Dynamic6->color = 6;
    ui_default_init6_Dynamic6->width = 20;
    ui_default_init6_Dynamic6->start_angle = 270;
    ui_default_init6_Dynamic6->end_angle = 310;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init6_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init6_0, sizeof(ui_default_init6_0));
}

void _ui_update_default_init6_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init6_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init6_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init6_0, sizeof(ui_default_init6_0));
}

void _ui_remove_default_init6_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init6_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init6_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init6_0, sizeof(ui_default_init6_0));
}

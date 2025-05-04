//
// Created by RM UI Designer
//

#include "ui_default_init6_0.h"
#include "read_data.h"
#include "TIM_DEV.h"    
#include "TOP.h"
#include "define.h"
#include "MOTOR.h"
#include "YU_MATH.h"
#include "CAP.h"
#include "VISION.h"

#define FRAME_ID 0
#define GROUP_ID 6
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

float angle_relative = 0.0f;

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
    ui_default_init6_Dynamic0->start_x = 1601;
    ui_default_init6_Dynamic0->start_y = 673;
    ui_default_init6_Dynamic0->end_x = 1601;
    ui_default_init6_Dynamic0->end_y = 774;
    ui_default_init6_Dynamic0->color = 6;
    ui_default_init6_Dynamic0->width = 26;
    //直线(摩擦轮速度)
    ui_default_init6_Dynamic1->figure_tpye = 0;
    ui_default_init6_Dynamic1->layer = 2;
    ui_default_init6_Dynamic1->start_x = 1675;
    ui_default_init6_Dynamic1->start_y = 673;
    ui_default_init6_Dynamic1->end_x = 1675;
    ui_default_init6_Dynamic1->end_y = 774;
    ui_default_init6_Dynamic1->color = 6;
    ui_default_init6_Dynamic1->width = 30;
    //圆弧(头朝向)
    ui_default_init6_Dynamic2->figure_tpye = 4;
    ui_default_init6_Dynamic2->layer = 2;
    ui_default_init6_Dynamic2->rx = 92;
    ui_default_init6_Dynamic2->ry = 92;
    ui_default_init6_Dynamic2->start_x = 1639;
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
    // //椭圆(自瞄指示器)
    // ui_default_init6_Dynamic4->figure_tpye = 4;
    // ui_default_init6_Dynamic4->layer = 2;
    // ui_default_init6_Dynamic4->rx = 300;
    // ui_default_init6_Dynamic4->ry = 200;
    // ui_default_init6_Dynamic4->start_x = 948;
    // ui_default_init6_Dynamic4->start_y = 481;
    // ui_default_init6_Dynamic4->color = 6;
    // ui_default_init6_Dynamic4->width = 20;
    // ui_default_init6_Dynamic4->start_angle = 345;
    // ui_default_init6_Dynamic4->end_angle = 15;
    // //圆弧(cap)
    ui_default_init6_Dynamic5->figure_tpye = 4;
    ui_default_init6_Dynamic5->layer = 2;
    ui_default_init6_Dynamic5->rx = 385;
    ui_default_init6_Dynamic5->ry = 385;
    ui_default_init6_Dynamic5->start_x = 958;
    ui_default_init6_Dynamic5->start_y = 543;
    ui_default_init6_Dynamic5->color = 6;
    ui_default_init6_Dynamic5->width = 20;
    ui_default_init6_Dynamic5->start_angle = 228;
    ui_default_init6_Dynamic5->end_angle = 270;
    //自瞄  黄1 蓝0
    ui_default_init6_Dynamic6->figure_tpye = 4;
    ui_default_init6_Dynamic6->layer = 2;
    ui_default_init6_Dynamic6->rx = 385;
    ui_default_init6_Dynamic6->ry = 385;
    ui_default_init6_Dynamic6->start_x = 958;
    ui_default_init6_Dynamic6->start_y = 543;
    ui_default_init6_Dynamic6->color = 6;
    ui_default_init6_Dynamic6->width = 20;
    ui_default_init6_Dynamic6->start_angle = 290;
    ui_default_init6_Dynamic6->end_angle = 310;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_init6_0);
    SEND_MESSAGE((uint8_t *) &ui_default_init6_0, sizeof(ui_default_init6_0));
}

void _ui_update_default_init6_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_init6_0.data[i].operate_tpyel = 2;
    }
    angle_relative = ((float) TOP.yaw[1] - (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INIT) / 22.755f;
    ui_default_init6_Dynamic2->figure_tpye = 4;
    ui_default_init6_Dynamic2->layer = 2;
    ui_default_init6_Dynamic2->rx = 92;
    ui_default_init6_Dynamic2->ry = 92;
    ui_default_init6_Dynamic2->start_x = 1639;
    ui_default_init6_Dynamic2->start_y = 723;
    ui_default_init6_Dynamic2->color = 6;
    ui_default_init6_Dynamic2->width = 20;
    ui_default_init6_Dynamic2->start_angle = 200 - (uint16_t)angle_relative;
    ui_default_init6_Dynamic2->end_angle = 160 - (uint16_t)angle_relative;

    //直线(摩擦轮速度)
    ui_default_init6_Dynamic0->figure_tpye = 0;
    ui_default_init6_Dynamic0->layer = 2;
    ui_default_init6_Dynamic0->start_x = 1601;
    ui_default_init6_Dynamic0->start_y = 673;
    ui_default_init6_Dynamic0->end_x = 1601;
    ui_default_init6_Dynamic0->end_y = 673 + (uint16_t)(0.05f * (float)(-MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW - 5000));
    ui_default_init6_Dynamic0->color = 6;
    ui_default_init6_Dynamic0->width = 26;
    //直线(摩擦轮速度)
    ui_default_init6_Dynamic1->figure_tpye = 0;
    ui_default_init6_Dynamic1->layer = 2;
    ui_default_init6_Dynamic1->start_x = 1675;
    ui_default_init6_Dynamic1->start_y = 673;
    ui_default_init6_Dynamic1->end_x = 1675;
    ui_default_init6_Dynamic1->end_y = 673 + (uint16_t)(0.05f * (float)(MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.SPEED_NOW - 5000));
    ui_default_init6_Dynamic1->color = 6;
    ui_default_init6_Dynamic1->width = 30;

    //高于或者低于量程的速度显示为黄色
    if((MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW > -5000 || MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW<-7800))
    {
    ui_default_init6_Dynamic0->figure_tpye = 0;
    ui_default_init6_Dynamic0->layer = 2;
    ui_default_init6_Dynamic0->start_x = 1601;
    ui_default_init6_Dynamic0->start_y = 673;
    ui_default_init6_Dynamic0->end_x = 1601;
    ui_default_init6_Dynamic0->end_y = 774;
    ui_default_init6_Dynamic0->color = 3;
    ui_default_init6_Dynamic0->width = 30;
    }
     if(MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.SPEED_NOW < 5000 || MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.SPEED_NOW>7800)
    {
    ui_default_init6_Dynamic1->figure_tpye = 0;
    ui_default_init6_Dynamic1->layer = 2;
    ui_default_init6_Dynamic1->start_x = 1675;
    ui_default_init6_Dynamic1->start_y = 673;
    ui_default_init6_Dynamic1->end_x = 1675;
    ui_default_init6_Dynamic1->end_y = 774;
    ui_default_init6_Dynamic1->color = 3;
    ui_default_init6_Dynamic1->width = 30;
    }

    // pitch
    ui_default_init6_Dynamic3->figure_tpye = 4;
    ui_default_init6_Dynamic3->layer = 2;
    ui_default_init6_Dynamic3->rx = 370;
    ui_default_init6_Dynamic3->ry = 370;
    ui_default_init6_Dynamic3->start_x = 958;
    ui_default_init6_Dynamic3->start_y = 543;
    ui_default_init6_Dynamic3->color = 4;
    ui_default_init6_Dynamic3->width = 30;
    ui_default_init6_Dynamic3->start_angle = 89 - (uint16_t)TOP.roll[5];
    ui_default_init6_Dynamic3->end_angle = 91 - (uint16_t)TOP.roll[5];

    //圆弧(cap)
    ui_default_init6_Dynamic5->figure_tpye = 4;
    ui_default_init6_Dynamic5->layer = 2;
    ui_default_init6_Dynamic5->rx = 385;
    ui_default_init6_Dynamic5->ry = 385;
    ui_default_init6_Dynamic5->start_x = 958;
    ui_default_init6_Dynamic5->start_y = 543;
    ui_default_init6_Dynamic5->color = 6;
    ui_default_init6_Dynamic5->width = 20;
    ui_default_init6_Dynamic5->start_angle = 228;
    ui_default_init6_Dynamic5->end_angle = 270 - (uint16_t)((23.0f - capData_JHB.Receive_data_typedef.capVolt) * 3.818f);

    if (VISION_V_DATA.RECEIVE.TARGET && !VISION_V_DATA.SEND.is_buff)  // 自瞄开 紫红色4
    {
        ui_default_init6_Dynamic6->figure_tpye = 4;
        ui_default_init6_Dynamic6->layer = 2;
        ui_default_init6_Dynamic6->rx = 385;
        ui_default_init6_Dynamic6->ry = 385;
        ui_default_init6_Dynamic6->start_x = 958;
        ui_default_init6_Dynamic6->start_y = 543;
        ui_default_init6_Dynamic6->color = 4;
        ui_default_init6_Dynamic6->width = 20;
        ui_default_init6_Dynamic6->start_angle = 290;
        ui_default_init6_Dynamic6->end_angle = 310;
    }else if (VISION_V_DATA.RECEIVE.TARGET && VISION_V_DATA.SEND.is_buff){ // 自瞄关，buff开 黄色1
        ui_default_init6_Dynamic6->figure_tpye = 4;
        ui_default_init6_Dynamic6->layer = 2;
        ui_default_init6_Dynamic6->rx = 385;
        ui_default_init6_Dynamic6->ry = 385;
        ui_default_init6_Dynamic6->start_x = 958;
        ui_default_init6_Dynamic6->start_y = 543;
        ui_default_init6_Dynamic6->color = 1;
        ui_default_init6_Dynamic6->width = 20;
        ui_default_init6_Dynamic6->start_angle = 290;
        ui_default_init6_Dynamic6->end_angle = 310;
    } else if (!VISION_V_DATA.RECEIVE.TARGET){                                            // 自瞄关 白色8
        ui_default_init6_Dynamic6->figure_tpye = 4;
        ui_default_init6_Dynamic6->layer = 2;
        ui_default_init6_Dynamic6->rx = 385;
        ui_default_init6_Dynamic6->ry = 385;
        ui_default_init6_Dynamic6->start_x = 958;
        ui_default_init6_Dynamic6->start_y = 543;
        ui_default_init6_Dynamic6->color = 8;
        ui_default_init6_Dynamic6->width = 20;
        ui_default_init6_Dynamic6->start_angle = 290;
        ui_default_init6_Dynamic6->end_angle = 310;
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

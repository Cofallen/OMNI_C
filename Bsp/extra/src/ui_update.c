#include "ui_update.h"

#include "ui_default_init0_0.h"
#include "ui_default_init0_1.h"
#include "ui_default_init0_2.h"
#include "ui_default_init1_0.h"
#include "ui_default_init2_0.h"
#include "ui_default_init3_0.h"
#include "ui_default_init4_0.h"
#include "ui_default_init5_0.h"
#include "ui_default_init5_1.h"
#include "ui_default_init5_2.h"
#include "ui_default_init5_3.h"
#include "ui_default_init5_4.h"
#include "ui_default_init5_5.h"
#include "ui_default_init6_0.h"

#include "DBUS.h"
#include "CHASSIS.h"
#include "DEFINE.h"
#include "ui_interface.h"
#include "Read_Data.h"

uint8_t RobotUI_Static_Init(uint8_t tim)
{
    ui_self_id = user_data.robot_status.robot_id;
    _ui_init_default_init0_0(); vTaskDelay(tim);
    _ui_init_default_init0_2(); vTaskDelay(tim);
    _ui_init_default_init1_0(); vTaskDelay(tim);
    _ui_init_default_init2_0(); vTaskDelay(tim);
    _ui_init_default_init3_0(); vTaskDelay(tim);
    _ui_init_default_init4_0(); vTaskDelay(tim);
    _ui_init_default_init5_0(); vTaskDelay(tim);
    _ui_init_default_init5_1(); vTaskDelay(tim);
    _ui_init_default_init5_2(); vTaskDelay(tim);
    _ui_init_default_init5_3(); vTaskDelay(tim);
    _ui_init_default_init5_4(); vTaskDelay(tim);
    _ui_init_default_init5_5(); vTaskDelay(tim);
    _ui_init_default_init6_0(); vTaskDelay(tim);
    return 1;
}

void RobotUI_Dynamic(uint8_t tim)
{
    static uint8_t UI_Init = 1;
    static uint8_t ui_status = 0;

    if (DBUS_V_DATA.KEY_BOARD.Z) ui_status = 1;
    else ui_status = 0;

    DBUS_V_DATA.KEY_BOARD.Z_PREE_NUMBER = DBUS_V_DATA.KEY_BOARD.Z;

    //遥控离线监测
    if(DBUS_V_DATA.IS_OFF || ui_status == 1)
    {
        UI_Init = 0;
    }

    // 静态UI刷新
    if (UI_Init != 1)
    {
        UI_Init = RobotUI_Static_Init(40);
    }

    //动态UI数据修改
    // ui_default_init6_Dynamic0->_a++;
    // ui_default_init6_Dynamic1->_a++;
    // ui_default_init6_Dynamic2->_c++;
    // ui_default_init6_Dynamic3->_c++;
    // ui_default_init6_Dynamic4->_c++;
    // ui_default_init6_Dynamic5->_c++;
    // ui_default_init6_Dynamic6->_c++;

    //动态UI刷新
    // if (cap_mode_ctrl[NOW] != cap_mode_ctrl[LAST])
    // {
        _ui_update_default_init5_0(); vTaskDelay(tim);  // c
    // }
    // if (chassis_control[0][NOW] != chassis_control[0][LAST])
    // {
        _ui_update_default_init5_1(); vTaskDelay(tim);  // f
    // }
    // if (chassis_control[1][NOW] != chassis_control[1][LAST])
    // {
        _ui_update_default_init5_2(); vTaskDelay(tim);  // g
    // }
    
    // _ui_update_default_init5_3(); vTaskDelay(tim);
    _ui_update_default_init5_4(); vTaskDelay(tim);       // x
    _ui_update_default_init5_5(); vTaskDelay(tim);       // v
    _ui_update_default_init6_0(); vTaskDelay(tim);
    
    cap_mode_ctrl[LAST] = cap_mode_ctrl[NOW];
    chassis_control[0][LAST] = chassis_control[0][NOW];
    chassis_control[1][LAST] = chassis_control[1][NOW];

    vTaskDelay(40);
}

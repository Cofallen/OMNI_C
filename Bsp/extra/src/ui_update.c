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
#include "ui_default_init6_0.h"

#include "DBUS.h"

uint8_t RobotUI_Static_Init()
{
    _ui_init_default_init0_0(); osDelay(34);
    _ui_init_default_init0_2(); osDelay(34);
    _ui_init_default_init1_0(); osDelay(34);
    _ui_init_default_init2_0(); osDelay(34);
    _ui_init_default_init3_0(); osDelay(34);
    _ui_init_default_init4_0(); osDelay(34);
    _ui_init_default_init5_0(); osDelay(34);
    _ui_init_default_init5_1(); osDelay(34);
    _ui_init_default_init5_2(); osDelay(34);
    _ui_init_default_init5_3(); osDelay(34);
    _ui_init_default_init6_0(); osDelay(34);
    return 1;
}

void RobotUI_Dynamic()
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
        UI_Init = RobotUI_Static_Init();
    }

    //动态UI数据修改
    ui_default_init6_Dynamic0->_a++;
    ui_default_init6_Dynamic1->_a++;
    ui_default_init6_Dynamic2->_c++;
    ui_default_init6_Dynamic3->_c++;
    ui_default_init6_Dynamic4->_c++;
    ui_default_init6_Dynamic5->_c++;
    ui_default_init6_Dynamic6->_c++;

    //动态UI刷新
    _ui_update_default_init5_3(); osDelay(34);
    _ui_update_default_init6_0(); osDelay(34);
    osDelay(34);
}

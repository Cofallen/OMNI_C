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
    ui_init_default_init0();
    osDelay(34);

    ui_init_default_init1();
    osDelay(34);

    ui_init_default_init2();
    osDelay(34);

    ui_init_default_init3();
    osDelay(34);

    ui_init_default_init4();
    osDelay(34);

    ui_init_default_init5();
    osDelay(34);

    ui_init_default_init6();

    return 1;
}

void RobotUI_Dynamic()
{
    static uint8_t UI_Init = 1;

    // //遥控离线监测
    // if(DBUS_V_DATA.IS_OFF)
    // {
    //     UI_Init = 0;
    // }

    //静态UI刷新
    // if (UI_Init != 1)
    // {
    //     UI_Init = RobotUI_Static_Init();
    // }

    //动态UI数据修改
    ui_default_init6_Dynamic0->_a++;
    ui_default_init6_Dynamic1->_a++;
    ui_default_init6_Dynamic2->_c++;
    ui_default_init6_Dynamic3->_c++;
    ui_default_init6_Dynamic4->_c++;
    ui_default_init6_Dynamic5->_c++;
    ui_default_init6_Dynamic6->_c++;

    //动态UI刷新
    ui_update_default_init6();
    vTaskDelay(2000);
}

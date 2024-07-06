#include "main.h"
#include <string.h>

#include "DBUS.h"
#include "ROOT.h"

// 遥控全局变量
TYPEDEF_DBUS DBUS_V_DATA = {0};
TYPEDEF_DBUS_UNION DBUS_V_UNION = {0}; // 共用体整合数据，待简化下成局部变量
/**
 * @brief 使用共用体整合遥控数据
 *
 * @param DBUS 解算后遥控数据
 */
void DBUS_F_Cal(TYPEDEF_DBUS *DBUS)
{
    DBUS->REMOTE.S1_u8 = DBUS_V_UNION.DATA_NEATEN.S1;
    DBUS->REMOTE.S2_u8 = DBUS_V_UNION.DATA_NEATEN.S2;
    DBUS->REMOTE.CH0_int16 = (int16_t)(DBUS_V_UNION.DATA_NEATEN.CH0 - 1024);
    DBUS->REMOTE.CH1_int16 = (int16_t)(DBUS_V_UNION.DATA_NEATEN.CH1 - 1024);
    DBUS->REMOTE.CH2_int16 = (int16_t)(DBUS_V_UNION.DATA_NEATEN.CH2 - 1024);
    DBUS->REMOTE.CH3_int16 = (int16_t)(DBUS_V_UNION.DATA_NEATEN.CH3 - 1024);
    DBUS->REMOTE.DIR_int16 = (int16_t)(DBUS_V_UNION.DATA_NEATEN.DIR - 1024);

    DBUS->KEY_BOARD.W = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_W);
    DBUS->KEY_BOARD.S = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_S);
    DBUS->KEY_BOARD.A = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_A);
    DBUS->KEY_BOARD.D = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_D);
    DBUS->KEY_BOARD.SHIFT = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_SHIFT);

    DBUS->KEY_BOARD.Q = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_Q);
    DBUS->KEY_BOARD.E = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_E);

    DBUS->KEY_BOARD.V = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_V);
    DBUS->KEY_BOARD.B = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_B);

    DBUS->KEY_BOARD.Z = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_Z);
    DBUS->KEY_BOARD.X = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_X);
    DBUS->KEY_BOARD.C = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_C);

    DBUS->KEY_BOARD.CTRL = (uint8_t)(DBUS_V_UNION.DATA_NEATEN.KEY_CTRL);

    // @TODO mouse to add

    // @TODO l/r flag to control atatck

    ROOT_V_MONITOR_DBUS = 0; //  离线判断参数清零
}

/**
 * @brief 离线清空遥控数据
 *
 * @param DBUS
 */
void DBUS_F_Offline(TYPEDEF_DBUS *DBUS)
{
    memset(DBUS, 0, sizeof(TYPEDEF_DBUS));
}

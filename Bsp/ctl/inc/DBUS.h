#ifndef __DBUS_H__
#define __DBUS_H__

typedef struct TYPEDEF_DBUS // 遥控器数据结构体
{
    uint8_t R_FLAG;
    uint8_t L_FLAG;

    // 遥控
    struct
    {
        int16_t CH0_int16;
        int16_t CH1_int16;
        int16_t CH2_int16;
        int16_t CH3_int16;
        int16_t DIR_int16;
        uint8_t S1_u8;
        uint8_t S2_u8;

        int8_t Error_int8;
    } REMOTE;
    // 鼠标
    struct
    {
        float X_FLT;
        float X_FILTER; // 处理后使用的鼠标值
        float X_MAX;    // 记录鼠标的最大值用来校准鼠标
        float Y_Flt;
        float Y_FILTER; // 处理后使用的鼠标值
        float Y_MAX;    // 记录鼠标的最大值用来校准鼠标
        uint8_t R_STATE : 4;
        uint8_t L_STATE : 4;
        uint8_t R_PRESS_TIME;
        uint8_t L_PRESS_TIME;
    } MOUSE;
    struct
    {
        uint8_t W : 4;
        uint8_t S : 4;
        uint8_t A : 4;
        uint8_t D : 4;
        uint8_t SHIFT : 4;
        uint8_t CTRL : 4;
        uint8_t Q : 4;
        uint8_t E : 4;
        uint8_t R : 4;
        uint8_t F : 4;
        uint8_t G : 4;
        uint8_t Z : 4;
        uint8_t X : 4;
        uint8_t C : 4;
        uint8_t V : 4;
        uint8_t B : 4;
        uint8_t W_PRESS_TIME;
        uint8_t S_PRESS_TIME;
        uint8_t A_PRESS_TIME;
        uint8_t D_PRESS_TIME;
        uint8_t Q_PREE_NUMBER : 1;
        uint8_t E_PREE_NUMBER : 1;
        uint8_t R_PREE_NUMBER : 1;
        uint8_t F_PREE_NUMBER : 1;
        uint8_t G_PREE_NUMBER : 1;
        uint8_t Z_PREE_NUMBER : 1;
        uint8_t X_PREE_NUMBER : 1;
        uint8_t C_PREE_NUMBER : 1;
        uint8_t SHIFT_PREE_NUMBER : 1;
        uint8_t CTRL_PREE_NUMBER : 1;
    } KEY_BOARD;
} TYPEDEF_DBUS;

typedef union TYPEDEF_DBUS_UNION // 使用共用体整合数据
{
    struct
    {
        // 遥控数据
        uint64_t CH0 : 11;
        uint64_t CH1 : 11;
        uint64_t CH2 : 11;
        uint64_t CH3 : 11;
        uint64_t S1 : 2;
        uint64_t S2 : 2;

        // 遥控滑轮
        uint64_t DIR : 11;
        uint64_t : 5;
    } DATA_NEATEN;
    // 接收到的数组
    uint8_t GET_DATA[19];
} TYPEDEF_DBUS_UNION;

extern TYPEDEF_DBUS DBUS_V_DATA;
extern TYPEDEF_DBUS_UNION DBUS_V_UNION;

extern void DBUS_F_Cal(TYPEDEF_DBUS *DBUS);

#endif // !__DBUS_H__

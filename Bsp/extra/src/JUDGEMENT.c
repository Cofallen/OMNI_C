#include "main.h"

#include "JUDGEMENT.h"
#include "DEFINE.h"

/***********************************    ↓    DJI提供的CRC校检函数   ↓  ***********************************/
const unsigned char RUI_V_CRC_8_INIT = 0xff;
const unsigned char RUI_V_CRC_8_TAB[256] =
    {
        0x00,
        0x5e,
        0xbc,
        0xe2,
        0x61,
        0x3f,
        0xdd,
        0x83,
        0xc2,
        0x9c,
        0x7e,
        0x20,
        0xa3,
        0xfd,
        0x1f,
        0x41,
        0x9d,
        0xc3,
        0x21,
        0x7f,
        0xfc,
        0xa2,
        0x40,
        0x1e,
        0x5f,
        0x01,
        0xe3,
        0xbd,
        0x3e,
        0x60,
        0x82,
        0xdc,
        0x23,
        0x7d,
        0x9f,
        0xc1,
        0x42,
        0x1c,
        0xfe,
        0xa0,
        0xe1,
        0xbf,
        0x5d,
        0x03,
        0x80,
        0xde,
        0x3c,
        0x62,
        0xbe,
        0xe0,
        0x02,
        0x5c,
        0xdf,
        0x81,
        0x63,
        0x3d,
        0x7c,
        0x22,
        0xc0,
        0x9e,
        0x1d,
        0x43,
        0xa1,
        0xff,
        0x46,
        0x18,
        0xfa,
        0xa4,
        0x27,
        0x79,
        0x9b,
        0xc5,
        0x84,
        0xda,
        0x38,
        0x66,
        0xe5,
        0xbb,
        0x59,
        0x07,
        0xdb,
        0x85,
        0x67,
        0x39,
        0xba,
        0xe4,
        0x06,
        0x58,
        0x19,
        0x47,
        0xa5,
        0xfb,
        0x78,
        0x26,
        0xc4,
        0x9a,
        0x65,
        0x3b,
        0xd9,
        0x87,
        0x04,
        0x5a,
        0xb8,
        0xe6,
        0xa7,
        0xf9,
        0x1b,
        0x45,
        0xc6,
        0x98,
        0x7a,
        0x24,
        0xf8,
        0xa6,
        0x44,
        0x1a,
        0x99,
        0xc7,
        0x25,
        0x7b,
        0x3a,
        0x64,
        0x86,
        0xd8,
        0x5b,
        0x05,
        0xe7,
        0xb9,
        0x8c,
        0xd2,
        0x30,
        0x6e,
        0xed,
        0xb3,
        0x51,
        0x0f,
        0x4e,
        0x10,
        0xf2,
        0xac,
        0x2f,
        0x71,
        0x93,
        0xcd,
        0x11,
        0x4f,
        0xad,
        0xf3,
        0x70,
        0x2e,
        0xcc,
        0x92,
        0xd3,
        0x8d,
        0x6f,
        0x31,
        0xb2,
        0xec,
        0x0e,
        0x50,
        0xaf,
        0xf1,
        0x13,
        0x4d,
        0xce,
        0x90,
        0x72,
        0x2c,
        0x6d,
        0x33,
        0xd1,
        0x8f,
        0x0c,
        0x52,
        0xb0,
        0xee,
        0x32,
        0x6c,
        0x8e,
        0xd0,
        0x53,
        0x0d,
        0xef,
        0xb1,
        0xf0,
        0xae,
        0x4c,
        0x12,
        0x91,
        0xcf,
        0x2d,
        0x73,
        0xca,
        0x94,
        0x76,
        0x28,
        0xab,
        0xf5,
        0x17,
        0x49,
        0x08,
        0x56,
        0xb4,
        0xea,
        0x69,
        0x37,
        0xd5,
        0x8b,
        0x57,
        0x09,
        0xeb,
        0xb5,
        0x36,
        0x68,
        0x8a,
        0xd4,
        0x95,
        0xcb,
        0x29,
        0x77,
        0xf4,
        0xaa,
        0x48,
        0x16,
        0xe9,
        0xb7,
        0x55,
        0x0b,
        0x88,
        0xd6,
        0x34,
        0x6a,
        0x2b,
        0x75,
        0x97,
        0xc9,
        0x4a,
        0x14,
        0xf6,
        0xa8,
        0x74,
        0x2a,
        0xc8,
        0x96,
        0x15,
        0x4b,
        0xa9,
        0xf7,
        0xb6,
        0xe8,
        0x0a,
        0x54,
        0xd7,
        0x89,
        0x6b,
        0x35,
};

uint8_t RUI_F_GET_CRC_8_CHECK_SUM(uint8_t *pchMessage, uint16_t dwLength, uint8_t ucCRC8)
{
    uint8_t UC_INDEX;

    while (dwLength--)
    {
        UC_INDEX = ucCRC8 ^ (*pchMessage++);
        ucCRC8 = RUI_V_CRC_8_TAB[UC_INDEX];
    }
    return ucCRC8;
}

uint32_t RUI_F_VERIFY_CRC_8_CHECK_SUM(uint8_t *pchMessage, uint16_t dwLength)
{
    uint8_t UC_EXPECTED = 0;

    if (pchMessage == NULL || dwLength <= 2)
    {
        return 0;
    }

    UC_EXPECTED = RUI_F_GET_CRC_8_CHECK_SUM(pchMessage, dwLength - 1, RUI_V_CRC_8_INIT);

    return (UC_EXPECTED == pchMessage[dwLength - 1]);
}

uint8_t ucCRC = 0;
void RUI_F_APPEND_CRC_8_CHECK_SUM(uint8_t *pchMessage, uint16_t dwLength)
{
    ucCRC = 0;

    if (pchMessage == NULL || dwLength <= 2)
    {
        return;
    }

    ucCRC = RUI_F_GET_CRC_8_CHECK_SUM((uint8_t *)pchMessage, dwLength - 1, RUI_V_CRC_8_INIT);

    pchMessage[dwLength - 1] = ucCRC;
}

uint16_t CRC_INIT = 0xffff;

const uint16_t RUI_V_WCRC_TABLE[256] =
    {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};

uint16_t RUI_F_GET_CRC_16_CHECK_SUM(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC)
{
    uint8_t CH_DATA;
    if (pchMessage == NULL)
    {
        return 0xFFFF;
    }
    while (dwLength--)
    {
        CH_DATA = *pchMessage++;
        (wCRC) = ((uint16_t)(wCRC) >> 8) ^ RUI_V_WCRC_TABLE[((uint16_t)(wCRC) ^ (uint16_t)(CH_DATA)) & 0x00ff];
    }
    return wCRC;
}

uint32_t RUI_F_VERIFY_CRC_16_CHECK_SUM(uint8_t *pchMessage, uint32_t dwLength)
{
    uint16_t W_EXPECTED = 0;
    if ((pchMessage == NULL) || (dwLength <= 2))
    {
        return 0;
    }
    W_EXPECTED = RUI_F_GET_CRC_16_CHECK_SUM(pchMessage, dwLength - 2, CRC_INIT);
    return ((W_EXPECTED & 0xff) == pchMessage[dwLength - 2] && ((W_EXPECTED >> 8) & 0xff) == pchMessage[dwLength - 1]);
}

void RUI_F_APPEND_CRC_16_CHECK_SUM(uint8_t *pchMessage, uint32_t dwLength)
{
    uint16_t W_CRC = 0;
    if ((pchMessage == NULL) || (dwLength <= 2))
    {
        return;
    }
    W_CRC = RUI_F_GET_CRC_16_CHECK_SUM((uint8_t *)pchMessage, dwLength - 2, CRC_INIT);
    pchMessage[dwLength - 2] = (uint8_t)(W_CRC & 0x00ff);
    pchMessage[dwLength - 1] = (uint8_t)((W_CRC >> 8) & 0x00ff);
}

/***********************************    ↑    DJI提供的CRC校检函数   ↑  ***********************************/

/************************************************************万能分隔符**************************************************************
 * 	@author:			//瑞 COPY 涵炜
 *	@performance:	    //
 *	@parameter:		    //@原始数据 @解析后的数据
 *	@time:				//24-3-7 下午7:41
 *	@ReadMe:			//裁判系统数据解析，将数据解析到用户数据结构体中
 ************************************************************万能分隔符**************************************************************/
uint8_t RUI_F_READ_DATA_SYSTEM(RUI_TYPEDEF_ALL_RX_DATA_T *ALL_RX_DATA, RUI_TYPEDEF_JUDGE *USER_DATA)
{
    if ((ALL_RX_DATA->RX_DATA.FRAME_HEADER.SOF == (uint8_t)0xA5) && (1 == RUI_F_VERIFY_CRC_8_CHECK_SUM(ALL_RX_DATA->DATA, 5)) && (1 == RUI_F_VERIFY_CRC_16_CHECK_SUM(ALL_RX_DATA->DATA, ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH + 9)))
    {
        ALL_RX_DATA->DATA[0]++;
        //        if (ALL_RX_DATA->RX_DATA.CMD_ID != 514)
        //        {
        //            RUI_D_LOG_INFO("CMD_ID:%d", ALL_RX_DATA->RX_DATA.CMD_ID);
        //        }
        switch (ALL_RX_DATA->RX_DATA.CMD_ID)
        {
        case GAME_STATE:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.GAME_STATUS) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->GAME_STATUS.GAME_PROGRESS = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_STATUS.GAME_PROGRESS;
                USER_DATA->GAME_STATUS.GAME_TYPE = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_STATUS.GAME_TYPE;
                USER_DATA->GAME_STATUS.STAGE_REMAIN_TIME = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_STATUS.STAGE_REMAIN_TIME;
                USER_DATA->GAME_STATUS.SYNC_TIME_STAMP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_STATUS.SYNC_TIME_STAMP;
                //                    RUI_D_LOG_INFO("SYNC_TIME_STAMP:%ld", USER_DATA->GAME_STATUS.SYNC_TIME_STAMP)
            }
            break;
        }

        case MATCH_RESULTS:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.GAME_RESULT) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->GAME_RESULT.WINNER = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_RESULT.WINNER;
            }
            break;
        }

        case ROBOT_HP:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->GAME_ROBOT_HP.BLUE_1_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_1_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.BLUE_2_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_2_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.BLUE_3_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_3_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.BLUE_4_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_4_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.BLUE_5_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_5_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.BLUE_7_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_7_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.BLUE_BASE_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_BASE_HP;
                USER_DATA->GAME_ROBOT_HP.BLUE_OUTPOST_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.BLUE_OUTPOST_HP;
                USER_DATA->GAME_ROBOT_HP.RED_1_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_1_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.RED_2_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_2_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.RED_3_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_3_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.RED_4_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_4_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.RED_5_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_5_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.RED_7_ROBOT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_7_ROBOT_HP;
                USER_DATA->GAME_ROBOT_HP.RED_BASE_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_BASE_HP;
                USER_DATA->GAME_ROBOT_HP.RED_OUTPOST_HP = ALL_RX_DATA->RX_DATA.READ_DATA.GAME_ROBOT_HP.RED_OUTPOST_HP;
            }
            break;
        }

        case VENUE_EVENTS:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->EVENT_DATA.BUFF_ACTIVATED = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.BUFF_ACTIVATED;
                USER_DATA->EVENT_DATA.BUFF_BIG = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.BUFF_BIG;
                USER_DATA->EVENT_DATA.BUFF_SMALL = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.BUFF_SMALL;
                USER_DATA->EVENT_DATA.CIRCULAR_HEIGHTS = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.CIRCULAR_HEIGHTS;
                USER_DATA->EVENT_DATA.DART_HIT_AIM = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.DART_HIT_AIM;
                USER_DATA->EVENT_DATA.DART_HIT_TIME = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.DART_HIT_TIME;
                USER_DATA->EVENT_DATA.REFUELING_BLOOD = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.REFUELING_BLOOD;
                USER_DATA->EVENT_DATA.RESERVED_BITS_1 = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.RESERVED_BITS_1;
                USER_DATA->EVENT_DATA.RESERVED_BITS_2 = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.RESERVED_BITS_2;
                USER_DATA->EVENT_DATA.SUPPLY_BLOOD = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.SUPPLY_BLOOD;
                USER_DATA->EVENT_DATA.TRAPEZOIDAL_HEIGHTS_3 = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.TRAPEZOIDAL_HEIGHTS_3;
                USER_DATA->EVENT_DATA.TRAPEZOIDAL_HEIGHTS_4 = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.TRAPEZOIDAL_HEIGHTS_4;
                USER_DATA->EVENT_DATA.VIRTUAL_SHIELD = ALL_RX_DATA->RX_DATA.READ_DATA.EVENT_DATA.VIRTUAL_SHIELD;
            }
            break;
        }

        case SUPPLY_ACTION:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.EXT_SUPPLY_PROJECTILE_ACTION) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->EXT_SUPPLY_PROJECTILE_ACTION.RESERVED = ALL_RX_DATA->RX_DATA.READ_DATA.EXT_SUPPLY_PROJECTILE_ACTION.RESERVED;
                USER_DATA->EXT_SUPPLY_PROJECTILE_ACTION.SUPPLY_PROJECTILE_NUM = ALL_RX_DATA->RX_DATA.READ_DATA.EXT_SUPPLY_PROJECTILE_ACTION.SUPPLY_PROJECTILE_NUM;
                USER_DATA->EXT_SUPPLY_PROJECTILE_ACTION.SUPPLY_PROJECTILE_STEP = ALL_RX_DATA->RX_DATA.READ_DATA.EXT_SUPPLY_PROJECTILE_ACTION.SUPPLY_PROJECTILE_STEP;
                USER_DATA->EXT_SUPPLY_PROJECTILE_ACTION.SUPPLY_ROBOT_ID = ALL_RX_DATA->RX_DATA.READ_DATA.EXT_SUPPLY_PROJECTILE_ACTION.SUPPLY_ROBOT_ID;
            }
            break;
        }

        case REFEREE_WARNING:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.referee_warning) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->REFEREE_WARNING.COUNT = ALL_RX_DATA->RX_DATA.READ_DATA.referee_warning.COUNT;
                USER_DATA->REFEREE_WARNING.LEVEL = ALL_RX_DATA->RX_DATA.READ_DATA.referee_warning.LEVEL;
                USER_DATA->REFEREE_WARNING.OFFENDING_ROBOT_ID = ALL_RX_DATA->RX_DATA.READ_DATA.referee_warning.OFFENDING_ROBOT_ID;
            }
            break;
        }

        case DART_FIRE:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.dart_info) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->DART_INFO.DART_AIMED = ALL_RX_DATA->RX_DATA.READ_DATA.dart_info.DART_AIMED;
                USER_DATA->DART_INFO.DART_AIMING = ALL_RX_DATA->RX_DATA.READ_DATA.dart_info.DART_AIMING;
                USER_DATA->DART_INFO.DART_REMAINING_TIME = ALL_RX_DATA->RX_DATA.READ_DATA.dart_info.DART_REMAINING_TIME;
                USER_DATA->DART_INFO.DART_SUCCESS = ALL_RX_DATA->RX_DATA.READ_DATA.dart_info.DART_SUCCESS;
                USER_DATA->DART_INFO.RESERVED_BITS = ALL_RX_DATA->RX_DATA.READ_DATA.dart_info.RESERVED_BITS;
            }
            break;
        }

        case ROBOT_PERFORMAN:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->ROBOT_STATUS.MAXIMUM_HP = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.MAXIMUM_HP;
                USER_DATA->ROBOT_STATUS.CURRENT_HP = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.CURRENT_HP;
                USER_DATA->ROBOT_STATUS.POWER_MANAGEMENT_CHASSIS_OUTPUT = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.POWER_MANAGEMENT_CHASSIS_OUTPUT;
                USER_DATA->ROBOT_STATUS.POWER_MANAGEMENT_GIMBAL_OUTPUT = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.POWER_MANAGEMENT_GIMBAL_OUTPUT;
                USER_DATA->ROBOT_STATUS.POWER_MANAGEMENT_SHOOTER_OUTPUT = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.POWER_MANAGEMENT_SHOOTER_OUTPUT;
                USER_DATA->ROBOT_STATUS.ROBOT_ID = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.ROBOT_ID;
                USER_DATA->ROBOT_STATUS.ROBOT_LEVEL = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.ROBOT_LEVEL;
                USER_DATA->ROBOT_STATUS.SHOOTER_BARREL_HEAT_LIMIT = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.ROBOT_LEVEL;
                USER_DATA->ROBOT_STATUS.SHOOTER_BARREL_COOLING_VALUE = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.ROBOT_LEVEL;
                USER_DATA->ROBOT_STATUS.CHASSIS_POWER_LIMIT = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_STATUS.CHASSIS_POWER_LIMIT;
                //                    RUI_D_LOG_INFO("MAXIMUM_HP %d ", USER_DATA->ROBOT_STATUS.CHASSIS_POWER_LIMIT);
            }
            break;
        }

        case TIME_POWER: // 514
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->POWER_HEAT_DATA.BUFFER_ENERGY = ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA.BUFFER_ENERGY;
                USER_DATA->POWER_HEAT_DATA.CHASSIS_CURRENT = ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA.CHASSIS_CURRENT;
                USER_DATA->POWER_HEAT_DATA.CHASSIS_POWER = ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA.CHASSIS_POWER;
                USER_DATA->POWER_HEAT_DATA.CHASSIS_VOLTAGE = ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA.CHASSIS_VOLTAGE;
                USER_DATA->POWER_HEAT_DATA.SHOOTER_17_MM_1_BARREL_HEAT = ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA.SHOOTER_17_MM_1_BARREL_HEAT;
                USER_DATA->POWER_HEAT_DATA.SHOOTER_17_MM_2_BARREL_HEAT = ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA.SHOOTER_17_MM_2_BARREL_HEAT;
                USER_DATA->POWER_HEAT_DATA.SHOOTER_42_MM_BARREL_HEAT = ALL_RX_DATA->RX_DATA.READ_DATA.POWER_HEAT_DATA.SHOOTER_42_MM_BARREL_HEAT;
                //                    RUI_D_LOG_INFO("CHASSIS_POWER %f LIMIT %d", USER_DATA->POWER_HEAT_DATA.CHASSIS_POWER, USER_DATA->ROBOT_STATUS.CHASSIS_POWER_LIMIT);
            }
            break;
        }

        case ROBOT_LOCATION:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_POS) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->ROBOT_POS.ANGLE = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_POS.ANGLE;
                USER_DATA->ROBOT_POS.X = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_POS.X;
                USER_DATA->ROBOT_POS.Y = ALL_RX_DATA->RX_DATA.READ_DATA.ROBOT_POS.Y;
            }
            break;
        }

        case ROBOT_BUFF: // 516
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.BUFF) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->BUFF.ATTACK_BUFF = ALL_RX_DATA->RX_DATA.READ_DATA.BUFF.ATTACK_BUFF;
                USER_DATA->BUFF.COOLING_BUFF = ALL_RX_DATA->RX_DATA.READ_DATA.BUFF.COOLING_BUFF;
                USER_DATA->BUFF.DEFENCE_BUFF = ALL_RX_DATA->RX_DATA.READ_DATA.BUFF.DEFENCE_BUFF;
                USER_DATA->BUFF.RECOVERY_BUFF = ALL_RX_DATA->RX_DATA.READ_DATA.BUFF.RECOVERY_BUFF;
            }
            break;
        }

        case AIR_SUPPORT:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.AIR_SUPPORT_DATA) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->AIR_SUPPORT_DATA.AIRFORCE_STATUS = ALL_RX_DATA->RX_DATA.READ_DATA.AIR_SUPPORT_DATA.AIRFORCE_STATUS;
                USER_DATA->AIR_SUPPORT_DATA.TIME_REMAIN = ALL_RX_DATA->RX_DATA.READ_DATA.AIR_SUPPORT_DATA.TIME_REMAIN;
            }
            break;
        }

        case DAMAGE_STATUS:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.HURT_DATA) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->HURT_DATA.ARMOR_ID = ALL_RX_DATA->RX_DATA.READ_DATA.HURT_DATA.ARMOR_ID;
                USER_DATA->HURT_DATA.HP_DEDUCTION_REASON = ALL_RX_DATA->RX_DATA.READ_DATA.HURT_DATA.HP_DEDUCTION_REASON;
            }
            break;
        }

        case TIME_SHOOTING:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.SHOOT_DATA) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->SHOOT_DATA.BULLET_TYPE = ALL_RX_DATA->RX_DATA.READ_DATA.SHOOT_DATA.BULLET_TYPE;
                USER_DATA->SHOOT_DATA.INITIAL_SPEED = ALL_RX_DATA->RX_DATA.READ_DATA.SHOOT_DATA.INITIAL_SPEED;
                USER_DATA->SHOOT_DATA.LAUNCHING_FREQUENCY = ALL_RX_DATA->RX_DATA.READ_DATA.SHOOT_DATA.LAUNCHING_FREQUENCY;
                USER_DATA->SHOOT_DATA.SHOOTER_NUMBER = ALL_RX_DATA->RX_DATA.READ_DATA.SHOOT_DATA.SHOOTER_NUMBER;
            }
            break;
        }

        case ALLOWABLE_AMMO: // 520
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.PROJECTILE_ALLOWANCE) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->PROJECTILE_ALLOWANCE.PROJECTILE_ALLOWANCE_17_MM = ALL_RX_DATA->RX_DATA.READ_DATA.PROJECTILE_ALLOWANCE.PROJECTILE_ALLOWANCE_17_MM;
                USER_DATA->PROJECTILE_ALLOWANCE.PROJECTILE_ALLOWANCE_42_MM = ALL_RX_DATA->RX_DATA.READ_DATA.PROJECTILE_ALLOWANCE.PROJECTILE_ALLOWANCE_42_MM;
                USER_DATA->PROJECTILE_ALLOWANCE.REMAINING_GOLD_COIN = ALL_RX_DATA->RX_DATA.READ_DATA.PROJECTILE_ALLOWANCE.REMAINING_GOLD_COIN;
            }
            break;
        }

        case RFID: // 521
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->RFID_STATUS.BIT_0 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_0;
                USER_DATA->RFID_STATUS.BIT_1 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_1;
                USER_DATA->RFID_STATUS.BIT_2 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_2;
                USER_DATA->RFID_STATUS.BIT_3 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_3;
                USER_DATA->RFID_STATUS.BIT_4 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_4;
                USER_DATA->RFID_STATUS.BIT_5 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_5;
                USER_DATA->RFID_STATUS.BIT_6 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_6;
                USER_DATA->RFID_STATUS.BIT_7 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_7;
                USER_DATA->RFID_STATUS.BIT_8 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_8;
                USER_DATA->RFID_STATUS.BIT_9 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_9;
                USER_DATA->RFID_STATUS.BIT_10 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_10;
                USER_DATA->RFID_STATUS.BIT_11 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_11;
                USER_DATA->RFID_STATUS.BIT_12 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_12;
                USER_DATA->RFID_STATUS.BIT_13 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_13;
                USER_DATA->RFID_STATUS.BIT_14 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_14;
                USER_DATA->RFID_STATUS.BIT_15 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_15;
                USER_DATA->RFID_STATUS.BIT_16 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_16;
                USER_DATA->RFID_STATUS.BIT_17 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_17;
                USER_DATA->RFID_STATUS.BIT_18 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_18;
                USER_DATA->RFID_STATUS.BIT_19 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_19;
                USER_DATA->RFID_STATUS.BIT_20_31 = ALL_RX_DATA->RX_DATA.READ_DATA.RFID_STATUS.BIT_20_31;
            }
            break;
        }

        case DART_DIRECTIVES:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.DART_CLIENT_CMD) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->DART_CLIENT_CMD.RESERVED = ALL_RX_DATA->RX_DATA.READ_DATA.DART_CLIENT_CMD.RESERVED;
                USER_DATA->DART_CLIENT_CMD.DART_LAUNCH_OPENING_STATUS = ALL_RX_DATA->RX_DATA.READ_DATA.DART_CLIENT_CMD.DART_LAUNCH_OPENING_STATUS;
                USER_DATA->DART_CLIENT_CMD.LATEST_LAUNCH_CMD_TIME = ALL_RX_DATA->RX_DATA.READ_DATA.DART_CLIENT_CMD.LATEST_LAUNCH_CMD_TIME;
                USER_DATA->DART_CLIENT_CMD.TARGET_CHANGE_TIME = ALL_RX_DATA->RX_DATA.READ_DATA.DART_CLIENT_CMD.TARGET_CHANGE_TIME;
            }
            break;
        }

        case GROUND_LOCATION:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->GROUND_ROBOT_POSITION.ENGINEER_X = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.ENGINEER_X;
                USER_DATA->GROUND_ROBOT_POSITION.ENGINEER_Y = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.ENGINEER_Y;
                USER_DATA->GROUND_ROBOT_POSITION.HERO_X = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.HERO_X;
                USER_DATA->GROUND_ROBOT_POSITION.HERO_Y = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.HERO_Y;
                USER_DATA->GROUND_ROBOT_POSITION.STANDARD_3_X = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.STANDARD_3_X;
                USER_DATA->GROUND_ROBOT_POSITION.STANDARD_3_Y = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.STANDARD_3_Y;
                USER_DATA->GROUND_ROBOT_POSITION.STANDARD_4_X = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.STANDARD_4_X;
                USER_DATA->GROUND_ROBOT_POSITION.STANDARD_4_Y = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.STANDARD_4_Y;
                USER_DATA->GROUND_ROBOT_POSITION.STANDARD_5_X = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.STANDARD_5_X;
                USER_DATA->GROUND_ROBOT_POSITION.STANDARD_5_Y = ALL_RX_DATA->RX_DATA.READ_DATA.GROUND_ROBOT_POSITION.STANDARD_5_Y;
            }
            break;
        }

        case RADAR_MARKING:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_MARK_DATA) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->RADAR_MARK_DATA.MARK_ENGINEER_PROGRESS = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_MARK_DATA.MARK_ENGINEER_PROGRESS;
                USER_DATA->RADAR_MARK_DATA.MARK_HERO_PROGRESS = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_MARK_DATA.MARK_HERO_PROGRESS;
                USER_DATA->RADAR_MARK_DATA.MARK_SENTRY_PROGRESS = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_MARK_DATA.MARK_SENTRY_PROGRESS;
                USER_DATA->RADAR_MARK_DATA.MARK_STANDARD_3_PROGRESS = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_MARK_DATA.MARK_STANDARD_3_PROGRESS;
                USER_DATA->RADAR_MARK_DATA.MARK_STANDARD_4_PROGRESS = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_MARK_DATA.MARK_STANDARD_4_PROGRESS;
                USER_DATA->RADAR_MARK_DATA.MARK_STANDARD_5_PROGRESS = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_MARK_DATA.MARK_STANDARD_5_PROGRESS;
            }
            break;
        }

        case ROUTE_INFORMAT:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.SENTRY_INFO) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->SENTRY_INFO.REMOTE_HP = ALL_RX_DATA->RX_DATA.READ_DATA.SENTRY_INFO.REMOTE_HP;
                USER_DATA->SENTRY_INFO.REMOTE_SUCCESSFULLY_REDEEMED = ALL_RX_DATA->RX_DATA.READ_DATA.SENTRY_INFO.REMOTE_SUCCESSFULLY_REDEEMED;
                USER_DATA->SENTRY_INFO.RESERVED_BITS = ALL_RX_DATA->RX_DATA.READ_DATA.SENTRY_INFO.RESERVED_BITS;
                USER_DATA->SENTRY_INFO.SUCCESSFULLY_REDEEMED = ALL_RX_DATA->RX_DATA.READ_DATA.SENTRY_INFO.SUCCESSFULLY_REDEEMED;
            }
            break;
        }

        case RADAR_INFORMAT:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_INFO) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->RADAR_INFO.VULNERABLE_BEGIN = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_INFO.VULNERABLE_BEGIN;
                USER_DATA->RADAR_INFO.VULNERABLE_NOW = ALL_RX_DATA->RX_DATA.READ_DATA.RADAR_INFO.VULNERABLE_NOW;
            }
            break;
        }

        case MINIMAP:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.MAP_COMMAND) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->MAP_COMMAND.CMD_KEYBOARD = ALL_RX_DATA->RX_DATA.READ_DATA.MAP_COMMAND.CMD_KEYBOARD;
                USER_DATA->MAP_COMMAND.CMD_SOURCE = ALL_RX_DATA->RX_DATA.READ_DATA.MAP_COMMAND.CMD_SOURCE;
                USER_DATA->MAP_COMMAND.TARGET_POSITION_X = ALL_RX_DATA->RX_DATA.READ_DATA.MAP_COMMAND.TARGET_POSITION_X;
                USER_DATA->MAP_COMMAND.TARGET_POSITION_Y = ALL_RX_DATA->RX_DATA.READ_DATA.MAP_COMMAND.TARGET_POSITION_Y;
                USER_DATA->MAP_COMMAND.TARGET_ROBOT_ID = ALL_RX_DATA->RX_DATA.READ_DATA.MAP_COMMAND.TARGET_ROBOT_ID;
            }
            break;
        }

        case REMOTE:
        {
            if (sizeof(ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL) == ALL_RX_DATA->RX_DATA.FRAME_HEADER.DATA_LENTH)
            {
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_A = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_A;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_B = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_B;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_C = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_C;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_CTRL = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_CTRL;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_D = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_D;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_E = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_E;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_F = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_F;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_G = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_G;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_Q = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_Q;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_R = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_R;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_S = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_S;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_SHIFT = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_SHIFT;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_V = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_V;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_W = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_W;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_X = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_X;
                USER_DATA->REMOTE_CONTROL.KEYBOARD_VALUE_Z = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.KEYBOARD_VALUE_Z;
                USER_DATA->REMOTE_CONTROL.LEFT_BUTTON_DOWN = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.LEFT_BUTTON_DOWN;
                USER_DATA->REMOTE_CONTROL.MOUSE_X = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.MOUSE_X;
                USER_DATA->REMOTE_CONTROL.MOUSE_Y = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.MOUSE_Y;
                USER_DATA->REMOTE_CONTROL.MOUSE_Z = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.MOUSE_Z;
                USER_DATA->REMOTE_CONTROL.RESERVED = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.RESERVED;
                USER_DATA->REMOTE_CONTROL.RIGHT_BUTTON_DOWN = ALL_RX_DATA->RX_DATA.READ_DATA.REMOTE_CONTROL.RIGHT_BUTTON_DOWN;
            }
            break;
        }
        default:
            return ROOT_ERROR; // 没有对应的ID
        }
    }
    else
    {
        return ROOT_ERROR; // 校验不通过
    }
    return ROOT_READY;
}

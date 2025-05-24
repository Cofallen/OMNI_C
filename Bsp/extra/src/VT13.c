#include "VT13.h"
#include "string.h"
#include "math.h"

float OneFilter(float last , float now , float thresholdValue);
uint8_t RUI_F_KEY_STATUS(uint64_t  KEY , uint8_t PRESS_TIME);
float float_deadbandv(float Value, float minValue, float maxValue);
static uint16_t get_crc16_check_sum(uint8_t *p_msg, uint16_t len, uint16_t crc16);
bool verify_crc16_check_sum(uint8_t *p_msg, uint16_t len);

#define RUI_DF_KEY_CLICK 1  // 点按
#define RUI_DF_KEY_PRESS 2  // 长按
#define RUI_DF_KEY_UP 0     // 无按

VT13_Typedef VT_V_DATA = {0}; // 视觉数据
VT13_UNION_Typdef VT13_V_UNION = {0}; // 视觉数据共用体

void VT13_Resovled(uint8_t* Data, VT13_Typedef* VT13)
{
	if(Data[0] == 0XA9 && Data[1] == 0X53 && (verify_crc16_check_sum(Data,21) == 1))
	{
		VT13->CRC_flag = 1;//У��ͨ��
		VT13_UNION_Typdef VT13_UNION = {0};
		static uint8_t Mouse_R_Lock = 0;
		static uint8_t Mouse_L_Lock = 0;
		
		memcpy(VT13_UNION.GetData , Data , 21);
		
		//ң������
		VT13->Remote.Channel[0] = VT13_UNION.DataNeaten.ch_0 - 1024;
		VT13->Remote.Channel[1] = VT13_UNION.DataNeaten.ch_1 - 1024;
		VT13->Remote.Channel[2] = VT13_UNION.DataNeaten.ch_2 - 1024;
		VT13->Remote.Channel[3] = VT13_UNION.DataNeaten.ch_3 - 1024;
		
		VT13->Remote.wheel = VT13_UNION.DataNeaten.wheel - 1024;
		
		VT13->Remote.mode_sw = VT13_UNION.DataNeaten.mode_sw;
		VT13->Remote.pause = VT13_UNION.DataNeaten.pause;
		VT13->Remote.fn_1 = VT13_UNION.DataNeaten.fn_1;
		VT13->Remote.fn_2 = VT13_UNION.DataNeaten.fn_2;
		VT13->Remote.trigger = VT13_UNION.DataNeaten.trigger;
		
		//*�Ե㰴�ͳ���������*//
		VT13->Mouse.L_State = RUI_F_KEY_STATUS(VT13_UNION.DataNeaten.mouse_left , VT13->Mouse.L_PressTime);
		VT13->Mouse.R_State = RUI_F_KEY_STATUS(VT13_UNION.DataNeaten.mouse_right  , VT13->Mouse.R_PressTime);
		VT13->Mouse.M_State = RUI_F_KEY_STATUS(VT13_UNION.DataNeaten.mouse_middle  , VT13->Mouse.M_PressTime);
		
		//*���̵İ���*//
		VT13->KeyBoard.W = RUI_F_KEY_STATUS(VT13_UNION.DataNeaten.KeyBoard_W , VT13->KeyBoard.W_PressTime);
		VT13->KeyBoard.A = RUI_F_KEY_STATUS(VT13_UNION.DataNeaten.KeyBoard_A , VT13->KeyBoard.A_PressTime);
		VT13->KeyBoard.S = RUI_F_KEY_STATUS(VT13_UNION.DataNeaten.KeyBoard_S , VT13->KeyBoard.S_PressTime);
		VT13->KeyBoard.D = RUI_F_KEY_STATUS(VT13_UNION.DataNeaten.KeyBoard_D , VT13->KeyBoard.D_PressTime);
		
		//*����˲�*//
		VT13->Mouse.X_Flt = OneFilter(VT13->Mouse.X_Flt , (float) VT13_UNION.DataNeaten.mouse_x , 500);
		VT13->Mouse.Y_Flt = OneFilter(VT13->Mouse.Y_Flt , (float) VT13_UNION.DataNeaten.mouse_y , 500);
		VT13->Mouse.Z_Flt = OneFilter(VT13->Mouse.Z_Flt , (float) VT13_UNION.DataNeaten.mouse_z , 500);
		
		//*�������*//
		VT13->Mouse.X_Flt = float_deadbandv(VT13->Mouse.X_Flt,-3.0e-3,3.0e-3);
		VT13->Mouse.Y_Flt = float_deadbandv(VT13->Mouse.Y_Flt,-3.0e-3,3.0e-3);
		VT13->Mouse.Z_Flt = float_deadbandv(VT13->Mouse.Z_Flt,-3.0e-3,3.0e-3);
		
		// Shift
		VT13->KeyBoard.Shift = VT13_UNION.DataNeaten.KeyBoard_Shift;
		VT13->KeyBoard.Ctrl = VT13_UNION.DataNeaten.KeyBoard_Ctrl;
		VT13->KeyBoard.Q = VT13_UNION.DataNeaten.KeyBoard_Q;
		VT13->KeyBoard.E = VT13_UNION.DataNeaten.KeyBoard_E;
		VT13->KeyBoard.R = VT13_UNION.DataNeaten.KeyBoard_R;
		VT13->KeyBoard.F = VT13_UNION.DataNeaten.KeyBoard_F;
		VT13->KeyBoard.G = VT13_UNION.DataNeaten.KeyBoard_G;
		VT13->KeyBoard.X = VT13_UNION.DataNeaten.KeyBoard_X;
		VT13->KeyBoard.C = VT13_UNION.DataNeaten.KeyBoard_C;
		VT13->KeyBoard.V = VT13_UNION.DataNeaten.KeyBoard_V;
		VT13->KeyBoard.B = VT13_UNION.DataNeaten.KeyBoard_B;
		if (VT13_UNION.DataNeaten.mouse_left == 1)
		{
			VT13->Mouse.L_PressTime++;
				
				if(VT13->Mouse.L_PressTime > 250)//��ѹʱ��ӽ��߽�ʱ�����ð�ѹʱ�䣬��ֹ����ʱ�����״̬��ɵ���-------����
				{VT13->Mouse.L_PressTime = 0;}
		}
		else if (VT13_UNION.DataNeaten.mouse_left == 0)
		{
			VT13->Mouse.L_PressTime = 0;
		}
		
		//RUI_V_DBUS->Mouse.R_State = RUI_V_DBUS_UNION.DataNeaten.Mouse_R;
		if (VT13_UNION.DataNeaten.mouse_right == 1)
		{
			VT13->Mouse.R_PressTime++;
				
				if(VT13->Mouse.R_PressTime > 250)
				{
					VT13->Mouse.R_PressTime = 0;
				}
		}
		else if (VT13_UNION.DataNeaten.mouse_right == 0)
		{
					VT13->Mouse.R_PressTime = 0;
		}
		
		if (VT13_UNION.DataNeaten.mouse_middle == 1)
		{
			VT13->Mouse.M_PressTime++;
				
				if(VT13->Mouse.M_PressTime > 250)
				{
					VT13->Mouse.M_PressTime = 0;
				}
		}
		else if (VT13_UNION.DataNeaten.mouse_middle == 0)
		{
					VT13->Mouse.M_PressTime = 0;
		}
		VT13->RUI_V_VT13_ONLINE_JUDGE_TIME = 0;
		VT13->IS_OFF = 0;
	}
	else
	{
		VT13->CRC_flag = 0;//У�鲻ͨ��
	}
}


/**
 * @brief Get the crc16 checksum
 *
 * @param p_msg Data to check
 * @param lenData length
 * @param crc16 Crc16 initialized checksum
 * @return crc16 Crc16 checksum
 */
static uint16_t get_crc16_check_sum(uint8_t *p_msg, uint16_t len, uint16_t crc16)
{
    uint8_t data;

    if(p_msg == NULL)
    {
        return 0xffff;
    }

    while(len--)
    {
        data = *p_msg++;
        (crc16) = ((uint16_t)(crc16) >> 8) ^ crc16_tab[((uint16_t)(crc16) ^ (uint16_t)(data)) & 0x00ff];
    }

    return crc16;
}

/**
 * @brief crc16 verify function
 *
 * @param p_msg Data to verify
 * @param len Stream length=data+checksum
 * @return bool Crc16 check result
 */
bool verify_crc16_check_sum(uint8_t *p_msg, uint16_t len)
{
    uint16_t w_expected = 0;

    if((p_msg == NULL) || (len <= 2))
    {
        return false;
    }
    w_expected = get_crc16_check_sum(p_msg, len - 2, crc16_init);

    return ((w_expected & 0xff) == p_msg[len - 2] && ((w_expected >> 8) & 0xff) == p_msg[len - 1]);
}

/************************************************************���ָܷ���**************************************************************
 * 	@author:			//С�� COPY ����
 *	@performance:	    //����˲�
 *	@parameter:		    //��һ�ε�ֵ//��ǰֵ//������ж�
 *	@time:				//22-11-23 16:39
 *	@ReadMe:			//ʹ��һ�׵�ͨ�˲�(�Ľ���)
                        //������жȣ�ԽС�Լ��Խ����	(һ��ȡֵΪ���ֵ��20%)
 ************************************************************���ָܷ���**************************************************************/
float OneFilter(float last , float now , float thresholdValue)
{
	// ��Сƽ���˲�ֵ���������ϸСë�̵Ĺ��˳̶�
	// ���Ӽ���˲�ֵ��������ڼ����ֵ����Ӧ�̶�
	const float sensitivlFilter = 0.8f; // ����˲�ֵ//С��1
	const float numbFilter = 0.2f; // ƽ���˲�ֵ//С��1
	
	if (fabsf(fabsf(last) - fabsf(now)) >= thresholdValue)
	{
		return (float) (now * sensitivlFilter + last * (1 - sensitivlFilter));
	}
	else
	{
		return (float) (now * numbFilter + last * (1 - numbFilter));
	}
}
/************************************************************���ָܷ���**************************************************************
 * 	@author:			//С��
 *	@performance:	    //���������̰�
 *	@parameter:		    //
 *	@time:				//23-04-26 21:40
 *	@ReadMe:			//
 ************************************************************���ָܷ���**************************************************************/
uint8_t RUI_F_KEY_STATUS(uint64_t  KEY , uint8_t PRESS_TIME)
{
	static uint8_t press_time = 0;
	if (KEY == 1)
	{
		if (PRESS_TIME <= 10)
		{
			PRESS_TIME++;
			return RUI_DF_KEY_CLICK;
		}
		else
		{
			return  RUI_DF_KEY_PRESS; // ����
		}
	}
	else
	{
		PRESS_TIME = 0;
		return  RUI_DF_KEY_UP;
	}
	
	
}

//��������
float float_deadbandv(float Value, float minValue, float maxValue)
{
	if (Value < maxValue && Value > minValue)
	{
		Value = 0.0f;
	}
	return Value;
}


void VT_F_Offline(VT13_Typedef *vt)
{
    memset(vt, 0, sizeof(VT13_Typedef));
    vt->IS_OFF = 1;
}


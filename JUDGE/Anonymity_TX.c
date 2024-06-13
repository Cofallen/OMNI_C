#include "Anonymity_TX.h"

uint8_t anonymity_au8[df_AnonymityLong] ={ 0 };

void Anonymity_TX(int16_t a_x , int16_t a_y , int16_t a_z , int16_t g_x , int16_t g_y , int16_t g_z , int16_t m_x , int16_t m_y , int16_t m_z , int32_t bar)
{
	//匿名V7发送协议
	uint8_t _cnt=0 , sum=0 , add=0 , i=0;
	int16_t _temp;

	anonymity_au8[_cnt++]=0xAA;
	anonymity_au8[_cnt++]=0xFF;
	anonymity_au8[_cnt++]=0xF1;
	anonymity_au8[_cnt++]=0;				//176

	_temp = a_x;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);
	_temp = a_y;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);
	_temp = a_z;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);

	_temp = g_x;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);
	_temp = g_y;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);
	_temp = g_z;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);

	_temp = m_x;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);
	_temp = m_y;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);
	_temp = m_z;
	anonymity_au8[_cnt++]=BYTE0(_temp);
	anonymity_au8[_cnt++]=BYTE1(_temp);

	anonymity_au8[3] = _cnt-4;

	for (i=0; (i<anonymity_au8[3]+4); i++)
	{
		sum += anonymity_au8[i];
		add += sum;
	}
	anonymity_au8[_cnt++]=sum;
	anonymity_au8[_cnt++]=add;
	//这里是对应的发送语句
	HAL_UART_Transmit_DMA(&huart3,anonymity_au8, _cnt);

}

void powerTX(void)
{
	Anonymity_TX((int16_t) 0	//1	底盘功率
							, 0	//2	//设定值
							, 0		//3		当前状态
							, 0
							, 0	//5
							, 0	//6
							, 0	//7
							, 0	//8
							, 0	//9	
							, 200);

}

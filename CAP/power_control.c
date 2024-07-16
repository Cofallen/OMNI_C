#include "power_control.h"
#include "superCap.h"
#include "TIM_DEV.h"
#include "main.h"
#include "MOTOR.h"
#include "math.h"
#include "PID.h"
#include "shu_math.h"
TYPEDEF_MOTOR_PID PID_Buffer;
	
fp32 scaled_give_power[4];

fp32 toque_coefficient = 1.99688994e-6f; // (20/16384)*(0.3)*(187/3591)/9.55 ���ص���ϵ��
fp32 a = 1.23e-07;						 // k1
fp32 k2 = 1.453e-07;					 // k2
fp32 constant = 4.081f;                  // a �������ϵ�����Լ�С���ʣ���֮����

/**
  * @author: �
  * @performance: ���ʿ����ܺ���
  * @parameter: ���ݱ�־λ���Ƿ�����	
  * @time: 24-4-1			
  * @ReadMe: ���ڵ���PID����󼴿�
*/ 
void chassis_power_control(uint8_t cap_state)
{
    //*�ɱ༭����*begin*//
    const uint16_t PowerCompensation = 10;  //����ģʽ�µĹ��ʲ���
    const uint16_t SuperMaxPower = 150;	    //���������µĹ��ʲ���
    const uint16_t capValt = 140;	        //ǿ���˳��ĵ�ѹ��ֵ
    //*�ɱ༭����*end*//
	
	uint16_t max_power_limit = 50;  //���������
	fp32 chassis_max_power = 0;
	fp32 input_power = 0;		    // ���빦�ʣ�����ϵͳ��
	fp32 initial_give_power[4];    // ��ʼ������PID�����Լ�������ݵõ�
	fp32 initial_total_power = 0;
		
	fp32 chassis_power = 0.0f;
	fp32 chassis_power_buffer = 0.0f;
	
    chassis_power = user_data.power_heat_data.chassis_power;		// �õ����̹���
    chassis_power_buffer = user_data.power_heat_data.buffer_energy;	// �õ���������
    max_power_limit = user_data.robot_status.chassis_power_limit;    // �õ����������

    PID_buffer_init(&PID_Buffer);
    PID_buffer(&PID_Buffer, chassis_power_buffer, 30);  // ���������ջ�

    chassis_max_power = input_power;

    input_power = max_power_limit - PID_Buffer.OUT.ALL_OUT;  // ���뻺������

    if(capData_t.capGetDate.capVolt > capValt)
	{	
        if(cap_state == 0)
        {
            chassis_max_power = input_power + PowerCompensation;    // ���������Դ���������빦�ʣ���ߵ�������������
        }else{
            chassis_max_power = input_power + SuperMaxPower;        // ��������
        }
    }else{
        chassis_max_power = input_power;    // ���ݵ����ͻ��������ʱ�޲���
    }

    //�õ���ʼ�������
    initial_give_power[0] = get_initial_power(&MOTOR_V_CHASSIS[0]);
    initial_give_power[1] = get_initial_power(&MOTOR_V_CHASSIS[1]);
    initial_give_power[2] = get_initial_power(&MOTOR_V_CHASSIS[2]);
    initial_give_power[3] = get_initial_power(&MOTOR_V_CHASSIS[3]);

    for(uint8_t i = 0; i < 4; i++)
    {
        if (initial_give_power[i] < 0) // �����Ǹ���(����綯��)
			continue;
		initial_total_power += initial_give_power[i]; // ��õ����ܹ���
    }
    
	if (initial_total_power > chassis_max_power) // ȷ���Ƿ���������
	{
		fp32 power_scale = chassis_max_power / initial_total_power;

		for(uint8_t i = 0; i < 4; i++)
		{
			scaled_give_power[i] = initial_give_power[i] * power_scale; // ������Ź���
		}
		
		//��ÿ������ֱ���й�������
		chassis_power_limit(&MOTOR_V_CHASSIS[0]);
		chassis_power_limit(&MOTOR_V_CHASSIS[1]);
		chassis_power_limit(&MOTOR_V_CHASSIS[2]);
		chassis_power_limit(&MOTOR_V_CHASSIS[3]);
	}
}

/**
  * @author: �
  * @performance: ������ʼ��㺯��
  * @parameter: ����ṹ��	
  * @time: 24-4-1			
  * @ReadMe: �����������ģ�ͼ���������
*/ 
float get_initial_power(TYPEDEF_MOTOR *chassis_power_control)
{
    float initial_power;

    int32_t speed_rpm = (int32_t)chassis_power_control->DATA.SPEED_NOW;
	
   initial_power = chassis_power_control ->PID_S.OUT.ALL_OUT * toque_coefficient * speed_rpm + \
                   k2 * speed_rpm * speed_rpm + \
                   a * chassis_power_control -> PID_S.OUT.ALL_OUT * chassis_power_control -> PID_S.OUT.ALL_OUT + constant;    

    return initial_power;
}

/**
  * @author: �
  * @performance: ����������ƺ���
  * @parameter: ����ṹ��	
  * @time: 24-4-1			
  * @ReadMe: �Ե�����ʽ������Ž��й����ٷ���
*/ 
void chassis_power_limit(TYPEDEF_MOTOR *chassis_power_limit)
{
    uint8_t p = 0;

    if (chassis_power_limit == &MOTOR_V_CHASSIS[0]){
        p = 0;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[1]){
        p = 1;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[2]){
        p = 2;
    }else if(chassis_power_limit == &MOTOR_V_CHASSIS[3]){
        p = 3;
    }

    if(scaled_give_power[p] < 0)
    {
        return;
    }

    fp32 speed_rpm = (int32_t)chassis_power_limit->DATA.SPEED_NOW;
    fp32 b = toque_coefficient * speed_rpm;
    fp32 c = k2 * speed_rpm * speed_rpm - scaled_give_power[p] + constant;

	
    if(chassis_power_limit->PID_S.OUT.ALL_OUT > 0)  // ����ԭ���ط���ѡ����㹫ʽ
    {
        fp32 temp = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        if(temp > 16000)
        {
            chassis_power_limit->PID_S.OUT.ALL_OUT = 16000;
        }else{
             chassis_power_limit -> PID_S.OUT.ALL_OUT = temp;
        }
    }else {
        fp32 temp = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        if (temp < -16000)
		{
             chassis_power_limit -> PID_S.OUT.ALL_OUT = -16000;
        }else{
            chassis_power_limit -> PID_S.OUT.ALL_OUT = temp;
        }
    }
}

/**
  * @author: �
  * @performance: ��������PID����
  * @parameter: PID �������� Ҫ��ʣ�����ͻ�������	
  * @time: 24-4-1			
  * @ReadMe: 
*/ 
void PID_buffer(TYPEDEF_MOTOR_PID *PID_buffer, fp32 power_buffer, fp32 temp)
{
    PID_buffer->OUT.ERROR[0] = temp - power_buffer;
    /*�������*/
    PID_buffer->OUT.P_OUT = ( PID_buffer->OUT.ERROR[0] * PID_buffer->IN.KP);
    /*�������*/
    PID_buffer->OUT.I_OUT += (PID_buffer->OUT.ERROR[0] * PID_buffer->IN.KI);
    /*�����޷�*/
    PID_buffer->OUT.I_OUT = SectionLimit_f(PID_buffer->IN.I_LIT, -PID_buffer->IN.I_LIT, PID_buffer->OUT.I_OUT);
    /*΢�����*/
    PID_buffer->OUT.D_OUT = -(PID_buffer->OUT.ERROR[0] - PID_buffer->OUT.ERROR[1]) * PID_buffer->IN.KD;
    /*���ݵ���*/
    PID_buffer->OUT.ERROR[1] = PID_buffer->OUT.ERROR[0];
    /*�ǶȻ������*/
    PID_buffer->OUT.ALL_OUT = (PID_buffer->OUT.P_OUT + PID_buffer->OUT.I_OUT + PID_buffer->OUT.D_OUT);
    /*������޷�*/
    PID_buffer->OUT.ALL_OUT = SectionLimit_f(PID_buffer->IN.ALL_LIT, -PID_buffer->IN.ALL_LIT, PID_buffer->OUT.ALL_OUT);
}

/**
  * @author: �
  * @performance: PID��ʼ��
  * @parameter: PID	
  * @time: 24-4-1			
  * @ReadMe: 
*/ 
void PID_buffer_init(TYPEDEF_MOTOR_PID *PID_buffer)
{
    PID_buffer->IN.KP = 2;
    PID_buffer->IN.KI = 0;
    PID_buffer->IN.KD = 0;
    PID_buffer->IN.I_LIT = 0;
    PID_buffer->IN.ALL_LIT = 100;
}

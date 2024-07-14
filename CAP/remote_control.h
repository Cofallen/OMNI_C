/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       remote_control.c/h
  * @brief      ң��������ң������ͨ������SBUS��Э�鴫�䣬����DMA���䷽ʽ��ԼCPU
  *             ��Դ�����ô��ڿ����ж�������������ͬʱ�ṩһЩ��������DMA������
  *             �ķ�ʽ��֤�Ȳ�ε��ȶ��ԡ�
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "main.h"
#include "stdint.h"
#include "shu_math.h"
#include "attack.h"

#define SBUS_RX_BUF_NUM 36u

#define RC_FRAME_LENGTH 18u

#define RC_CH_VALUE_MIN         ((uint16_t)364)
#define RC_CH_VALUE_OFFSET      ((uint16_t)1024)
#define RC_CH_VALUE_MAX         ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define KeyboardBoll (rc_ctrl.control.s1_u8 == CarKeyboard)		//����ģʽ���
#define KeyWindmillBoll rc_ctrl.control.s1_u8	//���ģʽ���
#define Mouse_VX rc_ctrl.mouse.x_filter		//�������ٶ�
#define Mouse_VY rc_ctrl.mouse.y_filter		//��������ٶ�
#define Mouse_R rc_ctrl.mouse.r_state	  //�������ٶ�
#define Mouse_L rc_ctrl.mouse.l_state		//��������ٶ�
//����ʽ��������
#define KeyBoard_ctrl rc_ctrl.key_board.ctrl		//Ctrl������
#define KeyBoard_a rc_ctrl.key_board.a		//a������
#define KeyBoard_s rc_ctrl.key_board.s		//s������
#define KeyBoard_d rc_ctrl.key_board.d		//d������
#define KeyBoard_w rc_ctrl.key_board.w		//w������
#define KeyBoard_q rc_ctrl.key_board.q		//q������
#define KeyBoard_e rc_ctrl.key_board.e		//e������
#define KeyBoard_r rc_ctrl.key_board.r		//r������
#define KeyBoard_t rc_ctrl.key_board.t		//t������
#define KeyBoard_g rc_ctrl.key_board.g		//g������
#define KeyBoard_z rc_ctrl.key_board.z		//z������
#define KeyBoard_x rc_ctrl.key_board.x		//x������
#define KeyBoard_c rc_ctrl.key_board.c		//c������
#define KeyBoard_v rc_ctrl.key_board.v		//v������
#define KeyBoard_b rc_ctrl.key_board.b		//b������
#define KeyBoard_shift rc_ctrl.key_board.shift		//shift������
//����ʽ��������
#define KeyBoard_qLock rc_ctrl.key_board.q_preeNumber 
#define KeyBoard_eLock rc_ctrl.key_board.e_preeNumber 
#define KeyBoard_rLock rc_ctrl.key_board.r_preeNumber 
#define KeyBoard_fLock rc_ctrl.key_board.f_preeNumber
#define KeyBoard_gLock rc_ctrl.key_board.g_preeNumber 
#define KeyBoard_zLock rc_ctrl.key_board.z_preeNumber 	
#define KeyBoard_xLock rc_ctrl.key_board.x_preeNumber 	
#define KeyBoard_cLock rc_ctrl.key_board.c_preeNumber 
#define KeyBoard_vLock rc_ctrl.key_board.v_preeNumber
#define KeyBoard_bLock rc_ctrl.key_board.b_preeNumber 
#define KeyBoard_shiftLock rc_ctrl.key_board.shift_preeNumber 
#define KeyBoard_ctrlLock rc_ctrl.key_board.ctrl_preeNumber 


#ifndef df_now
	#define df_now 0
	#define df_last 1
	#define df_llast 2
#endif

#ifndef df_passState
	//���״̬
	#define df_click 1	//�㰴
	#define df_uplift 3		//�ɿ�
	#define df_press 2	//����
	//����״̬
	#define df_click 1	//�㰴
	#define df_keyuplift 0		//�ɿ�
	#define df_press 2	//����
#endif
/* ----------------------- Data Struct ------------------------------------- */
typedef __packed struct
{
     __packed struct
	      {	
	        	int16_t ch0_int16;
	        	int16_t ch1_int16;
	        	int16_t ch2_int16;
	        	int16_t ch3_int16;
	        	uint8_t s1_u8;
	        	uint8_t s2_u8;
	        	int16_t direction;
	        	int8_t Bool_i8;			//�����ݲ���������ʱ�ļ�����
	       } control; 
				
     __packed struct
        {
                int16_t x;
                int16_t y;
                int16_t z;
                uint8_t press_l;
                uint8_t press_r;
					
					float x_flt;
		      float x_filter;		//�����ʹ�õ����ֵ
		      float x_max;	//��¼�������ֵ����У׼���		
		      float y_flt;
		      float y_filter;		//�����ʹ�õ����ֵ
		      float y_max;	//��¼�������ֵ����У׼���
		      uint8_t l_state:4;
		      uint8_t r_state:4;
		      uint8_t l_pressTime;
		      uint8_t r_pressTime;
        } mouse;
      __packed struct
	      {
		       uint8_t w:4;
		       uint8_t s:4;
		       uint8_t a:4;
		       uint8_t d:4;
		       uint8_t shift:4;		//С����
		       uint8_t ctrl:4;		//����
		       uint8_t q:4;
		       uint8_t e:4;
		       uint8_t r:4;
		       uint8_t f:4;
		       uint8_t g:4;
		       uint8_t z:4;
		       uint8_t x:4;
		       uint8_t c:4;
		       uint8_t v:4;
		       uint8_t b:4;
		       uint8_t w_pressTime;
		       uint8_t s_pressTime;
		       uint8_t a_pressTime;
		       uint8_t d_pressTime;
		       uint8_t q_preeNumber:1;		//�糵ģʽ
		       uint8_t e_preeNumber:1;		//����״̬
		       uint8_t r_preeNumber:1;		
		       uint8_t f_preeNumber:1;
		       uint8_t g_preeNumber:1;
		       uint8_t z_preeNumber:1;
		       uint8_t x_preeNumber:1;
		       uint8_t c_preeNumber:1;//V��B����ռ�ò�������
			     uint8_t b_preeNumber:1;
			     uint8_t v_preeNumber:1;

		       uint8_t shift_preeNumber:1;
		       uint8_t ctrl_preeNumber:1;
	    } key_board;

} RC_ctrl_t;

extern RC_ctrl_t rc_ctrl;

union remote_union_typdef		//ʹ�ù�������������
{
	struct {
		//ң������
		uint64_t ch0:11;
		uint64_t ch1:11;
		uint64_t ch2:11;
		uint64_t ch3:11;
		uint64_t s1:2;
		uint64_t s2:2;
		//�������
		int64_t	mouse_x:16;
		int64_t	mouse_y:16;
		int64_t	mouse_z:16;
		int64_t	mouse_l:8;
		int64_t	mouse_r:8;
		//��������
		uint64_t keyboard_w:1;
		uint64_t keyboard_s:1;
		uint64_t keyboard_a:1;
		uint64_t keyboard_d:1;
		uint64_t keyboard_shift:1;
		uint64_t keyboard_ctrl:1;
		uint64_t keyboard_q:1;
		uint64_t keyboard_e:1;
		uint64_t keyboard_r:1;
		uint64_t keyboard_f:1;
		uint64_t keyboard_g:1;
		uint64_t keyboard_z:1;
		uint64_t keyboard_x:1;
		uint64_t keyboard_c:1;
		uint64_t keyboard_v:1;
		uint64_t keyboard_b:1;
		//ң�ػ���
		uint64_t direction:11;
		uint64_t :0;
	}dataNeaten;
	//���յ�������
	uint8_t getData[18];
};
extern union remote_union_typdef remote_union_t;	

extern float shu_temp5;

//ң�����ݴ���
void Resovled_data(uint8_t * data);
//���ת����
void MouseToRemote(void);
//����˲�
float OneFilter(float last, float now, float thresholdValue);

extern void RC_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

/* ----------------------- Internal Data ----------------------------------- */

/**
  * @brief          remote control init
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          ң������ʼ��
  * @param[in]      none
  * @retval         none
  */
extern void remote_control_init(void);
/**
  * @brief          get remote control data point
  * @param[in]      none
  * @retval         remote control data point
  */
/**
  * @brief          ��ȡң��������ָ��
  * @param[in]      none
  * @retval         ң��������ָ��
  */
extern const RC_ctrl_t *get_remote_control_point(void);

#endif

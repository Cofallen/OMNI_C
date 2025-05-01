#include "main.h"
#include "robot.h"
#include "imu_temp_control_task.h"

#include "can.h"
#include "TIM_DEV.h"
#include "CAN_DEV.h"
#include "MOTOR.h"
#include "DEFINE.h"
#include "CHASSIS.h"
#include "DBUS.h"
#include "GIMBAL.h"
#include "ROOT.h"
#include "ATTACK.h"
#include "VOFA.h"
#include "TOP.h"
#include "VISION.h"
#include "Read_Data.h"
#include "YU_MATH.h"
#include "chassis_power_control.h"
#include "tim.h"
#include "niming.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "usb_device.h"
#include "bsp_dwt.h"

#include "ui_update.h"

// @todo 放到结构体里
float dt_pc = 0;

void StartIMUTask(void const * argument)
{
  MX_USB_DEVICE_Init();
  Imu_Init();
  portTickType currentIMU;
  currentIMU = xTaskGetTickCount();

  for(;;)
  {
    INS_Task();
	  
    vTaskDelayUntil(&currentIMU,1);
  }
}

void StartChassisTask(void const * argument)
{
  #ifndef LIFTED_DEBUG
  capData_JHB.Send_data_typedef.Send_data.switchControl = 0x01;  // �?启电�?
  #else
  capData_JHB.Send_data_typedef.Send_data.switchControl = 0x01;
  #endif // LIFTED_DEBUG

  for(;;)
  {
    CHASSIS_F_Ctl(MOTOR_V_CHASSIS, &DBUS_V_DATA);
    CAN_F_Send(&hcan1, 0x200, MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CAN_SEND,
                MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CAN_SEND,
                MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CAN_SEND,
                MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CAN_SEND);
		// CapSendInit(40 , user_data.power_heat_data.chassis_power , user_data.power_heat_data.chassis_voltage);
    CapSend_new( user_data.power_heat_data.buffer_energy ,  user_data.power_heat_data.chassis_voltage);    // 电容电量低或电容离线时无补偿
    vTaskDelay(1);
  }
}

void StartGimbalTask(void const * argument)
{
  DWT_Init(168);
  dt_pc = 0;
  static uint32_t INS_DWT_Count = 0;
	MOTOR_V_GIMBAL[0].DATA.AIM = TOP.yaw[5];

  for(;;)
  {
      GIMBAL_F_Ctl(MOTOR_V_GIMBAL, &DBUS_V_DATA, &VISION_V_DATA);
      CAN_F_Send(&hcan2, 0x1FF, 0,
                MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.CAN_SEND,
                0,
                MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.CAN_SEND);
      // 自瞄查看数据
      Vofa_intergrate(3);
      dt_pc = (float)DWT_GetDeltaT(&INS_DWT_Count);
      vTaskDelay(1);
  }
}

void StartAttackTask(void const * argument)
{
  for(;;)
  {
    ATTACK_F_Ctl(MOTOR_V_ATTACK, &DBUS_V_DATA);
    CAN_F_Send(&hcan2, 0x200, MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.CAN_SEND,
              MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.CAN_SEND,
              MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.CAN_SEND,
              0);
    vTaskDelay(1);
  }
}

void StartMonitorTask(void const * argument)
{
  for(;;)
  {  
    ROOT_F_MONITOR_DBUS(&DBUS_V_DATA);
    TOP_T_Monitor();
    // VISION_F_Monitor();
    TOP_T_Cal();
    // TOP_T_Cal_T();
    vTaskDelay(1);
  }
}

void StartvisionTask(void const * argument)
{
  RobotUI_Static_Init();
  for(;;)
  {
      // input
      RobotUI_Dynamic();
      // RobotUI_Static_Init();
	  // if(xSemaphoreTake(binarySemHandle, pdMS_TO_TICKS(1000)) == pdTRUE)
    //   {  
    //   // ControltoVision(&VISION_V_DATA.SEND ,sd_v_buff, 1);
    //   xSemaphoreGive(binarySemHandle);
	  // }
    vTaskDelay(1);
  }
}
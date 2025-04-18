/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "imu_temp_control_task.h"

#include "main.h"
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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId IMUTaskHandle;
osThreadId chassisTaskHandle;
osThreadId gimbalTaskHandle;
osThreadId attackTaskHandle;
osThreadId monitorTaskHandle;
osThreadId visionTaskHandle;
osSemaphoreId binarySemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartIMUTask(void const * argument);
void StartChassisTask(void const * argument);
void StartGimbalTask(void const * argument);
void StartAttackTask(void const * argument);
void StartMonitorTask(void const * argument);
void StartvisionTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of binarySem */
  osSemaphoreDef(binarySem);
  binarySemHandle = osSemaphoreCreate(osSemaphore(binarySem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of IMUTask */
  osThreadDef(IMUTask, StartIMUTask, osPriorityNormal, 0, 512);
  IMUTaskHandle = osThreadCreate(osThread(IMUTask), NULL);

  /* definition and creation of chassisTask */
  osThreadDef(chassisTask, StartChassisTask, osPriorityIdle, 0, 256);
  chassisTaskHandle = osThreadCreate(osThread(chassisTask), NULL);

  /* definition and creation of gimbalTask */
  osThreadDef(gimbalTask, StartGimbalTask, osPriorityIdle, 0, 256);
  gimbalTaskHandle = osThreadCreate(osThread(gimbalTask), NULL);

  /* definition and creation of attackTask */
  osThreadDef(attackTask, StartAttackTask, osPriorityIdle, 0, 256);
  attackTaskHandle = osThreadCreate(osThread(attackTask), NULL);

  /* definition and creation of monitorTask */
  osThreadDef(monitorTask, StartMonitorTask, osPriorityIdle, 0, 128);
  monitorTaskHandle = osThreadCreate(osThread(monitorTask), NULL);

  /* definition and creation of visionTask */
  osThreadDef(visionTask, StartvisionTask, osPriorityLow, 0, 1024);
  visionTaskHandle = osThreadCreate(osThread(visionTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartIMUTask */
/**
  * @brief  Function implementing the IMUTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartIMUTask */
__weak void StartIMUTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartIMUTask */

  Imu_Init();
  portTickType currentIMU;
  currentIMU = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    INS_Task();
    vTaskDelayUntil(&currentIMU,1);
  }
  /* USER CODE END StartIMUTask */
}

/* USER CODE BEGIN Header_StartChassisTask */
/**
* @brief Function implementing the chassisTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartChassisTask */
__weak void StartChassisTask(void const * argument)
{
  /* USER CODE BEGIN StartChassisTask */
  capData_JHB.Send_data_typedef.Send_data.switchControl = 0x00;
  /* Infinite loop */
  for(;;)
  {
    CHASSIS_F_Ctl(MOTOR_V_CHASSIS, &DBUS_V_DATA);
    CAN_F_Send(&hcan1, 0x200, MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CAN_SEND,
               MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CAN_SEND,
               MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CAN_SEND,
               MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CAN_SEND);
		// CapSendInit(40 , user_data.power_heat_data.chassis_power , user_data.power_heat_data.chassis_voltage);
    CapSend_new( user_data.power_heat_data.buffer_energy ,  user_data.power_heat_data.chassis_voltage);
    vTaskDelay(1);
  }
  /* USER CODE END StartChassisTask */
}

/* USER CODE BEGIN Header_StartGimbalTask */
/**
* @brief Function implementing the gimbalTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGimbalTask */
__weak void StartGimbalTask(void const * argument)
{
  /* USER CODE BEGIN StartGimbalTask */
	MOTOR_V_GIMBAL[0].DATA.AIM = TOP.yaw[5];
  /* Infinite loop */
	
  for(;;)
  {
      GIMBAL_F_Ctl(MOTOR_V_GIMBAL, &DBUS_V_DATA, &VISION_V_DATA);
      CAN_F_Send(&hcan2, 0x1FF, 0,
                MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.CAN_SEND,
                0,
                MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.CAN_SEND);
      // 自瞄查看数据
      // VOFA_T_SendTemp(10, 0.0f,
      //       (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INFINITE,
      //       (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.ANGLE_NOW,
      //       (float)VISION_V_DATA.RECV_FLAG,
      //       (float)VISION_V_DATA.RECEIVE.TARGET,
      //       (float)VISION_V_DATA.RECEIVE.YAW_DATA,
      //       (float)TOP.yaw[5],
      //       (float)(TOP.yaw[5] * 22.75555f),
      //       (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].PID_S.OUT.ALL_OUT,
      //       99.0f);
      // 飞坡查看数据
      // VOFA_T_SendTemp(10, 0.0f,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CURRENT,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CURRENT,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CURRENT,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CURRENT,
      //       (float)DBUS_V_DATA.is_front_lifted,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.AIM,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.AIM,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.AIM,
      //       (float)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.AIM);
        VOFA_T_SendTemp(10, 0.0f,
                watch[0], watch[1], watch[2], watch[3], watch[4], watch[5], 
                watch[6], watch[7], watch[8]);
      // 飞坡匿名查看数据
      // niming(0xF1, 
      //       (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_1].DATA.CURRENT,
      //       (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_2].DATA.CURRENT,
      //       (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_3].DATA.CURRENT,
      //       (int16_t)MOTOR_V_CHASSIS[MOTOR_D_CHASSIS_4].DATA.CURRENT, // y1
      //       (int16_t)DBUS_V_DATA.is_front_lifted,                     // y2
      //       0,                                                       // y3
      //       0, 0, 0,                                                // z1-z3
      //       0);  
      // send_sensor_data();                                                    // w
      // VOFA_T_Send(5, 1.0f,
      //                (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.AIM,
      //                (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_PIT].DATA.ANGLE_NOW,
      //                (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.AIM,
      //                (float)MOTOR_V_GIMBAL[MOTOR_D_GIMBAL_YAW].DATA.ANGLE_INFINITE);
//      xSemaphoreGive(binarySemHandle);
//    }
    vTaskDelay(5);
  }
  /* USER CODE END StartGimbalTask */
}

/* USER CODE BEGIN Header_StartAttackTask */
/**
* @brief Function implementing the attackTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAttackTask */
__weak void StartAttackTask(void const * argument)
{
  /* USER CODE BEGIN StartAttackTask */
  /* Infinite loop */
  for(;;)
  {
    ATTACK_F_Ctl(MOTOR_V_ATTACK, &DBUS_V_DATA);
    CAN_F_Send(&hcan2, 0x200, MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.CAN_SEND,
              MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.CAN_SEND,
              MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.CAN_SEND,
              0);
    vTaskDelay(1);
  }
  /* USER CODE END StartAttackTask */
}

/* USER CODE BEGIN Header_StartMonitorTask */
/**
* @brief Function implementing the monitorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMonitorTask */
__weak void StartMonitorTask(void const * argument)
{
  /* USER CODE BEGIN StartMonitorTask */
  /* Infinite loop */
  for(;;)
  {  
    ROOT_F_MONITOR_DBUS(&DBUS_V_DATA);
    TOP_T_Monitor();
    // VISION_F_Monitor();
    TOP_T_Cal();
    // TOP_T_Cal_T();
    vTaskDelay(1);
  /* USER CODE END StartMonitorTask */
  }
}

/* USER CODE BEGIN Header_StartvisionTask */
/**
* @brief Function implementing the visionTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartvisionTask */
__weak void StartvisionTask(void const * argument)
{
  /* USER CODE BEGIN StartvisionTask */
  /* Infinite loop */
  for(;;)
  {
      // input
	  if(xSemaphoreTake(binarySemHandle, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
      // ControltoVision(&VISION_V_DATA.SEND ,sd_v_buff, 1);
      xSemaphoreGive(binarySemHandle);
	  }
//    }
    // VOFA_T_Vision();
    // VOFA_T_SendTemp(9, 0.0f,  // debug yaw pid with top[3]
    //           (float)user_data.shoot_data.initial_speed,
    //           (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_L].DATA.SPEED_NOW,
    //           (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.SPEED_NOW,
    //           (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_R].DATA.AIM,
    //           (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.AIM,
    //           (float)MOTOR_V_ATTACK[MOTOR_D_ATTACK_G].DATA.ANGLE_INFINITE,
    //           (float)DBUS_V_DATA.IS_OFF,
    //           1.0f);
    vTaskDelay(1);
  }
 
  /* USER CODE END StartvisionTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

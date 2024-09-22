#ifndef IMU_TEMP_TASK_H
#define IMU_TEMP_TASK_H





/**
  * @brief          bmi088 temperature control 
  * @param[in]      argument: NULL
  * @retval         none
  */
/**
  * @brief          bmi088温度控制
  * @param[in]      argument: NULL
  * @retval         none
  */
void imu_temp_control_task(void const * argument);
void INS_Task(void);
void Imu_Init(void);
#endif




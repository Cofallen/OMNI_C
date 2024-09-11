/**
 * @Author         : Minghang Li
 * @Date           : 2022-11-25 22:54
 * @LastEditTime   : 2022-11-28 16:32
 * @Note           :
 * @Copyright(c)   : Minghang Li Copyright
 */
#include "bmi088.h"

#include <math.h>

#include "bmi088reg.h"
#include "gpio.h"
#include "spi.h"
#include "YU_MATH.h"

bmi088_error_e BMI088_INIT(void)
{
    bmi088_error_e error = NO_ERROR;

    BMI088_CONF_INIT();

    error |= VerifyAccChipID();
    error |= VerifyGyroChipID();
    if (1)
    { // 将来改成变量控制自检
        error |= VerifyAccSelfTest();
    }
    if (1)
    { // 将来改成变量控制自检
        error |= VerifyGyroSelfTest();
    }
    return error;
}

void WriteDataToAcc(uint8_t addr, uint8_t data)
{
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_RESET);
    uint8_t pTxData = (addr & BMI088_SPI_WRITE_CODE);
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    pTxData = data;
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_Delay(1);
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_SET);
}

void WriteDataToGyro(uint8_t addr, uint8_t data)
{
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_RESET);
    uint8_t pTxData = (addr & BMI088_SPI_WRITE_CODE);
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    pTxData = data;
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_Delay(1);
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_SET);
}

void ReadSingleDataFromAcc(uint8_t addr, uint8_t *data)
{
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_RESET);
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_SPI_Receive(&BMI088_SPI, data, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
        ;
    HAL_SPI_Receive(&BMI088_SPI, data, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
        ;
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_SET);
}

void ReadSingleDataFromGyro(uint8_t addr, uint8_t *data)
{
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_RESET);
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_SPI_Receive(&BMI088_SPI, data, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
        ;
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_SET);
}

void ReadMultiDataFromAcc(uint8_t addr, uint8_t len, uint8_t *data)
{
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_RESET);
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
    uint8_t pRxData;
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_SPI_Receive(&BMI088_SPI, &pRxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
        ;
    for (int i = 0; i < len; i++)
    {
        HAL_SPI_Receive(&BMI088_SPI, &pRxData, 1, 1000);
        while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
            ;
        data[i] = pRxData;
    }
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_SET);
}

void ReadMultiDataFromGyro(uint8_t addr, uint8_t len, uint8_t *data)
{
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_RESET);
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
    uint8_t pRxData;
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    for (int i = 0; i < len; i++)
    {
        HAL_SPI_Receive(&BMI088_SPI, &pRxData, 1, 1000);
        while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
            ;
        data[i] = pRxData;
    }
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_SET);
}

void BMI088_CONF_INIT(void)
{
    // 加速度计初始化
    // 先软重启，清空所有寄存器
    WriteDataToAcc(ACC_SOFTRESET_ADDR, ACC_SOFTRESET_VAL);
    HAL_Delay(50);
    // 打开加速度计电源
    WriteDataToAcc(ACC_PWR_CTRL_ADDR, ACC_PWR_CTRL_ON);
    // 加速度计变成正常模式
    WriteDataToAcc(ACC_PWR_CONF_ADDR, ACC_PWR_CONF_ACT);

    // 陀螺仪初始化
    // 先软重启，清空所有寄存器
    WriteDataToGyro(GYRO_SOFTRESET_ADDR, GYRO_SOFTRESET_VAL);
    HAL_Delay(50);
    // 陀螺仪变成正常模式
    WriteDataToGyro(GYRO_LPM1_ADDR, GYRO_LPM1_NOR);

    // 加速度计配置写入
    // 写入范围，+-3g的测量范围
    WriteDataToAcc(ACC_RANGE_ADDR, ACC_RANGE_3G);
    // 写入配置，正常带宽，1600hz输出频率
    WriteDataToAcc(ACC_CONF_ADDR,
                   (ACC_CONF_RESERVED << 7) | (ACC_CONF_BWP_NORM << 6) | (ACC_CONF_ODR_1600_Hz));

     // 陀螺仪配置写入
    // 写入范围，+-500°/s的测量范围
    WriteDataToGyro(GYRO_RANGE_ADDR, GYRO_RANGE_500_DEG_S);
     // 写入带宽，2000Hz输出频率，532Hz滤波器带宽
    WriteDataToGyro(GYRO_BANDWIDTH_ADDR, GYRO_ODR_2000Hz_BANDWIDTH_532Hz);
}

bmi088_error_e VerifyAccChipID(void)
{
    uint8_t chip_id;
    ReadSingleDataFromAcc(ACC_CHIP_ID_ADDR, &chip_id);
    if (chip_id != ACC_CHIP_ID_VAL)
    {
        return ACC_CHIP_ID_ERR;
    }
    return NO_ERROR;
}

bmi088_error_e VerifyGyroChipID(void)
{
    uint8_t chip_id;
    ReadSingleDataFromGyro(GYRO_CHIP_ID_ADDR, &chip_id);
    if (chip_id != GYRO_CHIP_ID_VAL)
    {
        return GYRO_CHIP_ID_ERR;
    }
    return NO_ERROR;
}

bmi088_error_e VerifyAccSelfTest(void)
{
    acc_raw_data_t pos_data, neg_data;
    WriteDataToAcc(ACC_RANGE_ADDR, ACC_RANGE_24G);
    WriteDataToAcc(ACC_CONF_ADDR, 0xA7);
    HAL_Delay(10);
    WriteDataToAcc(ACC_SELF_TEST_ADDR, ACC_SELF_TEST_POS);
    HAL_Delay(100);
    ReadAccData(&pos_data);
    WriteDataToAcc(ACC_SELF_TEST_ADDR, ACC_SELF_TEST_NEG);
    HAL_Delay(100);
    ReadAccData(&neg_data);
    WriteDataToAcc(ACC_SELF_TEST_ADDR, ACC_SELF_TEST_OFF);
    HAL_Delay(100);
    if ((fabs(pos_data.x - neg_data.x) > 0.1f) || (fabs(pos_data.y - neg_data.y) > 0.1f) || (fabs(pos_data.z - neg_data.z) > 0.1f))
    {
        return ACC_DATA_ERR;
    }
    WriteDataToAcc(ACC_SOFTRESET_ADDR, ACC_SOFTRESET_VAL);
    WriteDataToAcc(ACC_PWR_CTRL_ADDR, ACC_PWR_CTRL_ON);
    WriteDataToAcc(ACC_PWR_CONF_ADDR, ACC_PWR_CONF_ACT);
    WriteDataToAcc(ACC_CONF_ADDR,
                   (ACC_CONF_RESERVED << 7) | (ACC_CONF_BWP_NORM << 6) | (ACC_CONF_ODR_1600_Hz));
    WriteDataToAcc(ACC_RANGE_ADDR, ACC_RANGE_3G);
    return NO_ERROR;
}

bmi088_error_e VerifyGyroSelfTest(void)
{
    WriteDataToGyro(GYRO_SELF_TEST_ADDR, GYRO_SELF_TEST_ON);
    uint8_t bist_rdy = 0x00, bist_fail;
    while (bist_rdy == 0)
    {
        ReadSingleDataFromGyro(GYRO_SELF_TEST_ADDR, &bist_rdy);
        bist_rdy = (bist_rdy & 0x02) >> 1;
    }
    ReadSingleDataFromGyro(GYRO_SELF_TEST_ADDR, &bist_fail);
    bist_fail = (bist_fail & 0x04) >> 2;
    if (bist_fail == 0)
    {
        return NO_ERROR;
    }
    else
    {
        return GYRO_DATA_ERR;
    }
}

///
/// @brief  低通滤波

#define ALPHA 0.5  // 滤波系数，范围在0到1之间
#define FILTER_ON 1 // 滤波开关

void LowPassFilter(float *current_data, float new_data) {
    *current_data = ALPHA * (*current_data) + (1 - ALPHA) * new_data;
}

void ReadAccData(acc_raw_data_t *data)
{
    uint8_t buf[ACC_XYZ_LEN], range;
    int16_t acc[3];
    ReadSingleDataFromAcc(ACC_RANGE_ADDR, &range);
    ReadMultiDataFromAcc(ACC_X_LSB_ADDR, ACC_XYZ_LEN, buf);
    acc[0] = ((int16_t)buf[1] << 8) + (int16_t)buf[0];
    acc[1] = ((int16_t)buf[3] << 8) + (int16_t)buf[2];
    acc[2] = ((int16_t)buf[5] << 8) + (int16_t)buf[4];
    data->x = (float)acc[0] * BMI088_ACCEL_3G_SEN;
    data->y = (float)acc[1] * BMI088_ACCEL_3G_SEN;
    data->z = (float)acc[2] * BMI088_ACCEL_3G_SEN;

    #ifdef FILTER_ON
    static float filtered_x = 0, filtered_y = 0, filtered_z = 0;
    LowPassFilter(&filtered_x, data->x);
    LowPassFilter(&filtered_y, data->y);
    LowPassFilter(&filtered_z, data->z);
    data->x = filtered_x;
    data->y = filtered_y;
    data->z = filtered_z;
    #endif
}

void ReadGyroData(gyro_raw_data_t *data)
{
    uint8_t buf[GYRO_XYZ_LEN], range;
    int16_t gyro[3];
    float unit;
    ReadSingleDataFromGyro(GYRO_RANGE_ADDR, &range);
    switch (range)
    {
    case 0x00:
        unit = 16.384;
        break;
    case 0x01:
        unit = 32.768;
        break;
    case 0x02:
        unit = 65.536;
        break;
    case 0x03:
        unit = 131.072;
        break;
    case 0x04:
        unit = 262.144;
        break;
    default:
        unit = 16.384;
        break;
    }
    ReadMultiDataFromGyro(GYRO_RATE_X_LSB_ADDR, GYRO_XYZ_LEN, buf);
    gyro[0] = ((int16_t)buf[1] << 8) + (int16_t)buf[0];
    gyro[1] = ((int16_t)buf[3] << 8) + (int16_t)buf[2];
    gyro[2] = ((int16_t)buf[5] << 8) + (int16_t)buf[4];
    data->roll = (float)gyro[0] / unit * DEG2SEC;
    data->pitch = (float)gyro[1] / unit * DEG2SEC;
    data->yaw = (float)gyro[2] / unit * DEG2SEC;

    #ifdef FILTER_ON
    static float filtered_roll = 0, filtered_pitch = 0, filtered_yaw = 0;
    LowPassFilter(&filtered_roll, data->roll);
    LowPassFilter(&filtered_pitch, data->pitch);
    LowPassFilter(&filtered_yaw, data->yaw);
    data->roll = filtered_roll;
    data->pitch = filtered_pitch;
    data->yaw = filtered_yaw;
    #endif
    
    // Compute delta time
    cp.acc_data.delta_time = cp.acc_data.sensor_time - cp.acc_data.prev_time;
    
    // Update absolute angles using integration
    cp.gyro_data.absolute[0] += data->roll * cp.acc_data.delta_time;
    cp.gyro_data.absolute[1] += data->pitch * cp.acc_data.delta_time;
    cp.gyro_data.absolute[2] += data->yaw * cp.acc_data.delta_time;
    
    // Save the current time for the next function call
    cp.acc_data.prev_time = cp.acc_data.sensor_time;

    cp.gyro_data.absolute[2] *= SCALR_FACTOR; // yaw
    LimitRange(&cp.gyro_data.absolute[2], 8192, 0);
}

void ReadAccSensorTime(float *time)
{
    uint8_t buf[SENSORTIME_LEN];
    ReadMultiDataFromAcc(SENSORTIME_0_ADDR, SENSORTIME_LEN, buf);
    *time = buf[0] * SENSORTIME_0_UNIT + buf[1] * SENSORTIME_1_UNIT + buf[2] * SENSORTIME_2_UNIT;
}

void ReadAccTemperature(float *temp)
{
    uint8_t buf[TEMP_LEN];
    ReadMultiDataFromAcc(TEMP_MSB_ADDR, TEMP_LEN, buf);
    uint16_t temp_uint11 = (buf[0] << 3) + (buf[1] >> 5);
    int16_t temp_int11;
    if (temp_uint11 > 1023)
    {
        temp_int11 = (int16_t)temp_uint11 - 2048;
    }
    else
    {
        temp_int11 = (int16_t)temp_uint11;
    }
    *temp = temp_int11 * TEMP_UNIT + TEMP_BIAS;
}

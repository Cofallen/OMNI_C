#ifndef NIMING_H
#define NIMING_H

#include <stdint.h>

typedef enum {
    DATA_U8,
    DATA_S16,
    DATA_FLOAT,
    // 扩展其他数据类型...
} DataType;

typedef struct {
    uint8_t frame_id;   // 0xF1~0xFA
    uint8_t s_addr;     // 源地址（根据硬件配置）
    uint8_t d_addr;     // 目标地址（上位机一般为0xFE）
    uint8_t data[64];   // 数据缓冲区（根据实际需求调整大小）
    uint16_t data_len;  // 当前数据长度
} FlexFrame;

// 初始化灵活帧
void flex_frame_init(FlexFrame *frame, uint8_t frame_id, uint8_t s_addr, uint8_t d_addr);

// 添加数据（自动处理小端）
void flex_frame_add_data(FlexFrame *frame, void *value, DataType type);

// 计算校验并返回完整帧（需释放内存）
uint8_t* flex_frame_package(FlexFrame *frame, uint16_t *total_len);

void send_sensor_data(void);

#endif
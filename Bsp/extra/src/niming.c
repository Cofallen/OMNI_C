#include "niming.h"
#include "main.h"
#include <stdlib.h>
#include "string.h"

void flex_frame_init(FlexFrame *frame, uint8_t frame_id, uint8_t s_addr, uint8_t d_addr) {
    frame->frame_id = frame_id;
    frame->s_addr = s_addr;
    frame->d_addr = d_addr;
    frame->data_len = 0;
}

void flex_frame_add_data(FlexFrame *frame, void *value, DataType type) {
    switch(type) {
        case DATA_U8: {
            uint8_t v = *(uint8_t*)value;
            frame->data[frame->data_len++] = v;
            break;
        }
        case DATA_S16: {
            int16_t v = *(int16_t*)value;
            frame->data[frame->data_len++] = v & 0xFF;
            frame->data[frame->data_len++] = (v >> 8) & 0xFF;
            break;
        }
        case DATA_FLOAT: {
            float v = *(float*)value;
            uint8_t *p = (uint8_t*)&v;
            for(int i=0; i<4; i++) {
                frame->data[frame->data_len++] = p[i];
            }
            break;
        }
    }
}

uint8_t* flex_frame_package(FlexFrame *frame, uint16_t *total_len) {
    // 计算总长度：帧头(1)+地址(2)+ID(1)+长度(2)+数据(n)+校验(2) = 8+n
    uint16_t data_len = frame->data_len;
    *total_len = 8 + data_len;
    uint8_t *buffer = malloc(*total_len);
    
    // 填充基础字段
    buffer[0] = 0xAB;  // HEAD
    buffer[1] = frame->s_addr;
    buffer[2] = frame->d_addr;
    buffer[3] = frame->frame_id;
    buffer[4] = data_len & 0xFF;   // LEN小端
    buffer[5] = (data_len >> 8) & 0xFF;
    
    // 填充数据
    memcpy(&buffer[6], frame->data, data_len);
    
    // 计算校验
    uint8_t sum = 0, add = 0;
    for(int i=0; i<6+data_len; i++) {
        sum += buffer[i];
        add += sum;
    }
    buffer[6+data_len] = sum;
    buffer[7+data_len] = add;
    
    return buffer;
}



// 硬件驱动层
#include "usart.h"
#include <string.h>

void ano_send_flex_frame(FlexFrame *frame) {
    uint16_t total_len;
    uint8_t *buffer = flex_frame_package(frame, &total_len);
    
    // DMA发送（注意：HAL_UART_Transmit_DMA会自动处理忙状态）
    HAL_UART_Transmit_DMA(&huart1, buffer, total_len);
    
    // 释放内存（实际应用中建议使用内存池或静态内存）
    free(buffer);
}

// 
void send_sensor_data(void) {
    FlexFrame frame;
    flex_frame_init(&frame, 0xF1, 0x01, 0xFE); // ID=F1, 源地址=0x01
    
    // 添加示例数据：U8类型状态 + S16类型X加速度 + Float温度
    uint8_t status = 0xAA;
    int16_t acc_x = 1500;
    float temp = 25.5f;
    
    flex_frame_add_data(&frame, &status, DATA_U8);
    flex_frame_add_data(&frame, &acc_x, DATA_S16);
    flex_frame_add_data(&frame, &temp, DATA_FLOAT);
    
    ano_send_flex_frame(&frame);
}
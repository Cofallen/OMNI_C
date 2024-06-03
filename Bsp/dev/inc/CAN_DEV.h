#ifndef __CAN_DEV_H__
#define __CAN_DEV_H__

extern void CAN_F_Recv(CAN_HandleTypeDef *hcan, uint32_t ID);
extern void CAN_F_Send(CAN_HandleTypeDef *hcan, int16_t stdid, int16_t num1, int16_t num2, int16_t num3, int16_t num4);
extern void CAN_Filter_Init(void);

#endif // !__CAN_DEV_H__

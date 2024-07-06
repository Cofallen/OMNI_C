#ifndef __USART_DEV_H__
#define __USART_DEV_H__

#include <stdio.h>

extern int fputc(int ch, FILE *f);

void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void USER_UART3_IRQHandler();

#endif // !__USART_DEV_H__
#include "usart.h"
#include "usbd_cdc_if.h"
#include "VISION.h"
#include "USB_DEV.h"

uint8_t usb_Transmit_task(ALL_Data_t *ALL)
{
	uint8_t status;
	
    ALL->channal.HEAD = 0xA5;

    ALL->channal.END = 0x5A;
	
	status = CDC_Transmit_FS(ALL->Data, 141);
	
	HAL_UART_Transmit_DMA(&huart6,ALL->Data,141);
	
	return status;
}
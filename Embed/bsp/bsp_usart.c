#include "bsp_usart.h"
#include "stdio.h"

int fputc(int ch, FILE *f){
	uint8_t temp[1] = {ch};
	HAL_UART_Transmit(&huart1, temp, 1, 1);
	return ch;
}



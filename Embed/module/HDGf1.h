#ifndef HDGF1_H
#define HDGF1_H

#include "stm32f1xx_hal.h"
#include "spi.h"

#define A0_SET HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET)//display data
#define A0_CLR HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)//control data

#define RES_SET HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET)//1
#define RES_CLR HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET)//0

#define CS_SET HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET)//0 valid
#define CS_CLR HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET)//1 invalid

#define delay_ms(x) HAL_Delay(x) //方便移植

void HDG_Init(void);

void HDG_display_ascii(char ch,uint8_t x,uint8_t y,uint8_t indent);
void HDG_display_str_line(char *str,uint8_t x,uint8_t y,uint8_t indent);
void HDG_display_clr_ch(uint8_t x,uint8_t y);
void HDG_display_clr_line(uint8_t y);
#endif

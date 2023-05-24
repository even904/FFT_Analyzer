#ifndef KEY_H
#define KEY_H

#include "stm32f1xx_hal.h"

#define KEY_UP 		0
#define KEY_PRESS 	1

#define KEY0 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)
#define KEY1 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)
#define KEY2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)
#define KEY3 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)
#define KEY4 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)

static uint8_t key_status[5];

void key_init(void);
void key_toggle_status(uint8_t i);
uint8_t get_key_status(uint8_t i);

#endif


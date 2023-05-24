#ifndef BSP_LED_H
#define BSP_LED_H

#include "stm32f1xx_hal.h"
#include "gpio.h"

void led_init(void);
uint8_t get_led_status(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
#endif

#include "bsp_led.h"

static uint8_t LED_Status;

void led_init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
	LED_Status = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
}	

uint8_t get_led_status(void)
{
	LED_Status = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
	return LED_Status;
}

void led_on(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
}

void led_off(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
}

void led_toggle(void)
{
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}
